/* SPDX-License-Identifier: GPL-2.0 */
/*
 * asm-generic/int-ll64.h
 *
 * Integer declarations for architectures which use "long long"
 * for 64-bit types.
 */

/* modifications:
 *
 * I add "typedefs" of <asm-generic/int-l64.h>, <asm-generic/int-ll64.h>, <uapi/linux/types.h>, <linux/types.h> and <bsd/sys/types.h>
 * I changed the value of u64/__u64 to unsigned long long, so it's no longer unsigned long.
 */

/* If this code bothers you, due to the kernel model, including the hybrid architecture or the somewhat distant UNIX-like style, I apologize. */

#ifndef INT_LL64_H
#define INT_LL64_H

typedef __signed__ char __s8;
typedef unsigned char __u8;

typedef __signed__ short __s16;
typedef unsigned short __u16;

typedef __signed__ int __s32;
typedef unsigned int __u32;

typedef __signed__ long long __s64;
typedef unsigned long long __u64;

typedef __s8  s8;
typedef __u8  u8;
typedef __s16 s16;
typedef __u16 u16;
typedef __s32 s32;
typedef __u32 u32;
typedef __s64 s64;
typedef __u64 u64;


#define S8_C(x)  x
#define U8_C(x)  x ## U
#define S16_C(x) x
#define U16_C(x) x ## U
#define S32_C(x) x
#define U32_C(x) x ## U
#define S64_C(x) x ## LL
#define U64_C(x) x ## ULL

typedef __signed__ __int128 __s128 __attribute__((aligned(16)));
typedef unsigned __int128 __u128 __attribute__((aligned(16)));

typedef	unsigned short	ushort;		/* Sys V compatibility */
typedef	unsigned int	uint;		/* Sys V compatibility */
typedef	unsigned char	u_char;
typedef	unsigned short	u_short;
typedef	unsigned int	u_int;
typedef	unsigned long	u_long;

typedef struct {
	int __aligned(sizeof(int)) counter;
} atomic_t;

typedef struct {
	s64 counter;
} atomic64_t;

typedef struct {
	atomic_t refcnt;
} rcuref_t;

struct list_head {
	struct list_head *next, *prev;
};

struct hlist_head {
	struct hlist_node *first;
};

struct hlist_node {
	struct hlist_node *next, **pprev;
};


#endif /* INT_LL64_H */
