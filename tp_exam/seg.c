#include <debug.h>
#include <segmem.h>
#include <string.h>

#define tss_dsc(_dSc_,_tSs_)                                            \
   ({                                                                   \
      raw32_t addr    = {.raw = _tSs_};                                 \
      (_dSc_)->raw    = sizeof(tss_t);                                  \
      (_dSc_)->base_1 = addr.wlow;                                      \
      (_dSc_)->base_2 = addr._whigh.blow;                               \
      (_dSc_)->base_3 = addr._whigh.bhigh;                              \
      (_dSc_)->type   = SEG_DESC_SYS_TSS_AVL_32;                        \
      (_dSc_)->p      = 1;                                              \
   })

seg_desc_t my_gdt[5];
int gdt_idx;

void add_descriptor(int base, int limit, int type, char dpl) {
    // limit
    my_gdt[gdt_idx].limit_1 = (uint16_t) limit;	 	// bits 00-15
    my_gdt[gdt_idx].limit_2 = (limit >> 16) & (1 << 4); // bits 16-19

    // base
    my_gdt[gdt_idx].base_1 = (uint16_t) base;	 	// bits 00-15
    my_gdt[gdt_idx].base_2 = (uint8_t) (base >> 16); 	// bits 16-23
    my_gdt[gdt_idx].base_2 = (uint8_t) (base >> 24); 	// bits 24-31

    my_gdt[gdt_idx].type = type;      /* segment type */
    my_gdt[gdt_idx].dpl = dpl;        /* descriptor privilege level */

    my_gdt[gdt_idx].s = 1;            /* descriptor type */
    my_gdt[gdt_idx].p = 1;            /* segment present flag */
    my_gdt[gdt_idx].avl = 1;          /* available for fun and profit */
    my_gdt[gdt_idx].l = 0;            /* longmode */
    my_gdt[gdt_idx].d = 1;            /* default length, depend on seg type */
    my_gdt[gdt_idx].g = 1;            /* granularity */

    gdt_idx++;
}

void init_seg() {
    // fill gdt
    my_gdt[gdt_idx++].raw = 0ULL;
    add_descriptor(0, 0xfffff, 11, 0);
    add_descriptor(0, 0xfffff,  3, 0);

    add_descriptor(0, 0xfffff, 11, 3);
    add_descriptor(0, 0xfffff,  3, 3);

    // set gdtr
    gdt_reg_t my_gdtr;
    my_gdtr.addr = (long unsigned int)my_gdt;
    my_gdtr.limit = sizeof(my_gdt) - 1;
    set_gdtr(my_gdtr);

    // set descriptors
    set_ds(gdt_usr_seg_sel(5));
    set_es(gdt_usr_seg_sel(5));
    set_fs(gdt_usr_seg_sel(5));
    set_gs(gdt_usr_seg_sel(5));

    // set ss
    // TODO CHECK HERE DUNNO WHAT IM DOING
    tss_t TSS;
    TSS.s0.esp = get_ebp();
    TSS.s0.ss  = gdt_krn_seg_sel(2);
    tss_dsc(&my_gdt[6], (offset_t)&TSS);
    set_tr(gdt_krn_seg_sel(6));
 
    // set cs
    // cf tp3
}
