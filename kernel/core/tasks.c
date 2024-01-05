#include <tasks.h>
#include <debug.h>

void task0()
{
	for (;;)
	{
		// debug("Kernel reporting for duty\n");

		// Cheating :p
		asm volatile("int $0x20");
	}
}

__attribute__((section(".user1"))) void task1()
{
	uint32_t *counter = (uint32_t *)T1_SHARED_MEM;

	for (;;)
	{
		*counter += 1;

		// TODO
		// Comment the following line to test the real behavior
		asm volatile("int $0x20");
		// Hardware interupts doesn't work once I switch to ring3 (see NOTES.md)
		// I added this line to proove that even if the syscall is not triggered, it works!
		// I know... That's cheating :p
	}
}

__attribute__((section(".user2"))) void task2()
{
	uint32_t *counter = (uint32_t *)T2_SHARED_MEM;

	for (;;)
	{
		asm volatile("int $0x80" ::"a"(counter));

		// Cheating :p
		asm volatile("int $0x20");
	}
}