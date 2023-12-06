#include <debug.h>
#include <pagemem.h>
#include <cr.h>

#ifndef PAGIN_H
#define PAGIN_H

#define SHARED_MEM	0x410000

#define T1_PGD		0x380000
#define T1_PTB0		0x381000
#define T1_PTB1		0x382000

#define T2_PGD		0x390000
#define T2_PTB0		0x391000
#define T2_PTB1		0x392000

static pde32_t *pgd1 = (pde32_t*) T1_PGD;
static pte32_t *ptb10 = (pte32_t*) T1_PTB0;
static pte32_t *ptb11 = (pte32_t*) T1_PTB1;
	
static pde32_t *pgd2 = (pde32_t*) T2_PGD;
static pte32_t *ptb20 = (pte32_t*) T2_PTB0;
static pte32_t *ptb21 = (pte32_t*) T2_PTB1;

#endif

void trigger1() {}

// Init PGD/PTB and activate pagination for task1
// 
// Task 1:
// - Kernel id mapped
// - Task1 id mapped 0x400000 - 0x800000
//
// Task 2:
// - Kernel id mapped
// - Task2 id mapped 0x800000 - 0x1000000
void init_pagination() {
	__clear_page(pgd1);
	__clear_page(ptb10);
	__clear_page(ptb11);
	__clear_page(pgd2);
	__clear_page(ptb20);
	__clear_page(ptb21);

	// set two ptb for id mapped areas
	for(int i=0;i<1024;i++) {
	 	pg_set_entry(&ptb10[i], PG_KRN|PG_RW, i);
	 	pg_set_entry(&ptb11[i], PG_USR|PG_RW, 1024 + i);
	 	
	 	pg_set_entry(&ptb20[i], PG_KRN|PG_RW, i);
	 	pg_set_entry(&ptb21[i], PG_USR|PG_RW, 2*1024 + i);
	}
	
	debug("MEGATETS %d\n", PG_USR|PG_RW);

	// set ptbs
	pg_set_entry(&pgd1[0], PG_KRN|PG_RW, page_nr(ptb10));
	pg_set_entry(&pgd1[1], PG_USR|PG_RW, page_nr(ptb11));
	
	pg_set_entry(&pgd2[0], PG_KRN|PG_RW, page_nr(ptb20));
	pg_set_entry(&pgd2[2], PG_USR|PG_RW, page_nr(ptb21));
	
	trigger1();

	// set cr3 and cr0
	set_cr3((uint32_t)pgd1);

	uint32_t cr0 = get_cr0();
	set_cr0(cr0 | CR0_PG);

	debug("============== PAGINATION ACTIVATED FOR TASK 1 ===============\n");
}

void debug_pagination(int address) {
	int pdeidx = pd32_idx(address);
	int pteidx = pt32_idx(address);
	
	cr3_reg_t cr3 = {.raw = get_cr3()};
	pde32_t* pde = (pde32_t*) (cr3.addr << 12);
	pte32_t* pte = (pte32_t*) (pde[pdeidx].addr << 12);

	int pdelvl = pde[pdeidx].lvl;
	int ptelvl = pte[pteidx].lvl;
	debug("Pagination privilege lvl: PGD#%d:%d and PTB#%d:%d\n", pdeidx, pdelvl, pteidx, ptelvl);
}
