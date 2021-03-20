#include "trap.h"
#include "gate.h"

void do_divide_error(unsigned long rsp, unsigned long error_code) {
    unsigned long *p = NULL;
    p = (unsigned long *)(rsp + 0x98);
    color_printk(RED, BLACK, "do_divide_error(0), ERROR_CODE:%#018lx, RSP:%#018lx, RIP:%#018lx\n", error_code, rsp, *p);
    while(1);
}

void sys_vector_init() {
    set_trap_gate(0, 1, divide_error);
}