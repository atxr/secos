#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <pagemem.h>

#define PROCESS_MAX 3

typedef struct process_t
{
	int pid;
	int ppid;
	bool_t started;

	uint32_t esp;
	pde32_t *pgd;
	void *entry;
} process_t;

void init_process_table();
void create_process(process_t p);
void ack_and_run_user_process(int id);

process_t *get_process_list();
int get_process_list_len();
int get_current_process();
void set_current_process(int);

#endif