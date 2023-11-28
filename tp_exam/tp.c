/* GPLv2 (c) Airbus */
#include <debug.h>

#include "seg.c"
#include "int.c"

void tp() {
	init_seg();
	debug("Segmentation OK.\n");

	init_syscall();
	debug("Syscalls OK.\n");
}
