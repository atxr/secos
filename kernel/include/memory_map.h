#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

#include <pagemem.h>
#include <cr.h>

#define STACK_SIZE 0x8000

// Pagination
#define T0_PGD 0x370000
#define T0_PTB0 0x371000

#define T1_PGD 0x380000
#define T1_PTB0 0x381000
#define T1_PTB1 0x382000
#define T1_PTB2 0x383000

#define T2_PGD 0x390000
#define T2_PTB0 0x391000
#define T2_PTB1 0x392000
#define T2_PTB2 0x393000

// Stacks

#define KERNEL_STACK1 0x3b0000
#define KERNEL_STACK2 0x3d0000
#define T0_STACK 0x3b0000
#define T1_KERNEL_STACK 0x3d0000
#define T2_KERNEL_STACK 0x3e0000

// Userland memory areas
#define __user1_data__ __attribute__((section(".user1_data"), aligned(4)))
#define __user2_data__ __attribute__((section(".user2_data"), aligned(4)))

#define T1_STACK 0x600000
#define T1_SHARED_MEM 0x800000

#define T2_STACK 0xa00000
#define T2_SHARED_MEM 0x400000

#define SHARED_MEM 0xc00000

void init_pagination();
void debug_pagination(int address);

pde32_t *get_pgd(int id);

#endif
