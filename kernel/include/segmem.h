/* GPLv2 (c) Airbus */
#ifndef __SEGMEM_H__
#define __SEGMEM_H__

#include <types.h>
#include <gpr.h>

typedef struct descriptor_table_register
{
   uint16_t limit;
   raw32_t base;

} __attribute__((packed)) dt_reg_t;

/*
** Segment Selector
*/
#define SEG_SEL_GDT 0
#define SEG_SEL_LDT 1

#define SEG_SEL_KRN 0
#define SEG_SEL_USR 3

typedef union segment_selector
{
   struct
   {
      uint16_t rpl : 2;    /* bits 0-1  requested privilege level */
      uint16_t ti : 1;     /* bit  2    table indicator gdt(0) or ldt(1) */
      uint16_t index : 13; /* bits 3-15 index in descriptor table */

   } __attribute__((packed));

   uint16_t raw;

} __attribute__((packed)) seg_sel_t;

#define dt_seg_sel(idx, ti, rpl) (((idx) << 3) | ((ti) << 2) | (rpl))
#define gdt_seg_sel(idx, rpl) dt_seg_sel(idx, SEG_SEL_GDT, rpl)
#define ldt_seg_sel(idx, rpl) dt_seg_sel(idx, SEG_SEL_LDT, rpl)
#define gdt_krn_seg_sel(idx) gdt_seg_sel(idx, SEG_SEL_KRN)
#define gdt_usr_seg_sel(idx) gdt_seg_sel(idx, SEG_SEL_USR)

/*
** Segment Descriptor
*/
#define SEG_DESC_DATA_R 0x0
#define SEG_DESC_DATA_RA 0x1
#define SEG_DESC_DATA_RW 0x2
#define SEG_DESC_DATA_RWA 0x3
#define SEG_DESC_DATA_ER 0x4
#define SEG_DESC_DATA_ERA 0x5
#define SEG_DESC_DATA_ERW 0x6
#define SEG_DESC_DATA_ERWA 0x7

#define SEG_DESC_CODE_X 0x8
#define SEG_DESC_CODE_XA 0x9
#define SEG_DESC_CODE_XR 0xa
#define SEG_DESC_CODE_XRA 0xb
#define SEG_DESC_CODE_CX 0xc
#define SEG_DESC_CODE_CXA 0xd
#define SEG_DESC_CODE_CXR 0xe
#define SEG_DESC_CODE_CXRA 0xf

#define SEG_DESC_SYS_TSS_AVL_16 0x1
#define SEG_DESC_SYS_LDT 0x2
#define SEG_DESC_SYS_TSS_BUSY_16 0x3
#define SEG_DESC_SYS_CALL_GATE_16 0x4
#define SEG_DESC_SYS_TASK_GATE 0x5
#define SEG_DESC_SYS_INTR_GATE_16 0x6
#define SEG_DESC_SYS_TRAP_GATE_16 0x7
#define SEG_DESC_SYS_TSS_AVL_32 0x9
#define SEG_DESC_SYS_TSS_BUSY_32 0xb
#define SEG_DESC_SYS_CALL_GATE_32 0xc
#define SEG_DESC_SYS_INTR_GATE_32 0xe
#define SEG_DESC_SYS_TRAP_GATE_32 0xf

#define SEG_DESC_SYS_LDT_64 0x2
#define SEG_DESC_SYS_TSS_AVL_64 0x9
#define SEG_DESC_SYS_TSS_BUSY_64 0xb
#define SEG_DESC_SYS_CALL_GATE_64 0xc
#define SEG_DESC_SYS_INTR_GATE_64 0xe
#define SEG_DESC_SYS_TRAP_GATE_64 0xf

typedef union segment_descriptor
{
   struct
   {
      uint64_t limit_1 : 16; /* bits 00-15 of the segment limit */
      uint64_t base_1 : 16;  /* bits 00-15 of the base address */
      uint64_t base_2 : 8;   /* bits 16-23 of the base address */
      uint64_t type : 4;     /* segment type */
      uint64_t s : 1;        /* descriptor type */
      uint64_t dpl : 2;      /* descriptor privilege level */
      uint64_t p : 1;        /* segment present flag */
      uint64_t limit_2 : 4;  /* bits 16-19 of the segment limit */
      uint64_t avl : 1;      /* available for fun and profit */
      uint64_t l : 1;        /* longmode */
      uint64_t d : 1;        /* default length, depend on seg type */
      uint64_t g : 1;        /* granularity */
      uint64_t base_3 : 8;   /* bits 24-31 of the base address */

   } __attribute__((packed));

   raw64_t;

} __attribute__((packed)) seg_desc_t;

/*
** Global descriptor table
*/
typedef struct global_descriptor_table_register
{
   uint16_t limit; /* dt limit = size - 1 */
   union           /* base address */
   {
      offset_t addr;
      seg_desc_t *desc;
   };

} __attribute__((packed)) gdt_reg_t;

/*
** Local descriptor table
*/
typedef struct local_descriptor_table_register
{
   seg_sel_t sel;  /* segment selector */
   uint16_t limit; /* segment limit */
   union           /* base address */
   {
      offset_t addr;
      seg_desc_t *desc;
   };

} __attribute__((packed)) ldt_reg_t;

/*
** Interrupt descriptor
*/
typedef union interrupt_descriptor
{
   struct
   {
      uint64_t offset_1 : 16; /* bits 00-15 of the isr offset */
      uint64_t selector : 16; /* isr segment selector */
      uint64_t ist : 3;       /* stack table: only 64 bits */
      uint64_t zero_1 : 5;    /* must be 0 */
      uint64_t type : 4;      /* interrupt/trap gate */
      uint64_t zero_2 : 1;    /* must be zero */
      uint64_t dpl : 2;       /* privilege level */
      uint64_t p : 1;         /* present flag */
      uint64_t offset_2 : 16; /* bits 16-31 of the isr offset */

   } __attribute__((packed));

   raw64_t;

} __attribute__((packed)) int_desc_t;

/*
** Interrupt descriptor table
*/
typedef struct interrupt_descriptor_table_register
{
   uint16_t limit; /* dt limit = size - 1 */
   union           /* base address */
   {
      offset_t addr;
      int_desc_t *desc;
   };

} __attribute__((packed)) idt_reg_t;

/*
** Task state segment structures
*/
typedef struct task_state_segment_stack
{
   uint32_t esp;
   uint16_t ss;
   uint16_t pad;

} __attribute__((packed)) tss_stack_t;

typedef struct task_state_segment_maps
{
   uint16_t addr;
   uint8_t intr[32];
   uint8_t io[8192];
   uint8_t boundary;

} __attribute__((packed)) tss_maps_t;

typedef union task_state_segment_trap
{
   struct
   {
      uint16_t on : 1; /* #DB on task switch */
      uint16_t pad : 15;

   } __attribute__((packed));

   uint16_t raw;

} __attribute__((packed)) tss_trap_t;

/*
** 32 bits TSS
*/
typedef struct task_state_segment
{
   uint16_t back_link, r1;

   tss_stack_t s0;
   tss_stack_t s1;
   tss_stack_t s2;

   uint32_t cr3;
   uint32_t eip;
   eflags_reg_t eflags;
   gpr_t gpr;
   uint16_t es, r5;
   uint16_t cs, r6;
   uint16_t ss, r7;
   uint16_t ds, r8;
   uint16_t fs, r9;
   uint16_t gs, r10;
   uint16_t ldt, r11;

   tss_trap_t trap;
   tss_maps_t maps;

} __attribute__((packed)) tss_t;

/*
** Various segmentation related cpu instructions
*/
#define get_seg_sel(_reg_)                                 \
   ({                                                      \
      uint16_t seg;                                        \
      asm volatile("movw %%" #_reg_ ", %%ax" : "=a"(seg)); \
      seg;                                                 \
   })

#define set_seg_sel(_sel_, _reg_) \
   asm volatile("movw %%ax, %%" #_reg_ ::"a"(_sel_))

#define get_ss() get_seg_sel(ss)
#define get_ds() get_seg_sel(ds)
#define get_es() get_seg_sel(es)
#define get_fs() get_seg_sel(fs)
#define get_gs() get_seg_sel(gs)

#define set_ss(v) set_seg_sel(v, ss)
#define set_ds(v) set_seg_sel(v, ds)
#define set_es(v) set_seg_sel(v, es)
#define set_fs(v) set_seg_sel(v, fs)
#define set_gs(v) set_seg_sel(v, gs)

#define get_gdtr(aLocation) \
   asm volatile("sgdt %0" ::"m"(aLocation) : "memory")
#define get_ldtr(aLocation) \
   asm volatile("sldt %0" ::"m"(aLocation) : "memory")
#define get_idtr(aLocation) \
   asm volatile("sidt %0" ::"m"(aLocation) : "memory")
#define get_tr(aLocation) \
   asm volatile("str   %0" ::"m"(aLocation) : "memory")

#define set_gdtr(val) \
   asm volatile("lgdt  %0" ::"m"(val) : "memory")
#define set_ldtr(val) \
   asm volatile("lldt  %0" ::"m"(val) : "memory")
#define set_idtr(val) \
   asm volatile("lidt  %0" ::"m"(val) : "memory")
#define set_tr(val) \
   asm volatile("ltr   %%ax" ::"a"(val))

#define farjump(_fptr) \
   asm volatile("ljmp  *%0" ::"m"(_fptr) : "memory");
#define set_cs_eip(_cs, _eip) \
   asm volatile("ljmp  %0, %1" ::"i"(_cs), "i"(_eip))
#define set_cs(_cs) \
   asm volatile("ljmp  %0, $1f ; 1:" ::"i"(_cs))

#define c0_idx 1
#define d0_idx 2
#define c3_idx 3
#define d3_idx 4
#define tss_idx 5

#define c0_sel gdt_krn_seg_sel(c0_idx)
#define d0_sel gdt_krn_seg_sel(d0_idx)
#define c3_sel gdt_usr_seg_sel(c3_idx)
#define d3_sel gdt_usr_seg_sel(d3_idx)
#define tss_sel gdt_krn_seg_sel(tss_idx)

#define gdt_flat_dsc(_dSc_, _pVl_, _tYp_) \
   ({                                     \
      (_dSc_)->raw = 0;                   \
      (_dSc_)->limit_1 = 0xffff;          \
      (_dSc_)->limit_2 = 0xf;             \
      (_dSc_)->type = _tYp_;              \
      (_dSc_)->dpl = _pVl_;               \
      (_dSc_)->d = 1;                     \
      (_dSc_)->g = 1;                     \
      (_dSc_)->s = 1;                     \
      (_dSc_)->p = 1;                     \
   })

#define tss_dsc(_dSc_, _tSs_)                  \
   ({                                          \
      raw32_t addr = {.raw = _tSs_};           \
      (_dSc_)->raw = sizeof(tss_t);            \
      (_dSc_)->base_1 = addr.wlow;             \
      (_dSc_)->base_2 = addr._whigh.blow;      \
      (_dSc_)->base_3 = addr._whigh.bhigh;     \
      (_dSc_)->type = SEG_DESC_SYS_TSS_AVL_32; \
      (_dSc_)->p = 1;                          \
   })

#define c0_dsc(_d) gdt_flat_dsc(_d, 0, SEG_DESC_CODE_XR)
#define d0_dsc(_d) gdt_flat_dsc(_d, 0, SEG_DESC_DATA_RW)
#define c3_dsc(_d) gdt_flat_dsc(_d, 3, SEG_DESC_CODE_XR)
#define d3_dsc(_d) gdt_flat_dsc(_d, 3, SEG_DESC_DATA_RW)

void set_seg_for_task(int id);
void init_flat_seg();
void print_gdt_content(gdt_reg_t gdtr_ptr);
void debug_gdt();

#endif
