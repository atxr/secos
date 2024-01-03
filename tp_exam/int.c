#include <intr.h>

void syscall_counter_isr() {
   asm volatile (
      "leave ; pusha        \n"
      "mov %esp, %eax      \n"
      "call syscall_counter_handler \n"
      "popa ; iret"
      );
}

void __regparm__(1) syscall_counter_handler(int_ctx_t *ctx) {
	uint32_t* counter = (uint32_t*) ctx->gpr.eax.raw;
   	debug("SYSCALL counter:\n@counter = %p\ncounter = %d\n", counter, *counter);
}

void reg_syscall(int int_num, int isr) {   
    int_desc_t *dsc;
    idt_reg_t  idtr;
    get_idtr(idtr);
    dsc = &idtr.desc[int_num];
    dsc->offset_1 = (uint16_t)((uint32_t)isr); 
    dsc->offset_2 = (uint16_t)(((uint32_t)isr)>>16);
    dsc->dpl = 3;
}

void init_syscall() {
    reg_syscall(0x80, (int) syscall_counter_isr);
    //reg_syscall(32, (int) switch_task_handler);
}
