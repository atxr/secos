#include <process.h>
#include <segmem.h>

process_t process_list[PROCESS_MAX];
int process_list_len;
int current_process;

void init_process_table()
{

    process_list[0].entry = 0;
    process_list[0].esp = 0;
    process_list[0].pgd = 0;
    process_list[0].pid = 0;
    process_list[0].ppid = 0;

    current_process = 0;
    process_list_len = 1;
}

void create_process(process_t p)
{
    process_list[process_list_len] = p;
    process_list_len++;
}

void run_user_process(int id)
{
    // Switch seg to ring 3
    set_seg_to_user();

    asm volatile(
        "push %0 \n" // ss
        "push %1 \n" // esp pour du ring 3 !
        "pushf   \n" // eflags
        "push %2 \n" // cs
        "push %3 \n" // eip
        "iret" ::
            "i"(d3_sel),
        "m"(process_list[id].esp),
        "i"(c3_sel),
        "r"(process_list[id].entry));
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
