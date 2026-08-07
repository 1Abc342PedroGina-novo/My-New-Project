/* SPDX-License-Identifier: GPL-2.0 */

/* modified by Pedro Emanuel in 2026 */
#ifndef LINUX_SCHED_H
#define LINUX_SCHED_H

#include "asm-generic/int-ll64.h"
#include <kern/thread.h>

/* Used in thread->__state: */
#define TASK_RUNNING			0x00000000
#define TASK_INTERRUPTIBLE		0x00000001
#define TASK_UNINTERRUPTIBLE		0x00000002
#define __TASK_STOPPED			0x00000004
#define __TASK_TRACED			0x00000008
/* Used in thread->exit_state: */
#define EXIT_DEAD			0x00000010
#define EXIT_ZOMBIE			0x00000020
#define EXIT_TRACE			(EXIT_ZOMBIE | EXIT_DEAD)
/* Used in thread->__state again: */
#define TASK_PARKED			0x00000040
#define TASK_DEAD			0x00000080
#define TASK_WAKEKILL			0x00000100
#define TASK_WAKING			0x00000200
#define TASK_NOLOAD			0x00000400
#define TASK_NEW			0x00000800
#define TASK_RTLOCK_WAIT		0x00001000
#define TASK_FREEZABLE			0x00002000
#define __TASK_FREEZABLE_UNSAFE	       (0x00004000 * IS_ENABLED(CONFIG_LOCKDEP))
#define TASK_FROZEN			0x00008000
#define TASK_STATE_MAX			0x00010000

#define TASK_ANY			(TASK_STATE_MAX-1)

/*
 * DO NOT ADD ANY NEW USERS !
 */
#define TASK_FREEZABLE_UNSAFE		(TASK_FREEZABLE | __TASK_FREEZABLE_UNSAFE)

/* Convenience macros for the sake of set_current_state: */
#define TASK_KILLABLE			(TASK_WAKEKILL | TASK_UNINTERRUPTIBLE)
#define TASK_STOPPED			(TASK_WAKEKILL | __TASK_STOPPED)
#define TASK_TRACED			__TASK_TRACED

#define TASK_IDLE			(TASK_UNINTERRUPTIBLE | TASK_NOLOAD)

/* Convenience macros for the sake of wake_up(): */
#define TASK_NORMAL			(TASK_INTERRUPTIBLE | TASK_UNINTERRUPTIBLE)

/* get_task_state(): */
#define TASK_REPORT			(TASK_RUNNING | TASK_INTERRUPTIBLE | \
					 TASK_UNINTERRUPTIBLE | __TASK_STOPPED | \
					 __TASK_TRACED | EXIT_DEAD | EXIT_ZOMBIE | \
					 TASK_PARKED)


struct load_weight {
	unsigned long			weight;
	u32				inv_weight;
};


struct rb_node {
	unsigned long  __rb_parent_color;
	struct rb_node *rb_right;
	struct rb_node *rb_left;
} __attribute__((aligned(sizeof(long))));
/* The alignment might seem pointless, but allegedly CRIS needs it */

struct rb_node_linked {
	struct rb_node		node;
	struct rb_node_linked	*prev;
	struct rb_node_linked	*next;
};

struct rb_root {
	struct rb_node *rb_node;
};

struct rb_root_cached {
	struct rb_root rb_root;
	struct rb_node *rb_leftmost;
};

/*
 * Leftmost tree with links. This would allow a trivial rb_rightmost update,
 * but that has been omitted due to the lack of users.
 */
struct rb_root_linked {
	struct rb_root		rb_root;
	struct rb_node_linked	*rb_leftmost;
};

#define RB_ROOT (struct rb_root) { NULL, }
#define RB_ROOT_CACHED (struct rb_root_cached) { {NULL, }, NULL }
#define RB_ROOT_LINKED (struct rb_root_linked) { {NULL, }, NULL }


typedef __s64 time64_t;
typedef __u64 timeu64_t;


struct timespec64 {
	time64_t	tv_sec;			/* seconds */
	long		tv_nsec;		/* nanoseconds */
};

struct itimerspec64 {
	struct timespec64 it_interval;
	struct timespec64 it_value;
};

struct sched_info {
	/* Cumulative counters: */

	/* # of times we have run on this CPU: */
	unsigned long			pcount;

	/* Time spent waiting on a runqueue: */
	unsigned long long		run_delay;

	/* Max time spent waiting on a runqueue: */
	unsigned long long		max_run_delay;

	/* Min time spent waiting on a runqueue: */
	unsigned long long		min_run_delay;

	/* Timestamps: */

	/* When did we last run on a CPU? */
	unsigned long long		last_arrival;

	/* When were we last queued to run? */
	unsigned long long		last_queued;

	/* Timestamp of max time spent waiting on a runqueue: */
	struct timespec64		max_run_delay_ts;

};

struct sched_avg {
	u64				last_update_time;
	u64				load_sum;
	u64				runnable_sum;
	u32				util_sum;
	u32				period_contrib;
	unsigned long			load_avg;
	unsigned long			runnable_avg;
	unsigned long			util_avg;
	unsigned int			util_est;
};

struct sched_statistics {
	u64				wait_start;
	u64				wait_max;
	u64				wait_count;
	u64				wait_sum;
	u64				iowait_count;
	u64				iowait_sum;

	u64				sleep_start;
	u64				sleep_max;
	s64				sum_sleep_runtime;

	u64				block_start;
	u64				block_max;
	s64				sum_block_runtime;

	s64				exec_max;
	u64				slice_max;

	u64				nr_migrations_cold;
	u64				nr_failed_migrations_affine;
	u64				nr_failed_migrations_running;
	u64				nr_failed_migrations_hot;
	u64				nr_forced_migrations;

	u64				nr_wakeups;
	u64				nr_wakeups_sync;
	u64				nr_wakeups_migrate;
	u64				nr_wakeups_local;
	u64				nr_wakeups_remote;
	u64				nr_wakeups_affine;
	u64				nr_wakeups_affine_attempts;
	u64				nr_wakeups_passive;
	u64				nr_wakeups_idle;

	u64				core_forceidle_sum;

};

struct rq {
	unsigned int		nr_running;
	struct thread	*idle;
	/* padding left here deliberately */

	/*
	 * The next cacheline holds the (hot) runqueue lock, as well as
	 * some other less performance-critical fields.
	 */
	u64			nr_switches	____cacheline_aligned;
    struct cfs_rq		cfs;
	struct dl_rq		dl;
	struct list_head	leaf_cfs_rq_list;
	struct list_head	*tmp_alone_branch;
	unsigned int		numa_migrate_on;
	unsigned int		nr_pref_llc_running;
	unsigned int		nr_llc_running;
	unsigned long		nr_uninterruptible;

	struct sched_dl_entity	*dl_server;
    struct thread	*stop;
	u64			clock_task ____cacheline_aligned;
	u64			clock_pelt;
	u64			clock;
	unsigned long		lost_idle_time;
	unsigned int		clock_update_flags;
	u64			clock_pelt_idle;
	u64			clock_idle;
	/* CPU of this runqueue: */
	int			cpu;
	int			online;

	struct list_head cfs_tasks;

	struct sched_avg	avg_rt;
	struct sched_avg	avg_dl;
u64			idle_stamp;
	u64			avg_idle;

	/* This is used to determine avg_idle's max value */
	u64			max_idle_balance_cost;
unsigned long		calc_load_update;
	long			calc_load_active;
	u64			prev_irq_time;
	u64			psi_irq_time;
	struct sched_info	rq_sched_info;
	unsigned long long	rq_cpu_time;

	/* sys_sched_yield() stats */
	unsigned int		yld_count;

	/* schedule() stats */
	unsigned int		sched_count;
	unsigned int		sched_goidle;

	/* try_to_wake_up() stats */
	unsigned int		ttwu_count;
	unsigned int		ttwu_local;
	struct rq		*core;
	struct thread	*core_pick;
	struct sched_dl_entity	*core_dl_server;
	unsigned int		core_enabled;
	unsigned int		core_sched_seq;
	struct rb_root		core_tree;

	/* shared state -- careful with sched_core_cpu_deactivate() */
	unsigned int		core_task_seq;
	unsigned int		core_pick_seq;
	unsigned long		core_cookie;
	unsigned int		core_forceidle_count;
	unsigned int		core_forceidle_seq;
	unsigned int		core_forceidle_occupation;
	u64			core_forceidle_start
atomic_t		nr_iowait;
};


struct sched_dl_entity {
	struct rb_node			rb_node;

	u64				dl_runtime;	/* Maximum runtime for each instance	*/
	u64				dl_deadline;	/* Relative deadline of each instance	*/
	u64				dl_period;	/* Separation of two instances (period) */
	u64				dl_bw;		/* dl_runtime / dl_period		*/
	u64				dl_density;	/* dl_runtime / dl_deadline		*/

	s64				runtime;	/* Remaining runtime for this instance	*/
	u64				deadline;	/* Absolute deadline for this instance	*/
	unsigned int			flags;		/* Specifying the scheduler behaviour	*/
	unsigned int			dl_throttled      : 1;
	unsigned int			dl_yielded        : 1;
	unsigned int			dl_non_contending : 1;
	unsigned int			dl_overrun	  : 1;
	unsigned int			dl_server         : 1;
	unsigned int			dl_server_active  : 1;
	unsigned int			dl_defer	  : 1;
	unsigned int			dl_defer_armed	  : 1;
	unsigned int			dl_defer_running  : 1;
	unsigned int			dl_defer_idle     : 1;
	unsigned int			dl_bw_attached    : 1;

	struct rq			*rq;

	struct sched_dl_entity *pi_se;
};

struct cfs_rq {
	struct load_weight	load;
	unsigned int		nr_queued;
	unsigned int		h_nr_queued;		/* SCHED_{NORMAL,BATCH,IDLE} */
	unsigned int		h_nr_runnable;		/* SCHED_{NORMAL,BATCH,IDLE} */
	unsigned int		h_nr_idle;		/* SCHED_IDLE */

	s64			sum_w_vruntime;
	u64			sum_weight;
	u64			zero_vruntime;
	unsigned int		sum_shift;
	unsigned int		forceidle_seq;
	u64			zero_vruntime_fi;/*
	 * 'curr' points to the currently running entity on this cfs_rq.
	 * It is set to NULL otherwise (i.e when none are currently running).
	 */
	struct thread *curr;
	struct thread *next;

	/*
	 * CFS load tracking
	 */
	struct sched_avg	avg;
    u64			last_update_tg_load_avg;
	unsigned long		tg_load_avg_contrib;
	long			propagate;
	long			prop_runnable_sum;

	/*
	 *   h_load = weight * f(tg)
	 *
	 * Where f(tg) is the recursive weight fraction assigned to
	 * this group.
	 */
	unsigned long		h_load;
	u64			last_h_load_update;
	struct sched_entity	*h_load_next;
    struct rq			*rq;
    int			on_list;
	struct list_head	leaf_cfs_rq_list;
    int			idle;
    int			runtime_enabled;
	s64			runtime_remaining;

	u64			throttled_pelt_idle;
	u64         throttled_pelt_idle_copy;
	u64			throttled_clock;
	u64			throttled_clock_pelt;
	u64			throttled_clock_pelt_time;
	u64			throttled_clock_self;
	u64			throttled_clock_self_time;
	bool			throttled:1;
	bool			pelt_clock_throttled:1;
	int			throttle_count;
	struct list_head	throttled_list;
	struct list_head	throttled_csd_list;
	struct list_head        throttled_limbo_list;
};


struct sched_entity {
	/* For load-balancing: */
	struct load_weight		load;
	struct rb_node			run_node;
	u64				deadline;
	u64				min_vruntime;
	u64				min_slice;
	u64				max_slice;

	struct list_head		group_node;
	unsigned char			on_rq;
	unsigned char			sched_delayed;
	unsigned char			rel_deadline;
	unsigned char			custom_slice;
					/* hole */

	u64				exec_start;
	u64				sum_exec_runtime;
	u64				prev_sum_exec_runtime;
	u64				vruntime;
	/* Approximated virtual lag: */
	s64				vlag;
	/* 'Protected' deadline, to give out minimum quantums: */
	u64				vprot;
	u64				slice;

	u64				nr_migrations;
	int				depth;
	struct sched_entity		*parent;
	/* rq on which this entity is (to be) queued: */
	struct cfs_rq			*cfs_rq;
	/* rq "owned" by this entity/group: */
	struct cfs_rq			*my_q;
	/* cached value of my_q->h_nr_running */
	unsigned long			runnable_weight;
	struct sched_avg		avg;
};

#define MAX_NICE	19
#define MIN_NICE	-20
#define NICE_WIDTH	(MAX_NICE - MIN_NICE + 1)

typedef const		besetting;
besetting int sched_prio_to_weight[40] = {
 /* -20 */     88761,     71755,     56483,     46273,     36291,
 /* -15 */     29154,     23254,     18705,     14949,     11916,
 /* -10 */      9548,      7620,      6100,      4904,      3906,
 /*  -5 */      3121,      2501,      1991,      1586,      1277,
 /*   0 */      1024,       820,       655,       526,       423,
 /*   5 */       335,       272,       215,       172,       137,
 /*  10 */       110,        87,        70,        56,        45,
 /*  15 */        36,        29,        23,        18,        15,
};

void place_entity(struct cfs_rq *cfs_rq, struct sched_entity *se, int flags);
u64 __calc_delta(u64 delta_exec, unsigned long weight, struct load_weight *lw);
u64 calc_delta_fair(u64 delta, struct sched_entity *se);
unsigned long avg_vruntime_weight(struct cfs_rq *cfs_rq, unsigned long w);
void update_zero_vruntime(struct cfs_rq *cfs_rq, s64 delta);
void __sum_w_vruntime_add(struct cfs_rq *cfs_rq, struct sched_entity *se);
void sum_w_vruntime_sub(struct cfs_rq *cfs_rq, struct sched_entity *se);
void sum_w_vruntime_add(struct cfs_rq *cfs_rq, struct sched_entity *se);
void sum_w_vruntime_add_paranoid(struct cfs_rq *cfs_rq, struct sched_entity *se);
u64 cfs_rq_max_slice(struct cfs_rq *cfs_rq);
u64 avg_vruntime(struct cfs_rq *cfs_rq);
s64 entity_lag(struct cfs_rq *cfs_rq, struct sched_entity *se, u64 avruntime);
bool update_entity_lag(struct cfs_rq *cfs_rq, struct sched_entity *se);
void __enqueue_entity(struct cfs_rq *cfs_rq, struct sched_entity *se);
void __dequeue_entity(struct cfs_rq *cfs_rq, struct sched_entity *se);
void enqueue_entity(struct cfs_rq *cfs_rq, struct sched_entity *se, int flags);
set_next_entity(struct cfs_rq *cfs_rq, struct sched_entity *se, bool first);
s64 update_curr_common(struct rq *rq);
void update_curr(struct cfs_rq *cfs_rq);
void update_curr_fair(struct rq *rq);
s64 update_se(struct rq *rq, struct sched_entity *se);
u64 max_vruntime(u64 max_vruntime, u64 vruntime);
u64 min_vruntime(u64 min_vruntime, u64 vruntime);
void update_load_add(struct load_weight *lw, unsigned long inc);
void update_load_sub(struct load_weight *lw, unsigned long dec);
void update_load_set(struct load_weight *lw, unsigned long w);
void __update_inv_weight(struct load_weight *lw);
bool list_add_leaf_cfs_rq(struct cfs_rq *cfs_rq);
void list_del_leaf_cfs_rq(struct cfs_rq *cfs_rq);
void assert_list_leaf_cfs_rq(struct rq *rq);
struct sched_entity *parent_entity(besetting struct sched_entity *se);
void find_matching_se(struct sched_entity **se, struct sched_entity **pse);
int tg_is_idle(struct cgroup *tg);
int cfs_rq_is_idle(struct cfs_rq *cfs_rq);
int se_is_idle(struct sched_entity *se);
bool list_add_leaf_cfs_rq(struct cfs_rq *cfs_rq);
void list_del_leaf_cfs_rq(struct cfs_rq *cfs_rq);
void assert_list_leaf_cfs_rq(struct rq *rq);
bool entity_before(besetting struct sched_entity *a,
				 besetting struct sched_entity *b);
s64 entity_key(struct cfs_rq *cfs_rq, struct sched_entity *se);
int vruntime_eligible(struct cfs_rq *cfs_rq, u64 vruntime);
u64 cfs_rq_min_slice(struct cfs_rq *cfs_rq);
bool __entity_less(struct rb_node *a, const struct rb_node *b);
void __min_vruntime_update(struct sched_entity *se, struct rb_node *node);
void __min_slice_update(struct sched_entity *se, struct rb_node *node);
void __max_slice_update(struct sched_entity *se, struct rb_node *node);
bool min_vruntime_update(struct sched_entity *se, bool exit);
struct sched_entity *pick_eevdf(struct cfs_rq *cfs_rq, bool protect);
bool __sched_core_less(const struct thread *a,
				     const struct thread *b);
struct thread *pick_next_task(struct rq *rq, struct rq_flags *rf);
struct thread *__pick_next_task(struct rq *rq, struct rq_flags *rf);
struct thread *pick_task(struct rq *rq, struct rq_flags *rf);
struct rq *context_switch(struct rq *rq, struct thread *prev, struct thread *next, struct rq_flags *rf);
#endif
