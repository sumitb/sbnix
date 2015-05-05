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

/*static __inline uint64_t syscall_1(uint64_t n, uint64_t a1) {
	uint64_t ret;
   	__asm volatile("movq %1,%%rax;"
                   "movq %2,%%rbx;"
                   "int $80;"
                   "movq %%rax,%0;"
                   : "=r"(ret)
                   : "r"(n),
                   "r"(a1)
    : "rax", "rbx", "memory");
	
    return ret;
}
*/
static __inline uint64_t syscall_1(uint64_t n, uint64_t a1) {
        uint64_t ret;
        __asm volatile("movq %1,%%rax;"
		"movq %2,%%rbx\n\t"
		"int $80\n\t"
                   "movq %%rax,%0\n\t;"
                   :"=r"(ret):"r"(n),"r"(a1):"rax","rbx","memory");
        return ret;
}

/*static __inline uint64_t syscall_2(uint64_t n, uint64_t a1, uint64_t a2) {
    uint64_t ret;
   	__asm volatile("movq %1,%%rax;"
                   "movq %2,%%rbx;"
                   "movq %3,%%rdx;"
                   "int $80;"
                   "movq %%rax,%0;"
                   : "=r"(ret)
                   : "r"(n),
                   "r"(a1),
                   "r"(a2)
    : "rax", "rbx", "rdx", "memory");

    return ret;
}*/
static __inline uint64_t syscall_2(uint64_t n, uint64_t a1, uint64_t a2) {
    uint64_t ret;
        __asm volatile("movq %1,%%rax;"
		"movq %2,%%rbx\n\t"
                   "movq %3,%%rsi\n\t"
                   "int $80\n\t"
                   "movq %%rax,%0\n\t;"
		   :"=r"(ret):"r"(n),"r"(a1),"r"(a2):"rax","rbx","rsi","memory");
	return ret;
}


static __inline uint64_t syscall_3(uint64_t n, uint64_t a1, uint64_t a2, uint64_t a3) {
    uint64_t ret;
   	__asm volatile("movq %1,%%rax;"
                   "movq %2,%%rbx;"
                   "movq %3,%%rsi;"
                   "movq %4,%%rdx;"
                   "int $80;"
                   "movq %%rax,%0;"
                   : "=r"(ret)
                   : "r"(n),
                   "r"(a1),
                   "r"(a2),
                   "r"(a3)
    : "rax", "rbx", "rsi", "rdx", "memory");

    return ret;
}

#endif
