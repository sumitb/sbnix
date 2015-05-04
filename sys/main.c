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

void foo() {
    int cnt=0;
    while(1)
        printk("%d. Hello\n", ++cnt);
}

void bar() {
    int cnt=0;
    while(1)
        printk("%d. World\n", ++cnt);
}

void baz() {
    int cnt=0;
    while(1)
        printk("%d. Demon\n", ++cnt);
}

void qux() {
    int cnt=0;
    while(1)
        printk("%d. Lorde\n", ++cnt);
}

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
	mem_init(physbase, physfree);

    /* Preemptive schdeuler
     * Add two processes to task list
     * Debug: Print task run queue
     * Call sys_yield
     */

    //__asm volatile("callq handler_irq0");
	while(dbg);
   /* addTasktoQueue(initTask((uint64_t)&foo));
    addTasktoQueue(initTask((uint64_t)&bar));
    addTasktoQueue(initTask((uint64_t)&baz));
    addTasktoQueue(initTask((uint64_t)&qux));
*/	/* Moiz: User process init */
//	struct task_struct *task1=create_process("bin/foo");
//        addTasktoQueue(task1);
	struct task_struct *task=create_process("bin/hello");
	//struct task_struct *task=create_process("bin/sbush");
        addTasktoQueue(task);

    init_process((uint64_t *)stack);
//    schedule();
    printk("Die Kernel");
    
   // init_tasks();
//	__asm__ __volatile__ ("sti");
   // start_task();
    
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
//	 __asm__ __volatile__ ("sti");
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
