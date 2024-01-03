/* GPLv2 (c) Airbus */
#include <debug.h>
#include <user.h>
#include <process.h>
#include <io.h>
#include <segmem.h>
#include <intr.h>
#include <timer.h>
#include <asm.h>

int *__user_data__ stack1 = (int *)(T1_STACK + 0x80000);
int *__user_data__ stack2 = (int *)(T2_STACK + 0x80000);

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
    debug("SS: %d\nStack @ %p\nCS: %d\neip @ %p\n", d3_idx, stack1, c3_idx, task2);
    debug_pagination((int)task1);
    debug_pagination((int)T1_SHARED_MEM);
    //*/
}

void init_processes()
{
    process_t process1 = {
        .entry = task1,
        .esp = stack1,
        .pgd = get_pgd1(),
        .pid = 1,
        .ppid = 0};

    process_t process2 = {
        .entry = task2,
        .esp = stack2,
        .pgd = get_pgd2(),
        .pid = 2,
        .ppid = 0};

    create_process(process1);
    create_process(process2);
}

void tp()
{
    init_kernel();
    init_processes();
    run_user_process(1);

    for (;;)
    {
        debug("Kernel reporting for duty\n");
    }
}