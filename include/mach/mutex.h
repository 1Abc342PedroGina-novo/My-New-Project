/* SPDX License Indentifier: GPL-2.0 */
/* created by Pedro Emanuel */
/* HISTORIC:
 * 9:37 08/08/26 
 * i make struct mutex 
 * and define MUTEX_INITIALIZER
 */

#ifndef MACH_MUTEX_H
#define MACH_MUTEX_H

#include <asm-generic/int-ll64.h>

typedef struct mutex {
    int lock;       /* Estado do cadeado: 0 para livre, 1 para ocupado */
    char *name;     /* Nome do mutex para fins de depuração */
    u64    lock_byte;
#if defined(__aarch64__)
   u64    lock_arm64;
#elif defined(__x86_64__)
   u64    lock_i64;
#endif
} *mutex_t;

#define MUTEX_INITIALIZER { 0, 0 }

