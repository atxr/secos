# secos notes

## What did I manage to do?

My implementation of secos is not quite complete.

When switching to ring3 with the `run_user_task` function, hardware interrupts stop working for no reason :/
Hence, I cannot trigger my switch syscall once in ring3.

For the sake of clarity, I added the `int 0x80` call to every task to manually trigger the hardware interrupt.
If you find out the reason for this bug, let me know...

## Memory map

The physical memory is designed as follows:

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
|     Code        stacks        &         |                                |                           |   page                      |
|                              PTBs       |                                |                           |                             |
|                                         |                                |                           |                             |
|                                         |                                |                           |                             |
======================================================================================================================================

For the pagination, there are three different PGD (ont per process).
- PGD0 for the kernel process, which identity maps 0x0 to 0x400000 with kernel rights only 
- PGD1 for the process 1, which identity maps 0x0 to 0x400000 with kernel rights, 0x400000 to 0x800000 with user writes and maps the physical address 0xc00000 to 0x800000 for the shared page
- PGD1 for the process 1, which identity maps 0x0 to 0x400000 with kernel rights, 0x800000 to 0xc00000 with user writes and maps the physical address 0xc00000 to 0x400000 for the shared page

Regarding the kernel stacks, I dedicated three different areas. When switching to a user task, I change the value of esp0 in the TSS to the corresponding stack.
Hence, saved stacks do not overwrite themself.

## Thoughts on secos

This project was really interesting, and I learned a lot by designing an OS from (almost) scratch.

It was quite straightforward to achieve segmentation, pagination and syscalls, considering the lectures and the TPs.
However, I must confess the IRQ0 setup (with outb/intb) was quite challenging, and might deserve a little explanation during the lectures.

