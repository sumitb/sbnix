#include <sys/sbunix.h>
#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/tarfs.h>
#include <sys/timer.h>
#include <sys/memory.h>

void start(uint32_t* modulep, void* physbase, void* physfree)
{
	struct smap_t {
		uint64_t base, length;
		uint32_t type;
	}__attribute__((packed)) *smap;
	while(modulep[0] != 0x9001) modulep += modulep[1]+2;
	for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
		if (smap->type == 1 /* memory */ && smap->length != 0) {
	//		printk("Available Physical Memory [%x-%x]\n", smap->base, smap->base + smap->length);
		}
	}
	
	//initialize memory in pages
	mem_init(physbase,physfree);
//	printk("tarfs in [%p:%p]\n", &_binary_tarfs_start, &_binary_tarfs_end);
    //printk("Are interrupts enabled: %d", are_interrupts_enabled());
    // kernel starts here
    while(1);
}

#define INITIAL_STACK_SIZE 4096
char stack[INITIAL_STACK_SIZE];
uint32_t* loader_stack;
extern char kernmem, physbase;
struct tss_t tss;
volatile int gdb=0;

void boot(void)
{
	// note: function changes rsp, local stack variables can't be practically used
	register char *s, *v;
	__asm__(
		"movq %%rsp, %0;"
		"movq %1, %%rsp;"
		:"=g"(loader_stack)
		:"r"(&stack[INITIAL_STACK_SIZE])
	);
	reload_gdt();

	setup_tss();
	while(gdb);
	reload_idt();
        init_pic();
        timer_set();
	//__asm__ __volatile__ ("sti");
//	__asm volatile("callq handler_irq0");
	start(
		(uint32_t*)((char*)(uint64_t)loader_stack[3] + (uint64_t)&kernmem - (uint64_t)&physbase),
		&physbase,
		(void*)(uint64_t)loader_stack[4]
	);
	s = "!!!!! start() returned !!!!!";
	for(v = (char*)0xb8000; *s; ++s, v += 2) *v = *s;
    int a=5,b=0;
    a=a/b;
    while(1);
}
