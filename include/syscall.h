#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <sys/defs.h>
#include <sys/syscall.h>

static __inline uint64_t syscall_0(uint64_t n) {
	uint64_t ret;
   	__asm volatile("movq %1,%%rax;"
                   "int $80;"
                   "movq %%rax,%0;"
                   : "=r"(ret)
                   : "r"(n)
    : "rax", "memory");
	
    return ret;
}

static __inline uint64_t syscall_1(uint64_t n, uint64_t a1) {
	uint64_t ret;
   	__asm volatile("movq %1,%%rax;"
                   "movq %2,%%rdi;"
                   "int $80;"
                   "movq %%rax,%0;"
                   : "=r"(ret)
                   : "r"(n),
                   "r"(a1)
    : "rax", "rdi", "memory");
	
    return ret;
}

static __inline uint64_t syscall_2(uint64_t n, uint64_t a1, uint64_t a2) {
    uint64_t ret;
   	__asm volatile("movq %1,%%rax;"
                   "movq %2,%%rdi;"
                   "movq %3,%%rsi;"
                   "int $80;"
                   "movq %%rax,%0;"
                   : "=r"(ret)
                   : "r"(n),
                   "r"(a1),
                   "r"(a2)
    : "rax", "rdi", "rsi", "memory");

    return ret;
}

static __inline uint64_t syscall_3(uint64_t n, uint64_t a1, uint64_t a2, uint64_t a3) {
    uint64_t ret;
   	__asm volatile("movq %1,%%rax;"
                   "movq %2,%%rdi;"
                   "movq %3,%%rsi;"
                   "movq %4,%%rdx;"
                   "int $80;"
                   "movq %%rax,%0;"
                   : "=r"(ret)
                   : "r"(n),
                   "r"(a1),
                   "r"(a2),
                   "r"(a3)
    : "rax", "rdi", "rsi", "rdx", "memory");

    return ret;
}

#endif
