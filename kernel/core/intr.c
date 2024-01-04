/* GPLv2 (c) Airbus */
#include <intr.h>
#include <debug.h>
#include <info.h>
#include <memory_map.h>
#include <io.h>
#include <timer.h>
#include <process.h>

extern info_t *info;
extern void idt_trampoline();
static int_desc_t IDT[IDT_NR_DESC];

void intr_init()
{
   idt_reg_t idtr;
   offset_t isr;
   size_t i;

   isr = (offset_t)idt_trampoline;

   /* re-use default grub GDT code descriptor */
   for (i = 0; i < IDT_NR_DESC; i++, isr += IDT_ISR_ALGN)
      int_desc(&IDT[i], gdt_krn_seg_sel(1), isr);

   idtr.desc = IDT;
   idtr.limit = sizeof(IDT) - 1;
   set_idtr(idtr);
}

void __regparm__(1) intr_hdlr(int_ctx_t *ctx)
{
   debug("\nIDT event\n"
         " . int    #%d\n"
         " . error  0x%x\n"
         " . cs:eip 0x%x:0x%x\n"
         " . ss:esp 0x%x:0x%x\n"
         " . eflags 0x%x\n"
         "\n- GPR\n"
         "eax     : 0x%x\n"
         "ecx     : 0x%x\n"
         "edx     : 0x%x\n"
         "ebx     : 0x%x\n"
         "esp     : 0x%x\n"
         "ebp     : 0x%x\n"
         "esi     : 0x%x\n"
         "edi     : 0x%x\n",
         ctx->nr.raw, ctx->err.raw, ctx->cs.raw, ctx->eip.raw, ctx->ss.raw, ctx->esp.raw, ctx->eflags.raw, ctx->gpr.eax.raw, ctx->gpr.ecx.raw, ctx->gpr.edx.raw, ctx->gpr.ebx.raw, ctx->gpr.esp.raw, ctx->gpr.ebp.raw, ctx->gpr.esi.raw, ctx->gpr.edi.raw);

   uint8_t vector = ctx->nr.blow;

   if (vector < NR_EXCP)
      excp_hdlr(ctx);
   else
      debug("ignore IRQ %d\n", vector);
}

void isr_count()
{
   asm volatile(
       "leave ; pusha        \n"
       "mov %esp, %eax      \n"
       "call sys_handler_count \n"
       "popa ; iret");
}

void __regparm__(1) sys_handler_count(int_ctx_t *ctx)
{
   uint32_t *counter = (uint32_t *)ctx->gpr.eax.raw;
   debug("counter = %d\n", *counter);
}

void isr_switch()
{
   asm volatile(
       "leave ; pusha ; cli     \n"
       "call sys_handler_switch \n"
       "popa ; iret");
}

void sys_handler_switch()
{
   outb(0x20, 0x20); // ack IRQ0

   debug("SYS HANDLER SWITCH\n");
   int len = get_process_list_len();

   // Only kernel is running
   if (len == 1)
      goto exit;

   process_t *process_list = get_process_list();
   int current_process = get_current_process();
   int new_process = (current_process + 1) % len;
   debug("Switching from %d to %d\n", current_process, new_process);

   // Save current esp
   process_list[current_process].esp = get_esp();

   // Set new pagination
   pde32_t *new_pgd;
   if (new_process == 2)
   {
      new_pgd = get_pgd2();
   }
   else
   {
      new_pgd = get_pgd1();
   }

   set_cr3(new_pgd);
   set_current_process(new_process);

   // If process running, resume
   if (process_list[new_process].started)
   {
      set_esp(process_list[new_process].esp);
   }

   // Else start user process
   else
   {
      process_list[new_process].started = true;
      run_user_process(new_process);
   }

exit:
   asm volatile("sti");
}

void reg_syscall(int int_num, int isr)
{
   int_desc_t *dsc;
   idt_reg_t idtr;
   get_idtr(idtr);
   dsc = &idtr.desc[int_num];
   dsc->offset_1 = (uint16_t)((uint32_t)isr);
   dsc->offset_2 = (uint16_t)(((uint32_t)isr) >> 16);
   dsc->dpl = 3;
}

void init_syscall()
{
   reg_syscall(0x80, (int)isr_count);
   reg_syscall(0x20, (int)isr_switch);
}
