#include <intr.h>

void sys_counter() {}
void switch_task() {}

void reg_syscall(int int_num, int handler) {
    idt_reg_t idtr;
    get_idtr(idtr);

    int_desc_t *bp_dsc = &idtr.desc[int_num];
    bp_dsc->offset_1 = (uint16_t)((uint32_t)handler);
    bp_dsc->offset_2 = (uint16_t)(((uint32_t)handler)>>16);
}

void init_syscall() {
    reg_syscall(0x80, (int) sys_counter);
    reg_syscall(32, (int) switch_task);
}
