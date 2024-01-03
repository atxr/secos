#include <debug.h>

#include "seg.c"
#include "int.c"
#include "pagin.c"

#define __user_data__ 	__attribute__ ((section(".user_data"),aligned(4)))

#define T1_STACK		0x500000
#define T1_SHARED_MEM	0x800000

#define T2_STACK		0x600000
#define T2_SHARED_MEM	0x400000

static int* __user_data__ stack1 = (int*) (T1_STACK + 0x80000);
static int* __user_data__ stack2 = (int*) (T2_STACK + 0x80000);

int current_task = 1;

void switch_task() {
	int new_task = 3 - current_task;
	debug("Switching to task %d\n", new_task);
	
	debug("Stack @ %p\n", new_task == 1 ? stack1 : stack2); 

	// Update CR3
	// TODO
	
	// Flush TLB
	// TODO
}

__attribute__((section(".user"))) void task1() {
	uint32_t* counter = (uint32_t*) T1_SHARED_MEM;
	*counter += 10;
	
   	asm volatile ("int $0x80"::"a"(counter));
	*counter += 10;
	
   	asm volatile ("int $0x80"::"a"(counter));
	
	for(;;) {}
}

__attribute__((section(".user"))) void task2() {
	uint32_t* counter =  (uint32_t*) T2_SHARED_MEM;
   	asm volatile ("int $0x80"::"a"(counter));
   	
	for(;;) {}
}

void trigger() {}

void run_ring3() {
	init_flat_seg();
	debug("Segmentation OK.\n");

	init_pagination();
	debug("Pagination OK.\n");

	// Switch seg to ring 3
	set_ds(d3_sel);
    set_es(d3_sel);
    set_fs(d3_sel);
    set_gs(d3_sel);
    TSS.s0.esp = get_ebp();
    TSS.s0.ss  = d0_sel;
    tss_dsc(&GDT[ts_idx], (offset_t)&TSS);
    set_tr(ts_sel);
    
	init_syscall();
	debug("Syscalls OK.\n");

	// DEBUG
	debug_gdt();
	debug("SS: %d\nStack @ %p\nCS: %d\neip @ %p\n", d3_idx, stack1, c3_idx, task2);

	debug_pagination((int)task1);
	// DEBUG
	
	trigger();
	
	debug_pagination((int)T1_SHARED_MEM);
	
    asm volatile (
      "push %0 \n" // ss
      "push %1 \n" // esp pour du ring 3 !
      "pushf   \n" // eflags
      "push %2 \n" // cs
      "push %3 \n" // eip
      "iret"
      ::
       "i"(d3_sel),
       "m"(stack1),
       "i"(c3_sel),
       "r"(0x400000)
      );
}
