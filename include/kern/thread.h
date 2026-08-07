/* Copyright (C) Pedro Emanuel 2026
 * 
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU
 * General Public License as published by the Free Software Foundation; either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program; if not,
 * write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */
/* modified by Pedro Emanuel 2026 */
/*
 * HISTORY
 * 19-Aug-94  David Golub (dbg) at Carnegie-Mellon University
 *	Include <kern/pc_sample.h> instead of mach/pc_sample.h
 *	for sampling data structures.
 *
 * $Log:	thread.h,v $
 * Revision 2.19  93/08/10  15:12:01  mrt
 * 	Conditionalized atm hooks.
 * 	[93/07/30            cmaeda]
 * 	Included hook for network interface.
 * 	[93/06/09  15:45:03  jcb]
 * 
 * Revision 2.18  93/01/27  09:33:10  danner
 * 	Break latent include circularity by introduction of ipc_kmsg_queue.h. 
 * 
 * 
 * 
 * Revision 2.17  93/01/24  13:20:32  danner
 * 	Added sample_control to indicate that this threads is to
 * 	have its pc's sampled periodically.
 * 	[93/01/12            rvb]
 * 
 * Revision 2.16  93/01/21  14:04:00  danner
 * 	Typo correction.
 * 
 * Revision 2.15  93/01/19  09:01:07  danner
 * 	Correct arguments to natural_t.
 * 	[93/01/19            danner]
 * 
 * Revision 2.14  93/01/14  17:37:12  danner
 * 	Moved actual declaration of struct thread and thread_t to
 * 	kern/kern_types.h, to permit mutually recursive structure
 * 	definitions.  Added ANSI function prototypes.
 * 	[92/12/28            dbg]
 * 	Swapped some fields for better alignment.
 * 	[92/12/01            af]
 * 
 * Revision 2.13  91/08/28  11:14:53  jsb
 * 	Added ith_seqno.
 * 	[91/08/10            rpd]
 * 
 * Revision 2.12  91/07/31  17:49:44  dbg
 * 	Consolidated interruptible, swap_state, halted into state field.
 * 	Revised state machine.
 * 	[91/07/30  17:06:37  dbg]
 * 
 * Revision 2.11  91/05/18  14:34:28  rpd
 * 	Added depress_timer.
 * 	[91/03/31            rpd]
 * 
 * 	Replaced swap_privilege with stack_privilege.
 * 	[91/03/30            rpd]
 * 
 * Revision 2.10  91/05/14  16:48:57  mrt
 * 	Correcting copyright
 * 
 * Revision 2.9  91/03/16  14:52:58  rpd
 * 	Removed ith_saved.
 * 	[91/02/16            rpd]
 * 	Added save-state fields for page faults.
 * 	[91/02/05            rpd]
 * 	Added NCPUS to active_threads declaration.
 * 	Added active_stacks.
 * 	[91/01/28            rpd]
 * 	Added swap_privilege.
 * 	[91/01/18            rpd]
 * 
 * Revision 2.8  91/02/05  17:30:34  mrt
 * 	Changed to new Mach copyright
 * 	[91/02/01  16:19:58  mrt]
 * 
 * Revision 2.7  91/01/08  15:18:11  rpd
 * 	Added saved-state fields for exceptions.
 * 	[90/12/23            rpd]
 * 	Added swap_func.
 * 	[90/11/20            rpd]
 * 
 * Revision 2.6  90/08/27  22:04:23  dbg
 * 	Remove import of thread_modes.h (unneeded).
 * 	[90/07/17            dbg]
 * 
 * Revision 2.5  90/08/07  17:59:15  rpd
 * 	Put last_processor field under NCPUS > 1.
 * 	Removed tmp_address and tmp_object fields.
 * 	[90/08/07            rpd]
 * 
 * Revision 2.4  90/06/02  14:57:07  rpd
 * 	Converted to new IPC.
 * 	[90/03/26  22:25:34  rpd]
 * 
 * Revision 2.3  90/02/22  20:04:18  dbg
 * 	Add per-thread global VM variables (tmp_address, tmp_object).
 * 		[89/04/29	mwyoung]
 * 
 * Revision 2.2  89/09/08  11:26:59  dbg
 * 	Added simple_rpc_kmsg [rfr].  Set its size to size of
 * 	small Kmsg.  Moved all IPC data structures to end of
 * 	thread structure.
 * 	[89/08/16            dbg]
 * 
 * Revision 2.6  88/10/11  10:26:00  rpd
 * 	Added ipc_data to the thread structure.
 * 	[88/10/10  08:00:16  rpd]
 * 	
 * Revision 2.5  88/08/24  02:47:53  mwyoung
 * 	Adjusted include file references.
 * 	[88/08/17  02:24:57  mwyoung]
 * 
 *  1-Sep-88  David Black (dlb) at Carnegie-Mellon University
 *	Change all usage and delta fields to unsigned to prevent
 *	negative priorities.
 *
 * 11-Aug-88  David Black (dlb) at Carnegie-Mellon University
 *	Split exit_code field into ast and halted fields.
 *	Changed thread_should_halt() macro.
 *
 *  9-Aug-88  David Black (dlb) at Carnegie-Mellon University
 *	Replaced preempt_pri field with first_quantum.
 *
 * 19-Aug-88  David Golub (dbg) at Carnegie-Mellon University
 *	Changed 'struct pcb *' to 'pcb_t' to pry this file loose from
 *	old data structures.  Removed include of 'machine/pcb.h' - the
 *	structure definition should be moved to 'machine/thread.h'.
 *
 * Revision 2.4  88/08/06  19:22:05  rpd
 * Declare external variables as "extern".
 * Added macros ipc_thread_lock(), ipc_thread_unlock().
 * 
 * Revision 2.3  88/07/17  18:55:10  mwyoung
 * .
 * 
 * Revision 2.2.1.1  88/06/28  20:53:40  mwyoung
 * Reorganized.  Added thread_t->vm_privilege.
 * 
 * Added current_task() declaration.
 * 
 *
 *  6-Jul-88  David Golub (dbg) at Carnegie-Mellon University
 *	Remove compatibility data structures.  Add per-thread timeout
 *	element.
 *
 * 21-Jun-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Reorganized.  Added thread_t->vm_privilege.
 *
 *  4-May-88  David Golub (dbg) at Carnegie-Mellon University
 *	Remove vax-specific field (pcb physical address).
 *
 * 19-Apr-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Added current_task() declaration.
 *
 *  7-Apr-88  David Black (dlb) at Carnegie-Mellon University
 *	MACH_TIME_NEW is now standard.
 *
 *  4-Mar-88  David Black (dlb) at Carnegie-Mellon University
 *	Added usage_save and preempt_pri fields.
 *
 * 19-Feb-88  David Black (dlb) at Carnegie-Mellon University
 *	Deleted unused ticks field.  Rearranged and added MACH_TIME_NEW
 *	fields for scheduler interface.  user_ticks and system_ticks are
 *	not needed under MACH_TIME_NEW.  Change wait_time to sched_stamp.
 *
 * 21-Jan-88  David Golub (dbg) at Carnegie-Mellon University
 *	Replaced swappable boolean with swap_state field.  Swap states
 *	are defined in sys/thread_swap.h.
 *
 * 17-Jan-88  David Golub (dbg) at Carnegie-Mellon University
 *	Added declarations of new routines.
 *
 * 29-Dec-87  David Golub (dbg) at Carnegie-Mellon University
 *	Added declarations of thread_halt() and thread_halt_self().
 *
 * 21-Dec-87  David Golub (dbg) at Carnegie-Mellon University
 *	Added thread_should_halt macro.
 *
 *  9-Dec-87  David Golub (dbg) at Carnegie-Mellon University
 *	Added exit_code for thread termination and interrupt.
 *	Removed ipc_message_waiting and ipc_timer_set.
 *
 *  3-Dec-87  David Black (dlb) at Carnegie-Mellon University
 *	Added ipc_kernel field to indicate when message buffer is in
 *	kernel address space.  Added exception_clear_port to cache
 *	reply port for reuse in exc rpc.
 *
 *  2-Dec-87  David Black (dlb) at Carnegie-Mellon University
 *	Removed conditionals, purged history.
 */
/*
 *	File:	thread.h
 *	Author:	Avadis Tevanian, Jr.
 *
 *	This file contains the structure definitions for threads.
 *
 */


#ifndef	_KERN_THREAD_H_
#define _KERN_THREAD_H_

typedef u32		gid_t;
typedef u32		uid_t;

#include <asm-generic/int-ll64.h>
#include <linux/sched.h>
#include <mach/boolean.h>
#include <mach/thread_info.h>
#include <mach/thread_status.h>
#include <mach/machine/vm_types.h>
#include <mach/message.h>
#include <mach/port.h>
#include <mach/vm_prot.h>
#include <kern/ast.h>
#include <kern/cpu_number.h>
#include <kern/queue.h>
#include <kern/pc_sample.h>
#include <kern/processor.h>
#include <kern/sched_prim.h>	/* event_t, continuation_t */
#include <kern/time_out.h>
#include <kern/timer.h>
#include <kern/lock.h>
#include <kern/sched.h>
#include <kern/task.h>
#include <machine/thread.h>
#include <ipc/ipc_kmsg_queue.h>


struct cgroup_base_stat {
	u64 cputime;
	u64 forceidle_sum;
	u64 ntime;
};

struct cgroup {

	unsigned long flags;		/* "unsigned long" so bitops work */

	/*
	 * The depth this cgroup is at.  The root is at depth zero and each
	 * step down the hierarchy increments the level.  This along with
	 * ancestors[] can determine whether a given cgroup is a
	 * descendant of another without traversing the hierarchy.
	 */
	int level;

	/* Maximum allowed descent tree depth */
	int max_depth;

	/*
	 * Keep track of total numbers of visible and dying descent cgroups.
	 * Dying cgroups are cgroups which were deleted by a user,
	 * but are still existing because someone else is holding a reference.
	 * max_descendants is a maximum allowed number of descent cgroups.
	 *
	 * nr_descendants and nr_dying_descendants are protected
	 * by cgroup_mutex and css_set_lock. It's fine to read them holding
	 * any of cgroup_mutex and css_set_lock; for writing both locks
	 * should be held.
	 */
	int nr_descendants;
	int nr_dying_descendants;
	int max_descendants;

	/*
	 * Domain/threaded split of self.nr_populated_children: each counts
	 * immediate-child cgroups whose subtree is populated and sums to
	 * self.nr_populated_children. Kept as separate fields to allow readers
	 * like cgroup_can_be_thread_root() unlocked access. Protected by
	 * css_set_lock; updated by css_update_populated().
	 */
	int nr_populated_domain_children;
	int nr_populated_threaded_children;

	int nr_threaded_children;	/* # of live threaded child cgroups */

	/* sequence number for cgroup.kill, serialized by css_set_lock. */
	unsigned int kill_seq;
    vm_object_t procs_object;	
	vm_object_t events_object;	

	/* handles for "{cpu,memory,io,irq}.pressure" */
	vm_object_t psi_object;

	/*
	 * The bitmask of subsystems enabled on the child cgroups.
	 * ->subtree_control is the one configured through
	 * "cgroup.subtree_control" while ->subtree_ss_mask is the effective
	 * one which may have more subsystems enabled.  Controller knobs
	 * are made available iff it's enabled in ->subtree_control.
	 */
	u32 subtree_control;
	u32 subtree_ss_mask;
	u32 old_subtree_control;
	u32 old_subtree_ss_mask;


	/*
	 * Keep track of total number of dying CSSes at and below this cgroup.
	 * Protected by cgroup_mutex.
	 */
	int nr_dying_subsys;

	struct cgroup_root *root;

	/*
	 * List of cgrp_cset_links pointing at css_sets with tasks in this
	 * cgroup.  Protected by css_set_lock.
	 */
	struct list_head cset_links;

	/*
	 * On the default hierarchy, a css_set for a cgroup with some
	 * susbsys disabled will point to css's which are associated with
	 * the closest ancestor which has the subsys enabled.  The
	 * following lists all css_sets which point to this cgroup's css
	 * for the given subsystem.
	 */
	struct list_head e_csets[CGROUP_SUBSYS_COUNT];

	/*
	 * If !threaded, self.  If threaded, it points to the nearest
	 * domain ancestor.  Inside a threaded subtree, cgroups are exempt
	 * from process granularity and no-internal-task constraint.
	 * Domain level resource consumptions which aren't tied to a
	 * specific task are charged to the dom_cgrp.
	 */
	struct cgroup *dom_cgrp;
	struct cgroup *old_dom_cgrp;		/* used while enabling threaded */

	/*
	 * Depending on the context, this field is initialized via
	 * css_rstat_init() at different places:
	 *
	 * when cgroup is the root cgroup
	 *   performed in cgroup_setup_root()
	 * otherwise
	 *   performed in cgroup_create()
	 */


	/* cgroup basic resource statistics */
	struct cgroup_base_stat last_bstat;
	struct cgroup_base_stat bstat;

	/*
	 * list of pidlists, up to two for each namespace (one for procs, one
	 * for tasks); created on demand.
	 */
	struct list_head pidlists;



	/* All ancestors including self */
	union {
		struct {
			struct cgroup *_root_ancestor;
		};
	};
};
};

struct cgroup_root {
	/* The bitmask of subsystems attached to this hierarchy */
	unsigned int subsys_mask;

	/* Unique id for this hierarchy. */
	int hierarchy_id;

	/* A list running through the active hierarchies */
	struct list_head root_list;

	/* Number of cgroups in the hierarchy, used only for /proc/cgroups */
	atomic_t nr_cgrps;

	/* Hierarchy-specific flags */
	unsigned int flags;

	/* The path to use for release notifications. */
	char release_agent_path[PATH_MAX];

	/* The name for this hierarchy - may be empty */
	char name[MAX_CGROUP_ROOT_NAMELEN];

	/*
	 * The root cgroup. The containing cgroup_root will be destroyed on its
	 * release. This must be embedded last due to flexible array at the end
	 * of struct cgroup.
	 */
	struct cgroup cgrp;
};

struct thread {

    struct sched_entity         se;
    struct sched_dl_entity      dl;
    struct sched_statistics     stats;
    struct sched_info           sched_info;
    struct sched_avg            avg;
    struct sched_avg            avg_rt;
    struct sched_avg            avg_dl;
    
    /*
     * Prioridades CFS
     */
    int                         static_prio;    /* prioridade estática (nice value) */
    int                         normal_prio;    /* prioridade normal calculada */
    unsigned int                rt_priority;    /* prioridade RT (se aplicável) */
    unsigned int                policy;         /* política de escalonamento */
    
    /*
     * Estado do thread (Linux)
     */
    unsigned int                __state;        /* estado atual (TASK_*) */
    int                         exit_state;     /* estado de saída (EXIT_*) */
    unsigned int                flags;          /* flags de thread (PF_*) */
    
    /*
     * Carga e peso para o CFS
     */
    unsigned long               load_weight;    /* peso da carga */
    unsigned long               runnable_weight; /* peso runnable em cache */
    unsigned long               saved_load_weight; /* peso salvo para idle */
    
    /*
     * Informações de grupo de tarefas (CFS)
     */
    struct cgroup           *sched_task_group; /* grupo de escalonamento */
    struct sched_dl_entity      *dl_server;     /* servidor DL associado */
    
    /*
     * Flags de controle do escalonador
     */
    unsigned int                on_rq:1,        /* está na runqueue? */
                                sched_delayed:1, /* escalonamento atrasado? */
                                sched_remote_wakeup:1, /* wakeup remoto? */
                                in_iowait:1,    /* em espera de I/O? */
                                in_memstall:1,  /* em stall de memória? */
                                need_resched:1, /* precisa reescalonar? */
                                preempt_count:8, /* contador de preempção */
                                softirq_count:8, /* contador de softirq */
                                hardirq_count:8; /* contador de hardirq */
    
    /*
     * Informações de deadline (EEVDF)
     */
    u64                         deadline;       /* deadline absoluto */
    u64                         runtime;        /* runtime restante */
    u64                         period;         /* período de execução */
    u64                         slice;          /* slice de tempo atual */
    u64                         min_slice;      /* slice mínimo */
    u64                         max_slice;      /* slice máximo */
    
    /*
     * Contadores de tempo de execução (CFS)
     */
    u64                         exec_start;     /* tempo de início da execução */
    u64                         sum_exec_runtime; /* tempo total de execução */
    u64                         prev_sum_exec_runtime; /* tempo anterior */
    u64                         vruntime;       /* tempo virtual (CFS) */
    s64                         vlag;           /* lag virtual */
    u64                         vprot;          /* deadline protegido */
    
    /*
     * Informações de migração
     */
    unsigned int                nr_migrations;  /* número de migrações */
    unsigned int                nr_migrations_cold; /* migrações a frio */
    int                         depth;          /* profundidade na hierarquia */
    
    /*
     * Relacionamento na árvore de escalonamento
     */
    struct sched_entity         *parent;        /* entidade pai */
    struct cfs_rq               *cfs_rq;        /* CFS runqueue atual */
    struct cfs_rq               *my_q;          /* CFS runqueue própria */
    
    /* ================================================================
     * SEÇÃO 2: AFINIDADE E BALANCEAMENTO (SMP/NUMA)
     * ================================================================ */
    
#ifdef CONFIG_SMP
    /*
     * Afinidade de CPU
     */
    cpumask_t                   *thread_cpus_allowed; /* máscara de afinidade */
    cpumask_t                   *thread_cpus_allowed_ptr; /* ponteiro para máscara */
    unsigned int                nr_cpus_allowed; /* número de CPUs permitidas */
    
    /*
     * Migração e balanceamento
     */
    int                         migration_disabled; /* desabilitar migração */
    int                         migration_pending; /* migração pendente? */
    int                         migration_flags; /* flags de migração */
    unsigned int                migrate_from_cpu; /* CPU de origem */
    unsigned int                migrate_to_cpu; /* CPU de destino */
    
    /*
     * Informações NUMA
     */
    unsigned int                numa_migrate_on; /* migração NUMA ativa? */
    unsigned int                numa_preferred_nid; /* nó NUMA preferido */
    unsigned int                numa_scan_seq; /* sequência de scan NUMA */
    unsigned int                numa_migrate_retry; /* retentativas de migração */
    
    /*
     * Últimos processadores
     */
    processor_t                 last_processor; /* último processador usado */
    processor_t                 prev_processor; /* processador anterior */
    processor_t                 last_processor_idle; /* último idle */
#endif /* CONFIG_SMP */
    
    /* ================================================================
     * SEÇÃO 3: ESTATÍSTICAS E MONITORAMENTO
     * ================================================================ */
    
    /*
     * Estatísticas de CPU
     */
    unsigned long               nvcsw;          /* mudanças de contexto voluntárias */
    unsigned long               nivcsw;         /* mudanças de contexto involuntárias */
    unsigned long               last_switch_count; /* último contador de switches */
    u64                         last_switch_time; /* último tempo de switch */
    
    /*
     * Estatísticas de espera
     */
    unsigned long               wait_count;     /* contador de esperas */
    u64                         wait_time_start; /* início do tempo de espera */
    u64                         wait_time_max;  /* tempo máximo de espera */
    u64                         wait_time_total; /* tempo total de espera */
    
    /*
     * Estatísticas de I/O
     */
    unsigned long               io_wait_count;  /* contador de esperas I/O */
    u64                         io_wait_start;  /* início de espera I/O */
    u64                         io_wait_max;    /* tempo máximo I/O */
    u64                         io_wait_total;  /* tempo total I/O */
    
    /*
     * Estatísticas de bloqueio
     */
    unsigned long               block_count;    /* contador de bloqueios */
    u64                         block_start;    /* início de bloqueio */
    u64                         block_max;      /* tempo máximo de bloqueio */
    u64                         block_total;    /* tempo total de bloqueio */
    
    /*
     * Estatísticas de sleep
     */
    unsigned long               sleep_count;    /* contador de sleeps */
    u64                         sleep_start;    /* início de sleep */
    u64                         sleep_max;      /* tempo máximo de sleep */
    s64                         sum_sleep_runtime; /* tempo total de sleep */
    s64                         sum_block_runtime; /* tempo total de bloqueio */
    
    /*
     * Estatísticas de execução
     */
    u64                         exec_max;       /* tempo máximo de execução */
    u64                         slice_max;      /* slice máximo usado */
    u64                         core_forceidle_sum; /* tempo idle forçado */
    
    /*
     * Estatísticas de wakeup
     */
    unsigned long               nr_wakeups;     /* total de wakeups */
    unsigned long               nr_wakeups_sync; /* wakeups síncronos */
    unsigned long               nr_wakeups_migrate; /* wakeups com migração */
    unsigned long               nr_wakeups_local; /* wakeups locais */
    unsigned long               nr_wakeups_remote; /* wakeups remotos */
    unsigned long               nr_wakeups_affine; /* wakeups afins */
    unsigned long               nr_wakeups_affine_attempts; /* tentativas afins */
    unsigned long               nr_wakeups_passive; /* wakeups passivos */
    unsigned long               nr_wakeups_idle; /* wakeups de idle */
    
    /*
     * Estatísticas de migração
     */
    unsigned long               nr_failed_migrations_affine; /* migrações falhas (affine) */
    unsigned long               nr_failed_migrations_running; /* migrações falhas (running) */
    unsigned long               nr_failed_migrations_hot; /* migrações falhas (hot) */
    unsigned long               nr_forced_migrations; /* migrações forçadas */
    
    /* ================================================================
     * SEÇÃO 4: TASK - INFORMAÇÕES DA TAREFA (MACH)
     * ================================================================ */
    
    /*
     * Tarefa pai e relacionamentos
     */
    task_t                      task;           /* tarefa à qual pertence */
    struct thread               *task_thread;   /* thread principal da tarefa */
    queue_chain_t               thread_list;    /* lista de threads na tarefa */
    queue_chain_t               pset_threads;   /* lista de threads no processor set */
    
    /*
     * Thread ID e identificação
     */
    unsigned int                thread_id;      /* ID único da thread */
    unsigned int                pid;            /* ID do processo (compatibilidade) */
    unsigned int                tgid;           /* ID do grupo de threads */
    unsigned int                ppid;           /* ID do processo pai */
    
    /*
     * Nome e informações de usuário
     */
    char                        comm[32];       /* nome da thread (comando) */
    uid_t                       uid;            /* user ID */
    gid_t                       gid;            /* group ID */
    uid_t                       euid;           /* effective user ID */
    gid_t                       egid;           /* effective group ID */
    
    /*
     * Credenciais e capacidades
     */
    void                        *cred;          /* credenciais da thread */
    unsigned int                capabilities;   /* capacidades (capabilities) */
    
    /* ================================================================
     * SEÇÃO 5: AUTOPRESERVAÇÃO E REFERÊNCIAS
     * ================================================================ */
    
    /*
     * Lock e referências
     */
    decl_simple_lock_data(, lock)              /* lock da thread */
    int                         ref_count;      /* número de referências */
    int                         live_count;     /* contador de threads vivas */
    
    /*
     * Flags de ativação
     */
    boolean_t                   active;         /* thread está ativa? */
    boolean_t                   terminated;     /* thread terminou? */
    boolean_t                   exiting;        /* thread está saindo? */
    boolean_t                   aborting;       /* thread está abortando? */
    boolean_t                   interrupted;    /* thread foi interrompida? */
    
    /* ================================================================
     * SEÇÃO 6: HARDWARE E ESTADO DA MÁQUINA
     * ================================================================ */
    
    /*
     * PCB e estado da máquina
     */
    pcb_t                       pcb;            /* pcb hardware e estado */
    pcb_t                       *pcb_ptr;       /* ponteiro para PCB */
    vm_offset_t                 kernel_stack;   /* pilha do kernel */
    vm_offset_t                 kernel_stack_base; /* base da pilha do kernel */
    vm_offset_t                 kernel_stack_size; /* tamanho da pilha do kernel */
    vm_offset_t                 stack_privilege; /* pilha do kernel reservada */
    vm_offset_t                 user_stack;     /* pilha do usuário */
    
    /*
     * Estado da FPU/SSE/AVX
     */
    void                        *fpu_state;     /* estado da FPU */
    void                        *fpu_state_old; /* estado anterior da FPU */
    unsigned int                fpu_flags;      /* flags da FPU */
    unsigned int                fpu_counter;    /* contador de uso da FPU */
    
    /*
     * Estado do debug
     */
    unsigned int                debug_state;    /* estado de debug */
    void                        *debug_regs;    /* registradores de debug */
    unsigned int                watchpoint_count; /* número de watchpoints */
    
    /* ================================================================
     * SEÇÃO 7: SWAPPING E GERENCIAMENTO DE MEMÓRIA
     * ================================================================ */
    
    /*
     * Swapping
     */
    void                        (*swap_func)(void *); /* função de swap */
    void                        *swap_data;     /* dados para swap */
    vm_offset_t                 swap_address;   /* endereço de swap */
    unsigned int                swap_flags;     /* flags de swap */
    
    /*
     * VM e memória
     */
    vm_offset_t                 recover;        /* recuperação de falha de página */
    vm_offset_t                 recover_stack;  /* pilha para recuperação */
    boolean_t                   vm_privilege;   /* pode usar memória reservada? */
    unsigned int                vm_faults;      /* contador de faltas de página */
    unsigned int                vm_cow_faults;  /* faltas de copy-on-write */
    unsigned int                vm_zero_faults; /* faltas de página zero */
    unsigned int                vm_swap_faults; /* faltas de swap */
    
    /*
     * Reservas de memória
     */
    unsigned int                memory_pressure; /* pressão de memória atual */
    unsigned int                memory_high;    /* limite alto de memória */
    unsigned int                memory_low;     /* limite baixo de memória */
    unsigned int                memory_max;     /* limite máximo de memória */
    
    /*
     * Cgroups de memória
     */
    void                        *mem_cgroup;    /* cgroup de memória */
    void                        *mem_cgroup_old; /* cgroup anterior */
    
    /* ================================================================
     * SEÇÃO 8: BLOQUEIO E SINCRONIZAÇÃO
     * ================================================================ */
    
    /*
     * Eventos de espera
     */
    event_t                     wait_event;     /* evento em espera */
    event_t                     wake_event;     /* evento de wakeup */
    unsigned int                wait_flags;     /* flags de espera */
    unsigned int                wait_count;     /* contador de esperas */
    
    /*
     * Suspensão
     */
    int                         suspend_count;  /* contador de suspensão */
    int                         suspend_depth;  /* profundidade de suspensão */
    boolean_t                   suspend_pending; /* suspensão pendente? */
    boolean_t                   suspend_resume; /* resume de suspensão? */
    
    /*
     * Resultado de espera
     */
    kern_return_t               wait_result;    /* resultado da espera */
    unsigned int                wait_error;     /* erro da espera */
    boolean_t                   wake_active;    /* alguém esperando suspensão? */
    
    /*
     * Informações de bloqueio
     */
    void                        *lock_waiter;   /* bloqueio em espera */
    void                        *lock_holder;   /* dono do bloqueio */
    unsigned int                lock_depth;     /* profundidade do bloqueio */
    
    /* ================================================================
     * SEÇÃO 9: IPC - COMUNICAÇÃO ENTRE PROCESSOS (COMPLETO)
     * ================================================================ */
    
    /*
     * Lista encadeada de threads IPC
     */
    struct thread               *ith_next;
    struct thread               *ith_prev;
    
    /*
     * Estado da mensagem IPC
     */
    mach_msg_return_t           ith_state;      /* estado atual */
    mach_msg_return_t           ith_return;     /* valor de retorno */
    
    /*
     * Dados da mensagem
     */
    union {
        mach_msg_size_t         msize;          /* tamanho máximo para msg recebida */
        struct ipc_kmsg         *kmsg;          /* mensagem recebida */
        struct ipc_kmsg         *pending_kmsg;  /* mensagem pendente */
    } data;
    
    mach_port_seqno_t           ith_seqno;      /* sequência da mensagem */
    mach_port_seqno_t           ith_last_seqno; /* última sequência recebida */
    struct ipc_kmsg_queue       ith_messages;   /* fila de mensagens IPC */
    
    /*
     * Lock do IPC
     */
    decl_simple_lock_data(, ith_lock_data)
    decl_simple_lock_data(, ith_wait_lock)
    
    /*
     * Portas IPC
     */
    struct ipc_port             *ith_self;      /* porta própria (não é right) */
    struct ipc_port             *ith_sself;     /* send right para si mesmo */
    struct ipc_port             *ith_exception; /* send right para exceções */
    struct ipc_port             *ith_notify;    /* porta de notificação */
    
#if MACH_IPC_COMPAT
    struct ipc_port             *ith_reply;     /* send right para reply (compat) */
#endif
    
    /*
     * Portas de reply
     */
    mach_port_t                 ith_mig_reply;  /* porta reply para MIG */
    struct ipc_port             *ith_rpc_reply; /* porta reply para RPCs do kernel */
    struct ipc_port             *ith_kmsg_reply; /* porta reply para kmsg */
    
    /*
     * Fila de notificações
     */
    struct ipc_kmsg_queue       ith_notify_messages; /* fila de notificações IPC */
    unsigned int                ith_notify_count; /* contador de notificações */
    
    /*
     * Estado salvo quando a pilha é descartada (para IPC)
     */
    union {
        struct {
            mach_msg_header_t   *msg;           /* cabeçalho da mensagem */
            mach_msg_option_t   option;         /* opções da mensagem */
            mach_msg_size_t     rcv_size;       /* tamanho de recebimento */
            mach_msg_timeout_t  timeout;        /* timeout da mensagem */
            mach_port_t         notify;         /* porta de notificação */
            struct ipc_object   *object;        /* objeto IPC */
            struct ipc_mqueue   *mqueue;        /* mqueue IPC */
            struct ipc_kmsg     *kmsg;          /* kmsg associado */
        } receive;
        struct {
            struct ipc_port     *port;          /* porta de exceção */
            int                 exc;            /* código de exceção */
            int                 code;           /* código adicional */
            int                 subcode;        /* subcódigo */
        } exception;
        struct {
            struct ipc_port     *port;          /* porta de notificação */
            unsigned int        type;           /* tipo de notificação */
            unsigned int        value;          /* valor da notificação */
        } notify;
        void                    *other;         /* catch-all para outros estados */
    } saved;
    
    /*
     * Cache de mensagens IPC
     */
    struct ipc_kmsg             *kmsg_cache;    /* cache de kmsg */
    unsigned int                kmsg_cache_size; /* tamanho do cache */
    unsigned int                kmsg_cache_hits; /* hits no cache */
    unsigned int                kmsg_cache_misses; /* misses no cache */
    
    /* ================================================================
     * SEÇÃO 10: TIMERS E TEMPORIZADORES
     * ================================================================ */
    
    /*
     * Timers de usuário e sistema
     */
    timer_data_t                user_timer;     /* timer modo usuário */
    timer_data_t                system_timer;   /* timer modo sistema */
    timer_save_data_t           user_timer_save; /* timer usuário salvo */
    timer_save_data_t           system_timer_save; /* timer sistema salvo */
    
    /*
     * Timeouts para IPC e operações bloqueantes
     */
    timer_elt_data_t            timer;          /* timer para timeouts */
    timer_elt_data_t            wait_timer;     /* timer para esperas */
    timer_elt_data_t            sleep_timer;    /* timer para sleeps */
    
    /*
     * Timers de I/O
     */
    timer_elt_data_t            io_timer;       /* timer para I/O */
    timer_elt_data_t            block_timer;    /* timer para bloqueios */
    
    /*
     * Timers de escalonamento (CFS)
     */
    timer_elt_data_t            sched_timer;    /* timer do escalonador */
    timer_elt_data_t            preempt_timer;  /* timer de preempção */
    timer_elt_data_t            balance_timer;  /* timer de balanceamento */
    
    /*
     * Contadores de tempo
     */
    unsigned int                cpu_delta;      /* uso CPU desde última atualização */
    unsigned int                sched_delta;    /* uso ponderado desde atualização */
    unsigned int                system_time;    /* tempo em modo sistema */
    unsigned int                user_time;      /* tempo em modo usuário */
    unsigned int                total_time;     /* tempo total de CPU */
    
    /*
     * Tempos de espera
     */
    unsigned int                wait_time;      /* tempo total em espera */
    unsigned int                block_time;     /* tempo total em bloqueio */
    unsigned int                sleep_time;     /* tempo total em sleep */
    
    /* ================================================================
     * SEÇÃO 11: AST (ASYNCHRONOUS SYSTEM TRAPS) E CONTROLE
     * ================================================================ */
    
    /*
     * ASTs pendentes
     */
    int                         ast;            /* ASTs pendentes (ver ast.h) */
    int                         ast_pending;    /* ASTs em processamento */
    unsigned int                ast_mask;       /* máscara de ASTs */
    
    /*
     * Controle de threads
     */
    int                         user_stop_count; /* stops solicitados pelo usuário */
    int                         kernel_stop_count; /* stops solicitados pelo kernel */
    boolean_t                   stop_pending;   /* stop pendente? */
    boolean_t                   stop_suspend;   /* suspender no stop? */
    
    /*
     * Sinais e notificações
     */
    unsigned int                signal_pending; /* sinais pendentes */
    unsigned int                signal_mask;    /* máscara de sinais */
    void                        *signal_info;   /* informações de sinal */
    
    /* ================================================================
     * SEÇÃO 12: PROCESSADOR E PROCESSOR SET
     * ================================================================ */
    
    /*
     * Processor set atribuído
     */
    processor_set_t             processor_set;  /* processor set atribuído */
    processor_set_t             old_processor_set; /* processor set anterior */
    
    /*
     * Processador vinculado
     */
    processor_t                 bound_processor; /* processador vinculado */
    processor_t                 preferred_processor; /* processador preferido */
    
    /*
     * Informações de processador
     */
    unsigned int                cpu_affinity;   /* afinidade de CPU */
    unsigned int                cpu_migrate;    /* migração de CPU */
    unsigned int                cpu_usage;      /* uso de CPU da thread */
    
    /*
     * Tempo de CPU
     */
    unsigned int                cpu_time;       /* tempo de CPU usado */
    unsigned int                cpu_time_max;   /* tempo máximo de CPU */
    unsigned int                cpu_time_limit; /* limite de tempo de CPU */
    
    /* ================================================================
     * SEÇÃO 13: AMOSTRAGEM E DEBUG
     * ================================================================ */
    
    /*
     * Controle de amostragem
     */
    sample_control_t            pc_sample;      /* controle de amostragem */
    sample_control_t            pc_sample_prev; /* amostragem anterior */
    
    /*
     * Debug e trace
     */
    unsigned int                debug_flags;    /* flags de debug */
    void                        *debug_info;    /* informações de debug */
    unsigned int                trace_count;    /* contador de traces */
    unsigned int                trace_entries;  /* entradas de trace */
    
    /*
     * Profiling
     */
    unsigned int                profile_count;  /* contador de profiling */
    unsigned int                profile_hits;   /* hits de profiling */
    void                        *profile_data;  /* dados de profiling */
    
    /* ================================================================
     * SEÇÃO 14: OPTIONAIS E ESPECÍFICOS DA PLATAFORMA
     * ================================================================ */
    
#if MACH_HOST
    /*
     * Atribuição de host
     */
    boolean_t                   may_assign;     /* pode mudar de atribuição? */
    boolean_t                   assign_active;  /* alguém esperando may_assign? */
    boolean_t                   assign_pending; /* atribuição pendente? */
    unsigned int                assign_count;   /* contador de atribuições */
#endif
    
#if NET_ATM
    /*
     * Suporte ATM
     */
    nw_ep_owned_t               nw_ep_waited;   /* entidade de rede ATM */
    nw_ep_owned_t               nw_ep_pending;  /* entidade ATM pendente */
    unsigned int                nw_ep_count;    /* contador de entidades ATM */
#endif
    
#ifdef CONFIG_ARCH_HAS_CPU
    /*
     * Específico da arquitetura
     */
    void                        *arch_data;     /* dados específicos da arquitetura */
    unsigned int                arch_flags;     /* flags da arquitetura */
    unsigned int                arch_state;     /* estado da arquitetura */
#endif
    
#ifdef CONFIG_VIRTUALIZATION
    /*
     * Virtualização
     */
    void                        *vm_data;       /* dados de virtualização */
    unsigned int                vm_flags;       /* flags de virtualização */
    unsigned int                vm_state;       /* estado de virtualização */
#endif

};


#ifndef	_KERN_KERN_TYPES_H_
typedef struct thread *thread_t;

#define THREAD_NULL	((thread_t) 0)

typedef	mach_port_t *thread_array_t;
#endif	/* _KERN_KERN_TYPES_H_ */


extern thread_t		active_threads[NCPUS];	/* active threads */
extern vm_offset_t	active_stacks[NCPUS];	/* active kernel stacks */

/*
 *	User routines
 */

 kern_return_t	thread_create(
	task_t		parent_task,
	thread_t	*child_thread);
 kern_return_t	thread_terminate(
	thread_t	thread);
 kern_return_t	thread_suspend(
	thread_t	thread);
 kern_return_t	thread_resume(
	thread_t	thread);
 kern_return_t	thread_abort(
	thread_t	thread);
 kern_return_t	thread_get_state(
	thread_t	thread,
	int		flavor,
	thread_state_t	old_state,
	natural_t	*old_state_count);
 kern_return_t	thread_set_state(
	thread_t	thread,
	int		flavor,
	thread_state_t	new_state,
	natural_t	new_state_count);
 kern_return_t	thread_get_special_port(
	thread_t	thread,
	int		which,
	struct ipc_port	**portp);
 kern_return_t	thread_set_special_port(
	thread_t	thread,
	int		which,
	struct ipc_port	*port);
 kern_return_t	thread_info(
	thread_t	thread,
	int		flavor,
	thread_info_t	thread_info_out,
	natural_t	*thread_info_count);
 kern_return_t	thread_assign(
	thread_t	thread,
	processor_set_t	new_pset);
 kern_return_t	thread_assign_default(
	thread_t	thread);
 void		thread_init(void);
 void		thread_reference(thread_t);
 void		thread_deallocate(thread_t);
 void		thread_hold(thread_t);
 kern_return_t	thread_dowait(
	thread_t	thread,
	boolean_t	must_halt);
 void		thread_release(thread_t);
 kern_return_t	thread_halt(
	thread_t	thread,
	boolean_t	must_halt);
 void		thread_halt_self(void);
 void		thread_force_terminate(thread_t);
 void		thread_set_own_priority(
	int		priority);
 thread_t		kernel_thread(
	task_t		task,
	void		(*start)(void),
	void *		arg);
 void		thread_freeze(
	thread_t	thread);
 void		thread_doassign(
	thread_t	thread,
	processor_set_t	new_pset,
	boolean_t	release_freeze);
 void		thread_unfreeze(
	thread_t	thread);

 kern_return_t	cgroup_create(
	task_t		parent_task,
	struct cgroup	*child_cgroup);
 kern_return_t	struct cgrouperminate(
	struct cgroup	cgroup);
 kern_return_t	cgroup_suspend(
	struct cgroup	cgroup);
 kern_return_t	cgroup_resume(
	struct cgroup	cgroup);
 kern_return_t	cgroup_abort(
	struct cgroup	cgroup);
 kern_return_t	cgroup_get_state(
	struct cgroup	cgroup,
	int		flavor,
	cgroup_state_t	old_state,
	natural_t	*old_state_count);
 kern_return_t	cgroup_set_state(
	struct cgroup	cgroup,
	int		flavor,
	cgroup_state_t	new_state,
	natural_t	new_state_count);
 kern_return_t	cgroup_get_special_port(
	struct cgroup	cgroup,
	int		which,
	struct ipc_port	**portp);
 kern_return_t	cgroup_set_special_port(
	struct cgroup	cgroup,
	int		which,
	struct ipc_port	*port);
 kern_return_t	cgroup_info(
	struct cgroup	cgroup,
	int		flavor,
	cgroup_info_t	cgroup_info_out,
	natural_t	*cgroup_info_count);
 kern_return_t	cgroup_assign(
	struct cgroup	cgroup,
	processor_set_t	new_pset);
 kern_return_t	cgroup_assign_default(
	struct cgroup	cgroup);
 void		cgroup_init(void);
 void		cgroup_reference(struct cgroup);
 void		cgroup_deallocate(struct cgroup);
 void		cgroup_hold(struct cgroup);
 kern_return_t	cgroup_dowait(
	struct cgroup	cgroup,
	boolean_t	must_halt);
 void		cgroup_release(struct cgroup);
 kern_return_t	cgroup_halt(
	struct cgroup	cgroup,
	boolean_t	must_halt);
 void		cgroup_halt_self(void);
 void		cgroup_force_terminate(struct cgroup);
 void		cgroup_set_own_priority(
	int		priority);
 struct cgroup		kernel_cgroup(
	task_t		task,
	void		(*start)(void),
	void *		arg);
 void		cgroup_freeze(
	struct cgroup	cgroup);
 void		cgroup_doassign(
	struct cgroup	cgroup,
	processor_set_t	new_pset,
	boolean_t	release_freeze);
 void		cgroup_unfreeze(
	struct cgroup	cgroup);


/*
 *	Macro-defined routines
 */

#define thread_pcb(th)		((th)->pcb)

#define thread_lock(th)		simple_lock(&(th)->lock)
#define thread_unlock(th)	simple_unlock(&(th)->lock)

#define thread_should_halt(thread)	\
		((thread)->ast & (AST_HALT|AST_TERMINATE))

/*
 *	Machine specific implementations of the current thread macro
 *	designate this by defining CURRENT_THREAD.
 */
#ifndef	CURRENT_THREAD
#define current_thread()	(active_threads[cpu_number()])
#endif	/* CURRENT_THREAD */

#define	current_stack()		(active_stacks[cpu_number()])

#define	current_task()		(current_thread()->task)
#define	current_space()		(current_task()->itk_space)
#define	current_map()		(current_task()->map)

#endif	/* _KERN_THREAD_H_ */
