/* 
 * Mach Operating System
 * Copyright (c) 1994-1988 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */
/*
 * HISTORY
 * 19-Aug-94  David Golub (dbg) at Carnegie-Mellon University
 *	Include <kern/pc_sample.h> instead of mach/pc_sample.h
 *	for sampling data structures.
 *
 * $Log:	task.h,v $
 * Revision 2.12  93/08/10  15:12:22  mrt
 * 	Conditionalized atm hooks.
 * 	[93/07/30            cmaeda]
 * 	Included hooks for network interface.
 * 	[93/06/09  15:43:43  jcb]
 * 
 * Revision 2.11  93/01/24  13:20:05  danner
 * 	Added sample_control to indicate that all the threads of the task are
 * 	to have their pc's sampled periodically.
 * 	[93/01/12            rvb]
 * 
 * Revision 2.10  93/01/21  12:22:26  danner
 * 	Expanded to allow TASK_FAST_TAS_NRAS ras.
 * 	[93/01/19  16:33:32  bershad]
 * 
 * Revision 2.9  93/01/19  09:01:02  danner
 * 	Corrected argument types.
 * 	[93/01/19            danner]
 * 
 * Revision 2.8  93/01/14  17:36:49  danner
 * 	Moved actual declaration of struct task and task_t to
 * 	kern/kern_types.h, to permit mutually recursive structure
 * 	definitions.  Added ANSI function prototypes.
 * 	[92/12/28            dbg]
 * 	Swapped fields for better alignment.
 * 	[92/12/01            af]
 * 
 * Revision 2.7  92/07/20  13:33:02  cmaeda
 * 	Added fast tas addresses to task structure.
 * 	[92/05/11  14:36:52  cmaeda]
 * 
 * Revision 2.6  91/11/15  14:11:16  rpd
 * 	NORMA_TASK: added child_node field, which determines where child tasks
 * 	are created for the given task. A value of -1 means local node.
 * 	[91/09/23  09:19:35  jsb]
 * 
 * Revision 2.5  91/05/14  16:48:20  mrt
 * 	Correcting copyright
 * 
 * Revision 2.4  91/02/05  17:30:06  mrt
 * 	Changed to new Mach copyright
 * 	[91/02/01  16:19:18  mrt]
 * 
 * Revision 2.3  90/06/02  14:56:48  rpd
 * 	Removed kernel_vm_space, keep_wired.  They are no longer needed.
 * 	[90/04/29            rpd]
 * 	Converted to new IPC.
 * 	[90/03/26  22:23:06  rpd]
 * 
 * Revision 2.2  89/09/08  11:26:47  dbg
 * 	Add 'keep_wired' privilege field, to allow out-of-line data
 * 	passed to task to remain wired.  Needed for default pager.
 * 	Remove kernel_vm_space (not used).
 * 	[89/07/17            dbg]
 * 
 * 19-Oct-88  David Golub (dbg) at Carnegie-Mellon University
 *	Removed all non-MACH data structures.
 *
 * Revision 2.6  88/09/25  22:16:41  rpd
 * 	Changed port_cache fields/definitions to obj_cache.
 * 	[88/09/24  18:13:13  rpd]
 * 
 * Revision 2.5  88/08/24  02:46:30  mwyoung
 * 	Adjusted include file references.
 * 	[88/08/17  02:24:13  mwyoung]
 * 
 * Revision 2.4  88/07/20  21:07:49  rpd
 * Added ipc_task_lock/ipc_task_unlock definitions.
 * Changes for port sets.
 * Add ipc_next_name field, used for assigning local port names.
 * 
 * Revision 2.3  88/07/17  18:56:33  mwyoung
 * .
 * 
 * Revision 2.2.2.1  88/06/28  20:02:03  mwyoung
 * Cleaned up.  Replaced task_t->kernel_only with
 * task_t->kernel_ipc_space, task_t->kernel_vm_space, and
 * task_t->ipc_privilege, to prevent overloading errors.
 * 
 * Remove current_task() declaration.
 * Eliminate paging_task.
 * 
 * Revision 2.2.1.2  88/06/26  00:45:49  rpd
 * Changes for port sets.
 * 
 * Revision 2.2.1.1  88/06/23  23:32:38  rpd
 * Add ipc_next_name field, used for assigning local port names.
 * 
 * 21-Jun-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Cleaned up.  Replaced task_t->kernel_only with
 *	task_t->kernel_ipc_space, task_t->kernel_vm_space, and
 *	task_t->ipc_privilege, to prevent overloading errors.
 *
 * 19-Apr-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Remove current_task() declaration.
 *	Eliminate paging_task.
 *
 * 18-Jan-88  David Golub (dbg) at Carnegie-Mellon University
 *	Removed task_data (now is per_thread).  Added
 *	task_bootstrap_port.  Added new routine declarations.
 *	Removed wake_active (unused).  Added fields to accumulate
 *	user and system time for terminated threads.
 *
 *  19-Feb-88 Douglas Orr (dorr) at Carnegie-Mellon University
 *	Change emulation bit mask into vector of routine  addrs
 *
 *  27-Jan-87 Douglas Orr (dorr) at Carnegie-Mellon University
 *	Add support for user space syscall emulation (bit mask
 *	of enabled user space syscalls and user space emulation
 *	routine).
 *
 *  3-Dec-87  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Change port cache account for per-task port names.
 *	Should move IPC stuff to a separate file :-).
 *	Add reply port for use by kernel-internal tasks.
 *
 *  2-Dec-87  David Black (dlb) at Carnegie-Mellon University
 *	Added active field.
 *
 * 18-Nov-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Eliminate conditionals, flush history.
 */
/*
 *	File:	task.h
 *	Author:	Avadis Tevanian, Jr.
 *
 *	This file contains the structure definitions for tasks.
 *
 */

#ifndef	_KERN_TASK_H_
#define _KERN_TASK_H_

#include <norma_task.h>
#include <fast_tas.h>
#include <net_atm.h>

#include <mach/boolean.h>
#include <mach/port.h>
#include <mach/time_value.h>
#include <mach/mach_param.h>
#include <mach/task_info.h>
#include <kern/thread.h>
#include <kern/kern_types.h>
#include <kern/lock.h>
#include <kern/queue.h>
#include <kern/pc_sample.h>
#include <kern/processor.h>
#include <kern/syscall_emulation.h>
#include <vm/vm_map.h>

#if	NET_ATM
typedef struct nw_ep_owned {
  unsigned int ep;
  struct nw_ep_owned *next;
} nw_ep_owned_s, *nw_ep_owned_t;
#endif


struct task {
    /* ============================================================
     * SEÇÃO 1: SINCRONIZAÇÃO E DESTRUIÇÃO
     * ============================================================ */
    
    decl_simple_lock_data(, lock)           /* Lock principal da task */
    decl_simple_lock_data(, lock_thread)    /* Lock da lista de threads */
    decl_simple_lock_data(, lock_cgroup)    /* Lock dos cgroups */
    decl_simple_lock_data(, lock_stat)      /* Lock das estatísticas */
    
    int                         ref_count;      /* Número de referências */
    int                         live_count;     /* Número de threads vivas */
    int                         zombie_count;   /* Número de threads zombies */
    int                         exit_count;     /* Número de threads em saída */
    
    boolean_t                   active;         /* Task não foi terminada */
    boolean_t                   terminated;     /* Task foi terminada */
    boolean_t                   exiting;        /* Task está em processo de saída */
    boolean_t                   aborting;       /* Task está abortando */
    boolean_t                   frozen;         /* Task está congelada */
    boolean_t                   killed;         /* Task foi morta */
    
    int                         exit_code;      /* Código de saída da task */
    int                         exit_signal;    /* Sinal de saída da task */

    /* ============================================================
     * SEÇÃO 2: THREADS - A Task contém threads, mas NÃO as escalona
     * ============================================================ */
    
    queue_head_t                thread_list;    /* Lista de todas as threads */
    queue_head_t                thread_active;  /* Lista de threads ativas */
    queue_head_t                thread_zombie;  /* Lista de threads zombies */
    queue_head_t                thread_dead;    /* Lista de threads mortas */
    
    int                         thread_count;   /* Número total de threads */
    int                         thread_active_count; /* Número de threads ativas */
    int                         thread_running_count; /* Número de threads executando */
    int                         thread_waiting_count; /* Número de threads em espera */
    int                         thread_sleeping_count; /* Número de threads em sleep */
    int                         thread_idle_count; /* Número de threads idle */
    int                         thread_stopped_count; /* Número de threads paradas */
    
    struct thread               *main_thread;   /* Thread principal da task */
    struct thread               *first_thread;  /* Primeira thread criada */
    struct thread               *last_thread;   /* Última thread criada */

    /* ============================================================
     * SEÇÃO 3: VM - ESPAÇO DE ENDEREÇAMENTO
     * ============================================================ */
    
    vm_map_t                    map;            /* Espaço de endereçamento principal */
    vm_map_t                    kernel_map;     /* Mapa do kernel (se aplicável) */
    vm_map_t                    shared_map;     /* Mapa compartilhado */
    
    vm_size_t                   mem_limit;      /* Limite total de memória */
    vm_size_t                   mem_reserved;   /* Memória reservada */
    vm_size_t                   mem_high;       /* Limite alto (alerta) */
    vm_size_t                   mem_low;        /* Limite baixo */
    vm_size_t                   mem_max;        /* Limite máximo absoluto */
    vm_size_t                   mem_swap_limit; /* Limite de swap */
    
    vm_size_t                   mem_rss;        /* Resident Set Size (RSS) */
    vm_size_t                   mem_cache;      /* Memória em cache */
    vm_size_t                   mem_swap;       /* Memória em swap */
    vm_size_t                   mem_allocated;  /* Memória total alocada */
    vm_size_t                   mem_mapped;     /* Memória mapeada */
    vm_size_t                   mem_shared;     /* Memória compartilhada */
    vm_size_t                   mem_private;    /* Memória privada */
    vm_size_t                   mem_stack;      /* Tamanho da pilha */
    vm_size_t                   mem_heap;       /* Tamanho do heap */
    vm_size_t                   mem_code;       /* Tamanho do código */
    vm_size_t                   mem_data;       /* Tamanho dos dados */
    vm_size_t                   mem_peak;       /* Pico de uso de memória */
    
    unsigned int                vm_faults;      /* Total de faltas de página */
    unsigned int                vm_major_faults; /* Faltas maiores (disk I/O) */
    unsigned int                vm_minor_faults; /* Faltas menores (em memória) */
    unsigned int                vm_cow_faults;  /* Faltas de copy-on-write */
    unsigned int                vm_zero_faults; /* Faltas de página zero */
    unsigned int                vm_swap_faults; /* Faltas de página de swap */
    unsigned int                vm_alloc_count; /* Número de alocações */
    unsigned int                vm_free_count;  /* Número de liberações */
    unsigned int                vm_swap_ins;    /* Páginas lidas do swap */
    unsigned int                vm_swap_outs;   /* Páginas escritas no swap */

    /* ============================================================
     * SEÇÃO 4: PROCESSADOR E PROCESSOR SET
     * ============================================================ */
    
    processor_set_t             processor_set;  /* Processor set para novas threads */
    processor_set_t             old_processor_set; /* Processor set anterior */
    
    boolean_t                   may_assign;     /* Pode mudar o pset atribuído? */
    boolean_t                   assign_active;  /* Esperando por may_assign */
    boolean_t                   assign_pending; /* Atribuição pendente */
    unsigned int                assign_count;   /* Contador de atribuições */
    
    cpumask_t                   *cpu_allowed;   /* Máscara de CPUs permitidas */
    cpumask_t                   *cpu_default;   /* Máscara de CPUs padrão */
    cpumask_t                   *cpu_reserved;  /* Máscara de CPUs reservadas */
    unsigned int                cpu_mask_updated; /* Máscara foi atualizada? */

    /* ============================================================
     * SEÇÃO 5: SUSPENSÃO E CONTROLE
     * ============================================================ */
    
    int                         suspend_count;  /* Contador de suspensão */
    int                         suspend_depth;  /* Profundidade de suspensão */
    boolean_t                   suspend_pending; /* Suspensão pendente */
    boolean_t                   suspend_resume;  /* Resume de suspensão */
    
    int                         user_stop_count; /* Stops pendentes do usuário */
    int                         user_suspend_count; /* Suspensões do usuário */
    int                         user_resume_count; /* Resumes do usuário */
    
    unsigned int                signal_pending; /* Sinais pendentes */
    unsigned int                signal_mask;    /* Máscara de sinais */
    void                        *signal_handlers; /* Handlers de sinais */
    void                        *signal_info;   /* Informações de sinal */

    /* ============================================================
     * SEÇÃO 6: IPC - COMUNICAÇÃO ENTRE PROCESSOS
     * ============================================================ */
    
    decl_simple_lock_data(, itk_lock_data)
    decl_simple_lock_data(, itk_lock_space)
    decl_simple_lock_data(, itk_lock_notify)
    
    struct ipc_port             *itk_self;      /* Porta própria (não é right) */
    struct ipc_port             *itk_sself;     /* Send right para si mesmo */
    struct ipc_port             *itk_exception; /* Send right para exceções */
    struct ipc_port             *itk_bootstrap; /* Send right para bootstrap */
    struct ipc_port             *itk_notify;    /* Porta de notificação */
    struct ipc_port             *itk_reply;     /* Porta de reply */
    struct ipc_port             *itk_death;     /* Porta de morte */
    struct ipc_port             *itk_debug;     /* Porta de debug */
    
    struct ipc_port             *itk_registered[TASK_PORT_REGISTER_MAX];
    unsigned int                itk_registered_count;
    unsigned int                itk_registered_max;
    
    struct ipc_space            *itk_space;     /* Espaço de nomes IPC */
    struct ipc_space            *itk_space_old; /* Espaço IPC anterior */
    struct ipc_space            *itk_space_backup; /* Backup do espaço IPC */
    
    struct ipc_kmsg_queue       itk_messages;   /* Fila de mensagens IPC */
    struct ipc_kmsg_queue       itk_notify_messages; /* Fila de notificações */
    struct ipc_kmsg_queue       itk_exception_messages; /* Fila de exceções */
    
    unsigned int                itk_msg_sent;   /* Mensagens enviadas */
    unsigned int                itk_msg_recv;   /* Mensagens recebidas */
    unsigned int                itk_msg_send_size; /* Tamanho total enviado */
    unsigned int                itk_msg_recv_size; /* Tamanho total recebido */
    unsigned int                itk_msg_send_fail; /* Falhas de envio */
    unsigned int                itk_msg_recv_fail; /* Falhas de recebimento */
    unsigned int                itk_msg_timeout; /* Timeouts de mensagens */
    unsigned int                itk_port_alloc; /* Portas alocadas */
    unsigned int                itk_port_free;  /* Portas liberadas */

    /* ============================================================
     * SEÇÃO 7: CGROUPS - CONTROLE DE RECURSOS
     * ============================================================ */
    
    struct cgroup               **cgroups;       /* Array de todos os cgroups */
    struct cgroup_subsys_state  *cgroup_cpu;     /* Cgroup de CPU */
    struct cgroup_subsys_state  *cgroup_memory;  /* Cgroup de memória */
    struct cgroup_subsys_state  *cgroup_io;      /* Cgroup de I/O */
    struct cgroup_subsys_state  *cgroup_pids;    /* Cgroup de PIDs */
    struct cgroup_subsys_state  *cgroup_cpuacct; /* Cgroup de CPU accounting */
    struct cgroup_subsys_state  *cgroup_hugetlb; /* Cgroup de HugeTLB */
    struct cgroup_subsys_state  *cgroup_sched;   /* Cgroup de escalonamento */
    struct cgroup_subsys_state  *cgroup_rdma;    /* Cgroup de RDMA */
    struct cgroup_subsys_state  *cgroup_misc;    /* Cgroup miscelâneo */
    
    unsigned int                cgroup_control;  /* Flags de controle */
    unsigned int                cgroup_count;    /* Número de cgroups ativos */
    char                        *cgroup_path;    /* Caminho do cgroup */
    char                        *cgroup_name;    /* Nome do cgroup */
    
    u64                         cgroup_cpu_limit;   /* Limite de CPU */
    u64                         cgroup_mem_limit;   /* Limite de memória */
    u64                         cgroup_io_limit;    /* Limite de I/O */
    u64                         cgroup_pid_limit;   /* Limite de PIDs */
    
    u64                         cgroup_cpu_usage;   /* Uso de CPU do cgroup */
    u64                         cgroup_mem_usage;   /* Uso de memória do cgroup */
    u64                         cgroup_io_usage;    /* Uso de I/O do cgroup */
    u64                         cgroup_pid_usage;   /* Uso de PIDs do cgroup */
    
    u64                         cpu_quota_us;    /* Quota de CPU (microssegundos) */
    u64                         cpu_period_us;   /* Período de CPU (microssegundos) */
    u64                         cpu_burst_us;    /* Burst de CPU (microssegundos) */
    u64                         cpu_rt_runtime;  /* Runtime RT da task */
    u64                         cpu_rt_period;   /* Período RT da task */

    /* ============================================================
     * SEÇÃO 8: ESTATÍSTICAS
     * ============================================================ */
    
    time_value_t                total_user_time;    /* Tempo em modo usuário */
    time_value_t                total_system_time;  /* Tempo em modo sistema */
    time_value_t                total_real_time;    /* Tempo real total */
    time_value_t                total_wait_time;    /* Tempo em espera */
    time_value_t                total_block_time;   /* Tempo em bloqueio */
    time_value_t                total_sleep_time;   /* Tempo em sleep */
    time_value_t                total_idle_time;    /* Tempo idle */
    
    u64                         total_cpu_user;     /* CPU em modo usuário */
    u64                         total_cpu_system;   /* CPU em modo sistema */
    u64                         total_cpu_steal;    /* CPU roubado */
    u64                         total_cpu_guest;    /* CPU em guest */
    u64                         total_cpu_iowait;   /* CPU em iowait */
    
    unsigned long               nvcsw;          /* Mudanças voluntárias */
    unsigned long               nivcsw;         /* Mudanças involuntárias */
    unsigned long               context_switches; /* Total de mudanças */
    
    unsigned long               forks;          /* Número de forks */
    unsigned long               execs;          /* Número de execs */
    unsigned long               clones;         /* Número de clones */
    unsigned long               exits;          /* Número de exits */
    
    u64                         io_read_bytes;    /* Bytes lidos */
    u64                         io_write_bytes;   /* Bytes escritos */
    u64                         io_read_ops;      /* Operações de leitura */
    u64                         io_write_ops;     /* Operações de escrita */
    u64                         io_queued;        /* Operações em fila */
    u64                         io_serviced;      /* Operações servidas */
    u64                         io_service_time;  /* Tempo de serviço */
    u64                         io_wait_time;     /* Tempo de espera */
    
    time_value_t                creation_time;  /* Tempo de criação */
    time_value_t                start_time;     /* Tempo de início */
    time_value_t                end_time;       /* Tempo de término */

    /* ============================================================
     * SEÇÃO 9: AMOSTRAGEM E DEBUG
     * ============================================================ */
    
    sample_control_t            pc_sample;
    sample_control_t            pc_sample_prev;
    
    unsigned int                debug_flags;
    void                        *debug_info;
    unsigned int                trace_count;
    unsigned int                trace_enabled;
    
    unsigned int                profile_count;
    unsigned int                profile_hits;
    void                        *profile_data;

    /* ============================================================
     * SEÇÃO 10: USER SPACE E EMULAÇÃO
     * ============================================================ */
    
    struct eml_dispatch         *eml_dispatch;
    void                        *eml_data;
    unsigned int                eml_flags;
    unsigned int                eml_abi_version;
    
    void                        *user_data;
    void                        *user_tls;
    void                        *user_stack;
    unsigned int                user_flags;

    /* ============================================================
     * SEÇÃO 11: HIERARQUIA E IDENTIFICAÇÃO
     * ============================================================ */
    
    struct task                 *parent;
    struct task                 *real_parent;
    struct task                 *group_leader;
    struct task                 *session_leader;
    
    queue_head_t                children;
    queue_head_t                sibling;
    queue_head_t                pset_tasks;
    queue_head_t                task_list;
    
    unsigned int                pid;
    unsigned int                tgid;
    unsigned int                pgid;
    unsigned int                sid;
    unsigned int                ppid;
    unsigned int                ptid;
    
    uid_t                       uid;
    gid_t                       gid;
    uid_t                       euid;
    gid_t                       egid;
    uid_t                       suid;
    gid_t                       sgid;
    uid_t                       fsuid;
    gid_t                       fsgid;
    uid_t                       ruid;
    gid_t                       rgid;
    
    unsigned int                capabilities;
    unsigned int                capabilities_effective;
    unsigned int                capabilities_permitted;
    unsigned int                capabilities_inheritable;
    
    char                        comm[TASK_NAME_MAX];
    char                        *path;
    char                        *cwd;
    char                        *root;
    char                        *exe_path;
    char                        *cmdline;

    /* ============================================================
     * SEÇÃO 12: ESPECÍFICOS DA PLATAFORMA
     * ============================================================ */
    
#if NORMA_TASK
    long                        child_node;
    long                        parent_node;
    long                        current_node;
    unsigned int                node_count;
#endif
    
#if FAST_TAS
    vm_offset_t                 fast_tas_base[TASK_FAST_TAS_NRAS];
    vm_offset_t                 fast_tas_end[TASK_FAST_TAS_NRAS];
    unsigned int                fast_tas_count;
    unsigned int                fast_tas_active;
#endif
    
#if NET_ATM
    nw_ep_owned_t               nw_ep_owned;
    nw_ep_owned_t               nw_ep_pending;
    unsigned int                nw_ep_count;
#endif
    
#ifdef CONFIG_NUMA
    unsigned int                numa_node;
    unsigned int                numa_preferred;
    unsigned int                numa_memory;
    unsigned int                numa_remote;
    unsigned int                numa_local;
#endif

};

#define task_lock(task)		simple_lock(&(task)->lock)
#define task_unlock(task)	simple_unlock(&(task)->lock)

#define	itk_lock_init(task)	simple_lock_init(&(task)->itk_lock_data)
#define	itk_lock(task)		simple_lock(&(task)->itk_lock_data)
#define	itk_unlock(task)	simple_unlock(&(task)->itk_lock_data)

/*
 *	Exported routines/macros
 */

extern kern_return_t	task_create(
	task_t		parent_task,
	boolean_t	inherit_memory,
	task_t		*child_task);
extern kern_return_t	task_terminate(
	task_t		task);
extern kern_return_t	task_suspend(
	task_t		task);
extern kern_return_t	task_resume(
	task_t		task);
extern kern_return_t	task_threads(
	task_t		task,
	thread_array_t	*thread_list,
	natural_t	*count);
extern kern_return_t	task_info(
	task_t		task,
	int		flavor,
	task_info_t	task_info_out,
	natural_t	*task_info_count);
extern kern_return_t	task_get_special_port(
	task_t		task,
	int		which,
	struct ipc_port	**portp);
extern kern_return_t	task_set_special_port(
	task_t		task,
	int		which,
	struct ipc_port	*port);
extern kern_return_t	task_assign(
	task_t		task,
	processor_set_t	new_pset,
	boolean_t	assign_threads);
extern kern_return_t	task_assign_default(
	task_t		task,
	boolean_t	assign_threads);

/*
 *	Internal only routines
 */

extern void		task_init();
extern void		task_reference();
extern void		task_deallocate();
extern kern_return_t	task_hold();
extern kern_return_t	task_dowait();
extern kern_return_t	task_release();
extern kern_return_t	task_halt();

extern kern_return_t	task_suspend_nowait();
extern task_t		kernel_task_create();

extern task_t	kernel_task;

#endif	_KERN_TASK_H_
