#ifndef __USER_H__
#define __USER_H__

#include <process.h>
#include <memory_map.h>

void task0();
__attribute__((section(".user1"))) void task1();
__attribute__((section(".user2"))) void task2();

#endif