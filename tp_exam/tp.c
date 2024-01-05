/* GPLv2 (c) Airbus */
#include <debug.h>
#include <tasks.h>
#include <process.h>
#include <io.h>
#include <segmem.h>
#include <intr.h>
#include <timer.h>
#include <asm.h>

uint32_t stack0_base = T0_STACK + (STACK_SIZE - 1) * sizeof(uint32_t);

uint32_t __user1_data__ stack1[STACK_SIZE];
uint32_t stack1_base = (uint32_t)(&stack1[STACK_SIZE - 1]);

uint32_t __user2_data__ stack2[STACK_SIZE];
uint32_t stack2_base = (uint32_t)(&stack2[STACK_SIZE - 1]);

void init_kernel()
{
    init_flat_seg();
    init_pagination();
    init_process_table();

    init_syscall();
    init_timer();
    force_interrupts_on();

    //* DEBUG
    debug_gdt();
    debug("SS: %d\nStack @ 0x%x\nCS: %d\neip @ %p\n", d3_idx, stack1_base, c3_idx, task2);
    debug_pagination((int)task1);
    debug_pagination((int)T1_SHARED_MEM);
    //*/
}

void init_processes()
{
    process_t process0 = {
        .entry = task0,
        .ebp = stack0_base,
        .pgd = get_pgd(0),
        .pid = 0,
        .started = false};

    process_t process1 = {
        .entry = task1,
        .ebp = stack1_base,
        .pgd = get_pgd(1),
        .pid = 1,
        .ppid = 0};

    process_t process2 = {
        .entry = task2,
        .ebp = stack2_base,
        .pgd = get_pgd(2),
        .pid = 2,
        .ppid = 0};

    create_process(process0);
    create_process(process1);
    create_process(process2);
}

void tp()
{
    init_kernel();
    init_processes();

    for (;;)
    {
    }
}