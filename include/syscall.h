#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <sys/defs.h>
#include <sys/syscall.h>

static __inline uint64_t syscall_0(uint64_t n) {
	int64_t ret;
	__asm volatile("syscall"
		: "=a" (ret)
		: "i" (0x80),
		"a" (n),
		"D" (0)
		: "cc","rcx","r11","memory");

	return 0;
}

static __inline uint64_t syscall_1(uint64_t n, uint64_t a1) {
	int64_t ret;
        __asm volatile("syscall"
                : "=a" (ret)
                : "i" (0x80),
                "a" (n),
                "D" (a1),
                "S" (0)
		: "cc","rcx","r11","memory");

	return ret;
}

static __inline uint64_t syscall_2(uint64_t n, uint64_t a1, uint64_t a2) {
        int64_t ret;
        __asm volatile("syscall"
                : "=a" (ret)
                : "i" (0x80),
                "a" (n),
                "D" (a1),
                "S" (a2)
		: "cc","rcx","r11","memory");

        return ret;
}

static __inline uint64_t syscall_3(uint64_t n, uint64_t a1, uint64_t a2, uint64_t a3) {
        int64_t ret;
        __asm volatile("syscall"
                : "=a" (ret)
                : "i" (0x80),
                "a" (n),
                "d" (a3),
                "D" (a1),
                "S" (a2)
		: "cc","rcx","r11","memory");

        return ret;
}

#endif
