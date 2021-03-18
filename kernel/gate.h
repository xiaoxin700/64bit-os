#ifndef __GATE_H__
#define __GATE_H__

struct desc_struct {
    unsigned char x[8];
};

struct gate_struct {
    unsigned char x[16];
};

extern struct desc_struct GDT_Table[];
extern struct gate_struct IDT_Table[];
extern unsigned int TSS64_Table[26];

#define _set_gate(gate_selector_addr, attr, ist, code_addr)    \
do                                                                                      \
{    unsigned long __d0, __d1;                                              \
     __asm__ __volatile__ (     "movw   %%dx,   %%ax    \n\t"  \
                    "andq   $0x7,   %%rcx   \n\t"                           \
                    "addq   %4, %%rcx   \n\t"                                \
                    "shlq   $32,    %%rcx   \n\t"                            \
                    "addq   %%rcx,  %%rax   \n\t"                          \
                    "movl %%edx,    %%ecx   \n\t"                         \
                    "shrq   $16,    %%rcx   \n\t"                            \
                    "shlq   $48,    %%rcx   \n\t"                            \
                    "addq   %%rcx,  %%rax   \n\t"                          \
                    "movq   %%rax,  %0  \n\t"                               \
                    "shrq   $32,    %%rdx   \n\t"                            \
                    "movq   %%rdx,  %1  \n\t"                               \
                    :"=m"(*((unsigned long *)(gate_selector_addr))),    \
                     "=m"(*(1 + (unsigned long *)(gate_selector_addr))),"=&a"(__d0),"=&d"(__d1) \
                    :"i"(attr << 8),                                                \
                     "3"((unsigned long *)(code_addr)),"2"(0x8 << 16), "c"(ist)     \
                    :"memory"                                                      \
     );                                                                                   \
}while(0)

inline void set_trap_gate(unsigned int n, unsigned char ist, void *addr) {
    _set_gate(IDT_Table + n, 0x8F, ist, addr);
}