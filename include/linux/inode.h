/* SPDX License Indentifier: GPL-2.0 */
/* modified by Pedro Emanuel */

#ifndef LINUX_INODE_H
#define LINUX_INODE_H

#include <linux/sched.h>
#include <vm/vm_map.h>
#include <mach/mutex.h>
#include <kern/lock.h>

struct address_space {
	struct inode		*host;
	struct rb_node		i_pages;
	struct mutex	invalidate_lock;
	atomic_t		i_mmap_writable;
	struct rb_root_cached	i_mmap;
	unsigned long		nrpages;
	unsigned long		flags;
    struct lock			*spin_lock;
} __attribute__((aligned(sizeof(long)))) __randomize_layout;

struct inode {
	unsigned short			i_mode;
	unsigned short		i_opflags;
	unsigned int		i_flags;
	u32			i_rdev;
	u64			i_size;
	s64		i_atime_sec;  //in linux time64_t = s64
	s64		i_mtime_sec;
	s64		i_ctime_sec;
	u32			i_atime_nsec; 
	u32			i_mtime_nsec;
	u32			i_ctime_nsec;
	u32			i_generation;
	unsigned short          i_bytes;
	u8			i_blkbits;
  u64		i_blocks;
  unsigned long		dirtied_when;	/* jiffies of first dirtying */
	unsigned long		dirtied_time_when;
	struct hlist_node	i_hash;
	struct list_head	i_io_list;	/* backing dev IO list */
  atomic64_t		i_version;
	atomic64_t		i_sequence; /* see futex */
	atomic_t		i_count;
	atomic_t		i_dio_count;
	atomic_t		i_writecount;
  char			*i_link;
		unsigned		i_dir_seq;
  struct rb_node      *rb_tree;
  struct vm_map      *virtual_map;
  struct adress_space		*object_node_space;
};

struct file {
	unsigned short f_mode;
	unsigned short f_flags;
	unsigned short f_count;
	struct inode * f_inode;
    s64 f_pos;
    struct file		*prev;
    struct file     *next;
};


#endif /* LINUX_INODE_H */
