#ifndef __TRAP_H__

#define __TRAP_H__

#include "linkage.h"
#include "printk.h"
#include "lib.h"

void divide_error();
void nmi();
void page_fault();
void invalid_TSS();

void sys_vector_init();

#endif