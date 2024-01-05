#include <segmem.h>
#include <debug.h>
#include <memory_map.h>

seg_desc_t GDT[8];
tss_t TSS0;
tss_t TSS1;
tss_t TSS2;

void set_seg_for_task(int pid)
{
    if (pid > 0)
    {
        set_ds(d3_sel);
        set_es(d3_sel);
        set_fs(d3_sel);
        set_gs(d3_sel);

        if (pid == 1)
        {
            TSS0.s0.esp = KERNEL_STACK1;
        }
        else
        {
            TSS0.s0.esp = KERNEL_STACK2;
        }
    }

    else
    {
        set_ds(d0_sel);
        set_es(d0_sel);
        set_fs(d0_sel);
        set_gs(d0_sel);
    }
}

void init_flat_seg()
{
    gdt_reg_t gdtr;

    GDT[0].raw = 0ULL;

    c0_dsc(&GDT[c0_idx]);
    d0_dsc(&GDT[d0_idx]);
    c3_dsc(&GDT[c3_idx]);
    d3_dsc(&GDT[d3_idx]);
    tss_dsc(&GDT[ts0_idx], (offset_t)&TSS0);
    tss_dsc(&GDT[ts1_idx], (offset_t)&TSS1);
    tss_dsc(&GDT[ts2_idx], (offset_t)&TSS2);

    gdtr.desc = GDT;
    gdtr.limit = sizeof(GDT) - 1;
    set_gdtr(gdtr);

    set_cs(c0_sel);

    set_ss(d0_sel);
    set_ds(d0_sel);
    set_es(d0_sel);
    set_fs(d0_sel);
    set_gs(d0_sel);

    TSS0.s0.esp = KERNEL_STACK1;
    TSS0.s0.ss = d0_sel;

    set_tr(ts0_sel);
}

void print_gdt_content(gdt_reg_t gdtr_ptr)
{
    seg_desc_t *gdt_ptr;
    gdt_ptr = (seg_desc_t *)(gdtr_ptr.addr);
    int i = 0;
    while ((uint32_t)gdt_ptr < ((gdtr_ptr.addr) + gdtr_ptr.limit))
    {
        uint32_t start = gdt_ptr->base_3 << 24 | gdt_ptr->base_2 << 16 | gdt_ptr->base_1;
        uint32_t end;
        if (gdt_ptr->g)
        {
            end = start + ((gdt_ptr->limit_2 << 16 | gdt_ptr->limit_1) << 12) + 4095;
        }
        else
        {
            end = start + (gdt_ptr->limit_2 << 16 | gdt_ptr->limit_1);
        }
        debug("%d ", i);
        debug("[0x%x ", start);
        debug("- 0x%x] ", end);
        debug("seg_t: 0x%x ", gdt_ptr->type);
        debug("desc_t: %d ", gdt_ptr->s);
        debug("priv: %d ", gdt_ptr->dpl);
        debug("present: %d ", gdt_ptr->p);
        debug("avl: %d ", gdt_ptr->avl);
        debug("longmode: %d ", gdt_ptr->l);
        debug("default: %d ", gdt_ptr->d);
        debug("gran: %d ", gdt_ptr->g);
        debug("\n");
        gdt_ptr++;
        i++;
    }
}

void debug_gdt()
{
    gdt_reg_t gdtr_ptr;
    get_gdtr(gdtr_ptr);
    print_gdt_content(gdtr_ptr);
}