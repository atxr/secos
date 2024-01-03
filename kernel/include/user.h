#ifndef __USER_H__
#define __USER_H__

#include <process.h>
#include <memory_map.h>

__attribute__((section(".user"))) void task1();
__attribute__((section(".user"))) void task2();

#endif