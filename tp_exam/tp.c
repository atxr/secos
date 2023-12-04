/* GPLv2 (c) Airbus */
#include <debug.h>

#include "seg.c"
#include "int.c"
#include "tasks.c"
#include "pagin.c"

void tp() {
	init_flat_seg();
	debug("Segmentation OK.\n");

	init_syscall();
	debug("Syscalls OK.\n");

	init_pagination();
	debug("Pagination OK.\n");
}
