/* SPDX License Indentifier: GPL-2.0 */

/* creation, modification - 002, 2026, Pedro Emanuel */

# define SCHED_FIXEDPOINT_SHIFT		10
# define SCHED_FIXEDPOINT_SCALE		(1L << SCHED_FIXEDPOINT_SHIFT)

#include <linux/sched.h>
#include <kern/thread.h>


#define __node_2_se(node) \
	rb_entry((node), struct sched_entity, run_node)

# define scale_load_down(w)					\
({								\
	unsigned long __w = (w);				\
								\
	if (__w)						\
		__w = max(2UL, __w >> SCHED_FIXEDPOINT_SHIFT);	\
	__w;							\
})

# define NICE_0_LOAD_SHIFT	(SCHED_FIXEDPOINT_SHIFT + SCHED_FIXEDPOINT_SHIFT)
# define scale_load(w)		((w) << SCHED_FIXEDPOINT_SHIFT)
#define NICE_0_LOAD		(1L << NICE_0_LOAD_SHIFT)

 void __BUILD_BUG_vruntime_op(void);

#define vruntime_op(A, OP_STR, B) ({				\
	s64 __res = 0;						\
								\
	if (!__builtin_strcmp(OP_STR, "-")) {			\
		__res = (s64)((A)-(B));				\
	} else {						\
		/* Unknown operator throws linker error: */	\
		__BUILD_BUG_vruntime_op();			\
	}							\
								\
	__res;						\
})

s64 entity_key(struct cfs_rq *cfs_rq, struct sched_entity *se)
{
	return vruntime_op(se->vruntime, "-", cfs_rq->zero_vruntime);
}


unsigned long avg_vruntime_weight(struct cfs_rq *cfs_rq, unsigned long w)
{
	if (cfs_rq->sum_shift)
		w = max(2UL, w >> cfs_rq->sum_shift);
	return w;
}

static void __update_inv_weight(struct load_weight *lw)
{
	unsigned long w;

	if (likely(lw->inv_weight))
		return;

	w = scale_load_down(lw->weight);

	if (BITS_PER_LONG > 32 && unlikely(w >= WMULT_CONST))
		lw->inv_weight = 1;
	else if (unlikely(!w))
		lw->inv_weight = WMULT_CONST;
	else
		lw->inv_weight = WMULT_CONST / w;
}

u64 __calc_delta(u64 delta_exec, unsigned long weight, struct load_weight *lw)
{
	return (delta_exec * weight) / lw->weight;
}

 void
__sum_w_vruntime_add(struct cfs_rq *cfs_rq, struct sched_entity *se)
{
	unsigned long weight = avg_vruntime_weight(cfs_rq, se->load.weight);
	s64 w_vruntime, key = entity_key(cfs_rq, se);

	w_vruntime = key * weight;
	WARN_ON_ONCE((w_vruntime >> 63) != (w_vruntime >> 62));

	cfs_rq->sum_w_vruntime += w_vruntime;
	cfs_rq->sum_weight += weight;
}

 void
sum_w_vruntime_add_paranoid(struct cfs_rq *cfs_rq, struct sched_entity *se)
{
	unsigned long weight;
	s64 key, tmp;

again:
	weight = avg_vruntime_weight(cfs_rq, se->load.weight);
	key = entity_key(cfs_rq, se);

	if (check_mul_overflow(key, weight, &key))
		goto overflow;

	if (check_add_overflow(cfs_rq->sum_w_vruntime, key, &tmp))
		goto overflow;

	cfs_rq->sum_w_vruntime = tmp;
	cfs_rq->sum_weight += weight;
	return;

overflow:
	/*
	 * There's gotta be a limit -- if we're still failing at this point
	 * there's really nothing much to be done about things.
	 */
	BUG_ON(cfs_rq->sum_shift >= 10);
	cfs_rq->sum_shift++;

	/*
	 * Note: \Sum (k_i * (w_i >> 1)) != (\Sum (k_i * w_i)) >> 1
	 */
	cfs_rq->sum_w_vruntime = 0;
	cfs_rq->sum_weight = 0;

	for (struct rb_node *node = cfs_rq->tasks_timeline.rb_leftmost;
	     node; node = rb_next(node))
		__sum_w_vruntime_add(cfs_rq, __node_2_se(node));

	goto again;
}

void
sum_w_vruntime_sub(struct cfs_rq *cfs_rq, struct sched_entity *se)
{
	unsigned long weight = avg_vruntime_weight(cfs_rq, se->load.weight);
	s64 key = entity_key(cfs_rq, se);

	cfs_rq->sum_w_vruntime -= key * weight;
	cfs_rq->sum_weight -= weight;
}


void
sum_w_vruntime_add(struct cfs_rq *cfs_rq, struct sched_entity *se)
{
	if (sched_feat(PARANOID_AVG))
		return sum_w_vruntime_add_paranoid(cfs_rq, se);

	__sum_w_vruntime_add(cfs_rq, se);
}

s64 entity_lag(struct cfs_rq *cfs_rq, struct sched_entity *se, u64 avruntime)
{
	u64 max_slice = cfs_rq_max_slice(cfs_rq) + TICK_NSEC;
	s64 vlag, limit;

	vlag = avruntime - se->vruntime;
	limit = calc_delta_fair(max_slice, se);

	return clamp(vlag, -limit, limit);
}

 u64 cfs_rq_max_slice(struct cfs_rq *cfs_rq)
{
	struct sched_entity *root = __pick_root_entity(cfs_rq);
	struct sched_entity *curr = cfs_rq->curr;
	u64 max_slice = 0ULL;

	if (curr && curr->on_rq)
		max_slice = curr->slice;

	if (root)
		max_slice = max(max_slice, root->max_slice);

	return max_slice;
}

struct sched_entity *__pick_root_entity(struct cfs_rq *cfs_rq)
{
	struct rb_node *root = cfs_rq->tasks_timeline.rb_root.rb_node;

	if (!root)
		return NULL;

	return __node_2_se(root);
}

u64 avg_vruntime(struct cfs_rq *cfs_rq)
{
	struct sched_entity *curr = cfs_rq->curr;
	long weight = cfs_rq->sum_weight;
	s64 delta = 0;

	if (curr && !curr->on_rq)
		curr = NULL;

	if (weight) {
		s64 runtime = cfs_rq->sum_w_vruntime;

		if (curr) {
			unsigned long w = avg_vruntime_weight(cfs_rq, curr->load.weight);

			runtime += entity_key(cfs_rq, curr) * w;
			weight += w;
		}

		/* sign flips effective floor / ceiling */
		if (runtime < 0)
			runtime -= (weight - 1);

		delta = div64_long(runtime, weight);
	} else if (curr) {
		/*
		 * When there is but one element, it is the average.
		 */
		delta = curr->vruntime - cfs_rq->zero_vruntime;
	}

	update_zero_vruntime(cfs_rq, delta);

	return cfs_rq->zero_vruntime;
}


void
place_entity(struct cfs_rq *cfs_rq, struct sched_entity *se, int flags)
{
	u64 vslice, vruntime = avg_vruntime(cfs_rq);
	bool update_zero = false;
	s64 lag = 0;

	if (!se->custom_slice)
		se->slice = sysctl_sched_base_slice;
	vslice = calc_delta_fair(se->slice, se);

	/*
	 * Due to how V is constructed as the weighted average of entities,
	 * adding tasks with positive lag, or removing tasks with negative lag
	 * will move 'time' backwards, this can screw around with the lag of
	 * other tasks.
	 *
	 * EEVDF: placement strategy #1 / #2
	 */
	if (sched_feat(PLACE_LAG) && cfs_rq->nr_queued && se->vlag) {
		struct sched_entity *curr = cfs_rq->curr;
		long load, weight;

		lag = se->vlag;

		/*
		 * If we want to place a task and preserve lag, we have to
		 * consider the effect of the new entity on the weighted
		 * average and compensate for this, otherwise lag can quickly
		 * evaporate.
		 *
		 * Lag is defined as:
		 *
		 *   lag_i = S - s_i = w_i * (V - v_i)
		 *
		 * To avoid the 'w_i' term all over the place, we only track
		 * the virtual lag:
		 *
		 *   vl_i = V - v_i <=> v_i = V - vl_i
		 *
		 * And we take V to be the weighted average of all v:
		 *
		 *   V = (\Sum w_j*v_j) / W
		 *
		 * Where W is: \Sum w_j
		 *
		 * Then, the weighted average after adding an entity with lag
		 * vl_i is given by:
		 *
		 *   V' = (\Sum w_j*v_j + w_i*v_i) / (W + w_i)
		 *      = (W*V + w_i*(V - vl_i)) / (W + w_i)
		 *      = (W*V + w_i*V - w_i*vl_i) / (W + w_i)
		 *      = (V*(W + w_i) - w_i*vl_i) / (W + w_i)
		 *      = V - w_i*vl_i / (W + w_i)
		 *
		 * And the actual lag after adding an entity with vl_i is:
		 *
		 *   vl'_i = V' - v_i
		 *         = V - w_i*vl_i / (W + w_i) - (V - vl_i)
		 *         = vl_i - w_i*vl_i / (W + w_i)
		 *
		 * Which is strictly less than vl_i. So in order to preserve lag
		 * we should inflate the lag before placement such that the
		 * effective lag after placement comes out right.
		 *
		 * As such, invert the above relation for vl'_i to get the vl_i
		 * we need to use such that the lag after placement is the lag
		 * we computed before dequeue.
		 *
		 *   vl'_i = vl_i - w_i*vl_i / (W + w_i)
		 *         = ((W + w_i)*vl_i - w_i*vl_i) / (W + w_i)
		 *
		 *   (W + w_i)*vl'_i = (W + w_i)*vl_i - w_i*vl_i
		 *                   = W*vl_i
		 *
		 *   vl_i = (W + w_i)*vl'_i / W
		 */
		load = cfs_rq->sum_weight;
		if (curr && curr->on_rq)
			load += avg_vruntime_weight(cfs_rq, curr->load.weight);

		weight = avg_vruntime_weight(cfs_rq, se->load.weight);
		lag *= load + weight;
		if (WARN_ON_ONCE(!load))
			load = 1;
		lag = div64_long(lag, load);

		/*
		 * A heavy entity (relative to the tree) will pull the
		 * avg_vruntime close to its vruntime position on enqueue. But
		 * the zero_vruntime point is only updated at the next
		 * update_deadline()/place_entity()/update_entity_lag().
		 *
		 * Specifically (see the comment near avg_vruntime_weight()):
		 *
		 *   sum_w_vruntime = \Sum (v_i - v0) * w_i
		 *
		 * Note that if v0 is near a light entity, both terms will be
		 * small for the light entity, while in that case both terms
		 * are large for the heavy entity, leading to risk of
		 * overflow.
		 *
		 * OTOH if v0 is near the heavy entity, then the difference is
		 * larger for the light entity, but the factor is small, while
		 * for the heavy entity the difference is small but the factor
		 * is large. Avoiding the multiplication overflow.
		 */
		if (weight > load)
			update_zero = true;
	}

	se->vruntime = vruntime - lag;

	if (update_zero)
		update_zero_vruntime(cfs_rq, -lag);

	if (sched_feat(PLACE_REL_DEADLINE) && se->rel_deadline) {
		se->deadline += se->vruntime;
		se->rel_deadline = 0;
		return;
	}

	/*
	 * When joining the competition; the existing tasks will be,
	 * on average, halfway through their slice, as such start tasks
	 * off with half a slice to ease into the competition.
	 */
	if (sched_feat(PLACE_DEADLINE_INITIAL) && (flags & ENQUEUE_INITIAL))
		vslice /= 2;

	/*
	 * EEVDF: vd_i = ve_i + r_i/w_i
	 */
	se->deadline = se->vruntime + vslice;
}

