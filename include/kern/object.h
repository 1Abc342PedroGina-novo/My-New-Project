/* SPDX License Indentifier: GPL-2.0 */
/* created by Pedro Emanuel */

#ifndef KERN_OBJECT_H
#define KERN_OBJECT_H

#include <mach/mutex.h>
#include <linux/inode.h>

struct object_info {
       u64      name;
   unsigned long      index;
   unsigned long      totalnumberofobject;
   unsigned long      totalnumberofhandles;
   unsigned long      highwaterobjnmb;
   unsigned long      highwaterhdrmbr;
   unsigned long      invalid_attr;
   bool        boolean;
   unsigned long      pooltype;
};

struct object_type {
   u64      eresource;
   struct mutex    mutex;
   void    *defaultobj;
   unsigned long      index;
   unsigned long      totalnumberofobject;
   unsigned long      totalnumberofhandles;
   unsigned long      highwaterobjnmb;
   unsigned long      highwaterhdrmbr;
   struct object_info data;
   unsigned long      key;
   struct lock    lock;
};

struct object {
 s64      pointer;
 struct inode    *inode;
 union {
    s64 handlecount;
    volatile void  nexttofree;
};
 struct object_type    type;
 u8    name;
 u8    handler;
 u8    quota;
 union {
 u64        creation_info;
 void      quotainfo;
};
 u8 body;
};


#define Thread    ((struct thread*)0)
#define Task      ((struct task*)0)
#define Memory_Page ((struct vm_page*)0)
#define Virtual_Map ((struct vm_map*)0)
#define Mutex     ((struct mutex*)0)
#define File      ((struct file*)0)
#define IPC_Message ((struct message*)0)
#define Key       ((void*)0)

#endif
