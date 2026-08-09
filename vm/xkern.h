/* SPDX License Indentifier: GPL-2.0 */

/*
 * Copyright 1991-1998 by Open Software Foundation, Inc. 
 *              All Rights Reserved 
 */
/* 
 * xkern.h,v
 *
 * x-kernel v3.2
 *
 * Copyright (c) 1993,1991,1990  Arizona Board of Regents
 *
 *
 * msg_s.h,v
 * Revision 1.12.2.4  1994/07/21  23:21:46  menze
 * Messages now store Paths rather than Allocators
 *
 * Revision 1.12.2.3  1994/03/31  16:52:55  menze
 * Allocator field declaration changed
 *
 * Revision 1.12.2.2  1994/03/30  19:05:30  hkaram
 * Added the allocator field to the Msg structure
 *
 * Revision 1.12.2.1  1994/03/25  23:07:30  hkaram
 * Resource allocation modifications
 *
 * Revision 1.12  1993/12/14  18:23:35  menze
 *   [ 1993/12/11          menze ]
 *   fixed #endif comments
 *
 */

#include <linux/types.h>

typedef unsigend int      node_t;
typedef s64      Map;

typedef struct {
    node_t		mynode;
    Map         	actMap;
    Map         	pasMap;
    unsigned long	mtu;
} PState;

typedef struct {
    node_t		dest_node;
    long		protNum;
} ActiveId;

 struct rpc_h {
	    /* 
	     * HACK! As I do not copy RPC data at the KKT boundary, I
	     * need to have the rpc header contiguous to the data.
	     * Thus, I pad it myself above the rpc_header.
	     */
	    unsigned int        pad1;
	    unsigned int	*ulink;	        /* user available field */
	    handle_t		handle;		/* RPC handle */
	};

typedef struct {
    unsigned long max_length;
} PSTATE;

typedef struct msg {
  char	     *headPtr;
  char	     *tailPtr;
  char	     *stackHeadPtr;
  char	     *stackTailPtr;
  struct {
    unsigned short numNodes;
    unsigned int   myStack;
  } state;
  node_t  stack;
  struct rb_root     tree;
  void       *attr;
  u64  lastStack;
  char	     *lastStackTailPtr;
  struct {
    unsigned int   myLastStack;
  } tailstate;
  u64 *path;
  int	attrLen;
} Msg_s;

#endif
