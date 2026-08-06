/* SPDX-License-Identifier: GPL-2.0 */

/* modified for Pedro Emanuel in 2026 */
#ifndef LINUX_SCHED_H
#define LINUX_SCHED_H

#include "asm-generic/int-ll64.h"

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

#endif
