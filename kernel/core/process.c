#include <process.h>
#include <segmem.h>
#include <io.h>
#include <debug.h>
#include <timer.h>

process_t process_list[PROCESS_MAX];
int process_list_len;
int current_process;

void init_process_table()
{
    current_process = -1;
    process_list_len = 0;
}

void create_process(process_t p)
{
    process_list[process_list_len] = p;
    process_list_len++;
}

void run_process(int id)
{
    debug("Run process %d\n", id);

    if (id > 0)
    {
        asm volatile(
            "push %0 \n" // ss
            "push %1 \n" // esp pour du ring 3 !
            "pushf   \n" // eflags
            "push %2 \n" // cs
            "push %3 \n" // eip
            ::"i"(d3_sel),
            "m"(process_list[id].ebp),
            "i"(c3_sel),
            "r"(process_list[id].entry));
    }

    else
    {
        asm volatile(
            "push %0 \n" // ss
            "push %1 \n" // esp
            "pushf   \n" // eflags
            "push %2 \n" // cs
            "push %3 \n" // eip
            ::"i"(d0_sel),
            "m"(process_list[id].ebp),
            "i"(c0_sel),
            "r"(process_list[id].entry));
    }

    outb(0x20, 0x20); // ack IRQ0
    asm volatile("iret");
}

process_t *get_process_list()
{
    return process_list;
}

int get_process_list_len()
{
    return process_list_len;
}

int get_current_process()
{
    return current_process;
}

void set_current_process(int new_process)
{
    current_process = new_process;
}
