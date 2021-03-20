#include "trap.h"
#include "gate.h"

void do_divide_error(unsigned long rsp, unsigned long error_code) {
    unsigned long *p = NULL;
    p = (unsigned long *)(rsp + 0x98);
    color_printk(RED, BLACK, "do_divide_error(0), ERROR_CODE:%#018lx, RSP:%#018lx, RIP:%#018lx\n", error_code, rsp, *p);
    while(1);
}

void do_nmi(unsigned long rsp, unsigned long err_code) {
    unsigned long *p = NULL;
    p = (unsigned long *)(rsp + 0x98);
    color_printk(RED, BLACK, "do_nmi(2), ERROR_CODE:%#018lx, RSP:%#018lx, RIP:%#018lx\n", err_code, rsp, *p);
    while(1);
}

void do_invalid_TSS(unsigned long rsp, unsigned long error_code) {
    unsigned long *p = NULL;
    p = (unsigned long *)(rsp + 0x98);
    color_printk(RED, BLACK, "do_invalid_TSS(10), ERROR_CODE:%#018lx, RSP:%#018lx, RIP:%#018lx\n", error_code, rsp, *p);

    if (error_code & 0x01) {
        color_printk(RED, BLACK, "The exception occured during delivery of an event external to the program, such as an interrupt or an earlier exception.\n");
    }

    if (error_code & 0x02) {
        color_printk(RED, BLACK, "Refers to a gate descriptor in the IDT;\n");
    } else {
        color_printk(RED, BLACK, "Refers to a descriptor in the GDT or the current LDT;\n");
    }

    if ((error_code & 0x02) == 0) {
        if (error_code & 0x04) {
            color_printk(RED, BLACK, "Refers to a segment or gate descriptor in the LDT:\n");
        } else {
            color_printk(RED, BLACK, "Refers to a descriptor in the current GDT;\n");
        }
    }

    color_printk(RED, BLACK, "Segment Selector index:%#010x\n", error_code&0xfff8);

    while(1);
}

void do_page_fault(unsigned long rsp, unsigned long error_code) {
    unsigned long *p = NULL;
    unsigned long cr2 = 0;

    __asm__ __volatile__("movq %%cr2,   %0":"=r"(cr2)::"memory");

    p = (unsigned long *)(rsp + 0x98);
    color_printk(RED, BLACK, "do_page_fault(14), ERROR_CODE:%#018lx, RSP:%#018lx, RIP:%#018lx\n", error_code, rsp, *p);

    if (!(error_code & 0x01)) {
        color_printk(RED, BLACK, "Page Not-Present,\t");
    }

    if (error_code & 0x02) {
        color_printk(RED, BLACK, "Write Cause Fault,\t");
    } else {
        color_printk(RED, BLACK, "Read Cause Fault,\t");
    }

    if (error_code & 0x04) {
        color_printk(RED, BLACK, "Fault in user(3)\t");
    } else {
        color_printk(RED, BLACK, "Fault in supervisor(0,1,2)\t");
    }

    if (error_code & 0x08) {
        color_printk(RED, BLACK, ",Reserved Bit Cause Fault\t");
    }

    if (error_code & 0x10) {
        color_printk(RED, BLACK, ",Instruction fetch Cause Fault");
    }

    color_printk(RED, BLACK, "\n");
    color_printk(RED, BLACK, "CR2:%#018lx\n", cr2);

    while(1);
}

void sys_vector_init() {
    set_trap_gate(0, 1, divide_error);
    set_intr_gate(2, 1, nmi);
    set_trap_gate(14, 1, page_fault);
}