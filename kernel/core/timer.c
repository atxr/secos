#include <timer.h>

void init_timer()
{
	uint32_t f = FOSC / 1000; // divide by 1Hz to trigger each second

	// set timer freq to 1kHz
	outb(0x34, 0x43);
	outb(f & 0xFF, 0x40);
	outb(f >> 8, 0x40);

	// demask irq0
	outb(inb(0x21) & 0xfe, 0x21);
}