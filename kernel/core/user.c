#include <user.h>

__attribute__((section(".user"))) void task1()
{
	uint32_t *counter = (uint32_t *)T1_SHARED_MEM;

	for (;;)
	{
		*counter += 1;

		// TODO
		// Comment the following line to test the real behavior
		asm volatile("int $0x80" ::"a"(counter));
		// Because task switching is not working, I let this line uncommented
		// to see the behavior of task1
		// I know... That's cheating :/
	}
}

__attribute__((section(".user"))) void task2()
{
	uint32_t *counter = (uint32_t *)T2_SHARED_MEM;

	for (;;)
	{
		asm volatile("int $0x80" ::"a"(counter));
	}
}