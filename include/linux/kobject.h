#ifndef LINUX_KOBJECT_H
#define LINUX_KOBJECT_H

#include <kern/object.h>

struct kobject {
	const char		*name;
	struct list_head	entry;
	struct kobject		*parent;
  struct object      *object;
	unsigned int state_initialized:1;
	unsigned int state_in_sysfs:1;
	unsigned int state_add_uevent_sent:1;
	unsigned int state_remove_uevent_sent:1;
	unsigned int uevent_suppress:1;
};

#endif
