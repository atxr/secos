# secos notes

## What did I managed to do?

My implementation of secos is not quite complete. Actually, I didn't manage to implement task switching correctly.

When switching to ring3 with the `run_user_task` function, hardware interrupts stop working for no reason :/
Hence, I'm not able to trigger my switch syscall once in ring3.

For the sake of clarity, I added the `int 0x80` call to task1 in the loop to proove the `counter` syscall is working well. 
If you find out the reason of this bug, let me know...

## Memory map

The physical memory is designed as following:

0x0                                     0x400000                         0x800000                    0xc00000
======================================================================================================================================
|                                         |                                |                           |                             |
|  KERNEL SPACE (`PG_KRN` bit on pgd[0])  |        PROCESS 1 SPACE         |      PROCESS 2 SPACE      |        SHARED MEMORY        |
|                                         |                                |                           |                             |
|         Id mapped in all pgds           |       Id mapped in pgd1        |     Id mapped in pgd2     |  Mapped @ 0x800000 in pgd1  |
|                                         |       Not mapped in pgd2       |     Not mapped in pgd1    |  Mapped @ 0x400000 in pgd2  |
|                                         |                                |                           |    (Only the first page)|   |
|                                         |                                |                           |                             |
|    Kernel       Kernel       PGDs       |   Task1 code         Stack1    |  Task2 code      Stack2   |  Shared                     |
|     Code        Stack         &         |                                |                           |   page                      |
|                              PTBs       |                                |                           |                             |
|                                         |                                |                           |                             |
|                                         |                                |                           |                             |
======================================================================================================================================

## Thoughts on secos

This project was really intersterring, and I think I learnt a lot by designing an os from (almost) scratch.

Regarding segmentation, pagination and syscalls, it was quite straightforward to achieve considering the lectures and the TPs.
However, I must confess the IRQ0 setup (with outb/intb) was quite challenging, and might deserve a little explanation during the lectures.

