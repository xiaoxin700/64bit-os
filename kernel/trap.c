#include "trap.h"
#include "gate.h"

void sys_vector_init() {
    set_trap_gate(0, 1, divide_error);
}