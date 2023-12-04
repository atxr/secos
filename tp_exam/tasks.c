#include <debug.h>

int current_task = 1;

void switch_task() {
	int new_task = 3 - current_task;
	debug("Switching to task %d\n", new_task);

	// Update CR3
	// TODO
	
	// Flush TLB
	// TODO
}

__attribute__((section(".user"))) int task1() {
	debug("Running task1\n");
	for(;;) {}
}

__attribute__((section(".user"))) int task2() {
	debug("Running task2\n");
	for(;;) {}
}
