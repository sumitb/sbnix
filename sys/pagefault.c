#include <sys/sched.h>
#include <sys/console.h>
#include <sys/memory.h>

void OOM() {
    printk("out of memory\n");
}

void protection_fault() {
	printk("protection fault\n");
}

void page_fault(uint64_t error){
	uint64_t *pte;
	uint64_t *pml4e_addr;
	uint64_t logical;
	uint64_t physical;

	__asm__ __volatile__ ("movq %%cr2, %0" : "=r"(logical));

	physical=mem_allocate();
	pml4e_addr=(uint64_t *)currentTask->pml4e_addr;
	pte=walk_pages(pml4e_addr,logical);
	if(*pte & PAGE_PRESENT)
		mem_free(*pte);
	*pte=(physical | PAGE_PERM);
}

