#include <intr.h>

void sys_counter_handler() {}
void switch_task_handler() {}

void reg_syscall(int int_num, int handler) {
    idt_reg_t idtr;
    get_idtr(idtr);

    int_desc_t *bp_dsc = &idtr.desc[int_num];
    bp_dsc->offset_1 = (uint16_t)((uint32_t)handler);
    bp_dsc->offset_2 = (uint16_t)(((uint32_t)handler)>>16);
}

void init_syscall() {
    reg_syscall(0x80, (int) sys_counter_handler);
    reg_syscall(32, (int) switch_task_handler);
}
