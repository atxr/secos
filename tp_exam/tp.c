/* GPLv2 (c) Airbus */
#include <debug.h>

#include "seg.c"
#include "int.c"

void tp() {
	init_seg();
	init_syscall();
}
