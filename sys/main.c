#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/sched.h>
#include <sys/tarfs.h>
#include <sys/timer.h>
#include <sys/memory.h>
#include <sys/tarfs.h>
#include <sys/console.h>
volatile int dbg = 0;

#define INITIAL_STACK_SIZE 4096
char stack[INITIAL_STACK_SIZE];
uint32_t* loader_stack;
extern char kernmem, physbase;
struct tss_t tss;

void start(uint32_t* modulep, void* physbase, void* physfree)
{
	/*
    struct smap_t {
		uint64_t base, length;
		uint32_t type;
	}__attribute__((packed)) *smap;
	while(modulep[0] != 0x9001) modulep += modulep[1]+2;
	for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
		if (smap->type == 1 && smap->length != 0) {
			printk("Available Physical Memory [%x-%x]\n", smap->base, smap->base + smap->length);
		}
	}
	printk("tarfs in [%p:%p]\n", &_binary_tarfs_start, &_binary_tarfs_end);
    */
	
	tarfs_initialize();
	/* initialize memory in pages */
	while(dbg);
	mem_init(physbase, physfree);
    
    /* Disable scan_flag by default */
    scan_flag = false;
    /* Clear Screen */
    reset();
    /* Preemptive schdeuler
     * Add two processes to task list
     */

    /*
    addTasktoQueue(create_process("bin/baz"));
    addTasktoQueue(create_process("bin/hello"));
    addTasktoQueue(create_process("bin/sbush"));
    addTasktoQueue(create_process("bin/w00t"));
    addTasktoQueue(create_process("bin/sleep"));
    addTasktoQueue(create_process("bin/foo"));
    addTasktoQueue(create_process("bin/bar"));
    addTasktoQueue(create_process("bin/qux"));
    */
	/* Moiz: User process init */
	addTasktoQueue(create_process("bin/idle"));
    addTasktoQueue(create_process("bin/sbush"));
    //printSchedulerQueue();
	init_process((uint64_t *)stack);

    schedule();
    printk("Die Kernel");
    /*
    init_tasks();
	__asm__ __volatile__ ("sti");
    start_task();
    */
    // kernel starts here
    while(1);
}


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
	reload_idt();
	start(
		(uint32_t*)((char*)(uint64_t)loader_stack[3] + (uint64_t)&kernmem - (uint64_t)&physbase),
		&physbase,
		(void*)(uint64_t)loader_stack[4]
	);
	s = "!!!!! start() returned !!!!!";
	for(v = (char*)0xb8000; *s; ++s, v += 2) *v = *s;
//    int a=5,b=0;
//    a=a/b;
    while(1);
}
