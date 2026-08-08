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
   unsigend long      pooltype;
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
   struct mutex     lock;
};

struct object {
 s64      pointer;
 struct inode    inode;
 union {
    s64 handlecount;
    volatile void  nexttofree;
};
 struct object_type    type;
 u8    name;
 u8    handler;
 u8    quota;
 union {
 u64        cration_info;
 void      quotainfo;
};
 u8 body;
};
