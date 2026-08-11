/* SPDX License indentifier: GPL-2.0 */
/* Created by Pedro Emanuel 2026 */

#ifndef KERN_PMAP_H
#define KERN_PMAP_H

#include <vm/vm_map.h>
#include <mach/vm_statistics.h>


struct pmap {
	u32	*dirbase;	/* page directory pointer register */
	vm_offset_t	pdirbase;	/* phys. address of dirbase */
	int		ref_count;	/* reference count */
	decl_simple_lock_data(,lock)	/* lock on map */
	struct pmap_statistics	stats;	/* map statistics */
	voilate long long	cpus_using;	/* bitmap of cpus using pmap */
 pgd_t *pgd
	boolean_t	private_pmap;	/* if set, flush_tlb() not needed */
};

typedef struct pv_entry {
	struct pv_entry	*next;		/* next pv_entry */
	struct pmap		pmap;		/* pmap where mapping lies */
	vm_offset_t	va;		/* virtual address for mapping */
} *pv_entry_t;

struct pmap_update_item {
  struct pmap		pmap;		/* pmap to invalidate */
	vm_offset_t	start;		/* start address to invalidate */
	vm_offset_t	end;		/* end address to invalidate */
};

struct pmap_update_list {
	decl_simple_lock_data(,lock)
	int			count;
	struct pmap_update_item	item[UPDATE_LIST_SIZE];
};

#define PMAP_ALIAS_MAX 32
struct pmap_alias {
        vm_offset_t rpc;
        pmap_t pmap;
        vm_offset_t va;
        int cookie;
#define PMAP_ALIAS_COOKIE 0xdeadbeef
} pmap_aliasbuf[PMAP_ALIAS_MAX];
int pmap_alias_index = 0;

#endif /* KERN_PMAP_H */
