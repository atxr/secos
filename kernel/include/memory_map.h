#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

#include <pagemem.h>
#include <cr.h>

// Pagination
#define T1_PGD 0x380000
#define T1_PTB0 0x381000
#define T1_PTB1 0x382000
#define T1_PTB2 0x383000

#define T2_PGD 0x390000
#define T2_PTB0 0x391000
#define T2_PTB1 0x392000
#define T2_PTB2 0x393000

// Userland memory areas
#define __user_data__ __attribute__((section(".user_data"), aligned(4)))

#define T1_STACK 0x500000
#define T1_SHARED_MEM 0x800000

#define T2_STACK 0x600000
#define T2_SHARED_MEM 0x400000

#define SHARED_MEM 0xc00000

void init_pagination();
void debug_pagination(int address);

pde32_t *get_pgd1();
pde32_t *get_pgd2();

#endif
