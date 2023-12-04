#include <debug.h>
#include <pagemem.h>
#include <cr.h>

#define T1_CNT	0x400000
#define T1_PGD	0x401000
#define T1_PTB0	0x402000
#define T1_PTB1	0x403000
#define T1_BASE	0x404000

#define T2_CNT	0x800000
#define T2_PGD	0x801000
#define T2_PTB0	0x802000
#define T2_PTB1	0x803000
#define T2_BASE	0x804000

int current_task = 1;

void switch_task() {
	int new_task = 3 - current_task;

	// Update CR3
	uint32_t new_pgd = new_task == 1 ? T1_PGD : T2_PGD;
	set_cr3((uint32_t)new_pgd);
	
	// Flush TLB
	// TODO
}

// Set pagination for task1
// One pgd identity mapped
// Identity mapped kernel
// Identity mapped user task
// Question:
// - Multiple ptb?
// - Id map pgd?
void init_pagination() {
	pde32_t *pgd = (pde32_t*) T1_PGD;
	__clear_page(pgd);

	pte32_t *ptb10 = (pte32_t*) T1_PTB0;
	pte32_t *ptb11 = (pte32_t*) T1_PTB1;

	pte32_t *ptb20 = (pte32_t*) T2_PTB0;
	pte32_t *ptb21 = (pte32_t*) T2_PTB1;

	// set two ptb for id mapped areas
	for(int i=0;i<1024;i++) {
	 	pg_set_entry(&ptb10[i], PG_KRN|PG_RW, i);
	 	pg_set_entry(&ptb11[i], PG_KRN|PG_RW, 1024 + i);
	 	pg_set_entry(&ptb20[i], PG_KRN|PG_RW, 2*1024 + i);
	 	pg_set_entry(&ptb21[i], PG_KRN|PG_RW, 3*1024 + i);
	}

	// set ptbs
	pg_set_entry(&pgd[0], PG_KRN|PG_RW, page_nr(ptb10));
	pg_set_entry(&pgd[1], PG_KRN|PG_RW, page_nr(ptb11));
	pg_set_entry(&pgd[2], PG_KRN|PG_RW, page_nr(ptb20));
	pg_set_entry(&pgd[2], PG_KRN|PG_RW, page_nr(ptb21));

	// set cr3 and cr0
	set_cr3((uint32_t)pgd);

	uint32_t cr0 = get_cr0();
	set_cr0(cr0 | CR0_PG);

	debug("============== PAGINATION ACTIVATED ===============\n");
}

__attribute__((section(".user"))) int task1() {
	debug("Running task1\n");
	for(;;) {}
}

__attribute__((section(".user"))) int task2() {
	debug("Running task2\n");
	for(;;) {}
}
