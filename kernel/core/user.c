#include <user.h>

__attribute__((section(".user"))) void task1()
{
	uint32_t *counter = (uint32_t *)T1_SHARED_MEM;

	for (;;)
	{
		*counter += 1;
		asm volatile("int $0x80" ::"a"(counter));
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