#include <string.h>
#include <sys/elf.h>
#include <sys/tarfs.h>
#include <sys/sched.h>
#include <sys/memory.h>
#include <sys/console.h>

static uint64_t avail_pid = 0;
struct task_struct task1, task2, task3, task4;

uint64_t getCR3() {
    uint64_t cr3_addr = 0;
    
    /* Save current cr3 value */
    __asm__ __volatile__("movq %%cr3, %0":: "b"(cr3_addr));
    return cr3_addr;
}

/* Prepare kernel stack for each process
 * Save sp registers and rip
*/
void prepKernelStack(uint64_t* stack, uint64_t rip) {
    stack[KERNEL_STACK_SIZE - 1] = rip;
    /*
    stack[KERNEL_STACK_SIZE - 1] = 0x23;
    stack[KERNEL_STACK_SIZE - 2] = (uint64_t) &foo;
    stack[KERNEL_STACK_SIZE - 3] = 0x246;
    stack[KERNEL_STACK_SIZE - 4] = 0x1b;
    stack[KERNEL_STACK_SIZE - 5] = rip;
    task1.kernel_rsp = &task1.kern_stack[KERNEL_STACK_SIZE - 5];
    task2.kernel_rsp = &task2.kern_stack[KERNEL_STACK_SIZE - 22];
    */
}

struct task_struct* initTask(uint64_t entry_point) {
    /* Allocate memory for a new task_struct */
    /* TODO: kmalloc issue */
    //struct task_struct* task = (struct task_struct*)kmalloc(sizeof(struct task_struct));
    struct task_struct* task;
    if(avail_pid == 0)
        task = &task1;
    else if(avail_pid == 1)
        task = &task2;
    else if(avail_pid == 2)
        task = &task3;
    else
        task = &task4;
    task->pid = ++avail_pid;
    task->state = 0;
    task->parent = NULL;
    //task->childList = ####; /* define in sched.h and init here*/
    //task-> mm = get_mm();
    /* define get_mm in memory.h */
    task->mm = (struct mm_struct *)((char *)(&task + 1)); /* COPY: from process.c */
    
    /* Init kernel stack */
    //task.stack = (uint64_t*)page_addr;
    prepKernelStack(task->kern_stack, entry_point);
    /* 15 regs, 1 retq, 1 offset*/
    task->kernel_rsp = (uint64_t*) &task->kern_stack[KERNEL_STACK_SIZE - 1 - NUM_REGISTERS_SAVED];
    
    /* Init user stack for user processes only*/
    
    task->pml4e_base_addr = getCR3();

    return task;
}

volatile int gdb=0;

struct run_queue * create_process(char *binary){
	uint64_t *page_addr;
	uint64_t *pml4e_pr;
	page_addr=(uint64_t *)mem_allocate();
	struct run_queue *proc=(struct run_queue *)(KERN_MEM + mem_allocate());
	//allocate 3 pages for process struct, change this later
	mem_allocate();
	mem_allocate();
	proc->next=NULL;
	struct run_queue *curr=rq_head;
	if(curr != NULL){
	while(curr->next != NULL)
		curr=curr->next;
	curr->next=proc;
	}
	else{
		curr=proc;
	}
	pml4e_pr=(uint64_t *)((uint64_t)page_addr + KERN_MEM);
	memset(pml4e_pr,0,4096);
	
	pml4e_pr[511]=pml4e[511];
	
	proc->process.cr3_address=(uint64_t)page_addr;
	proc->process.process_id=proc_cnt++;
	proc->process.pml4e_addr=(uint64_t)pml4e_pr;
	
	proc->process.mm=(struct mm_struct *)((char *)(&proc->process + 1));
	proc->process.mm->cnt=0;
	proc->process.mm->vma_addr=NULL;
	
	//proc->process.stack[63]= GDT_DS | P | W | DPL3,  /*** user data segment descriptor ***/
	//proc->process.stack[62]= (uint64_t)(&proc->process.stack[63]);
	//proc->process.stack[61]= 0x246;                           //  EFlags
//	proc->process.stack[60]= GDT_CS | P | DPL3 | L,  /*** user code segment descriptor ***/
	
	//proc->process.rsp_ptr = (uint64_t)(&proc->process.stack[63]);
	
        /*proc->process.kstack[506] = 0;
		proc->process.kstack[505] = 0; 
		proc->process.kstack[504] = 0; 
		proc->process.kstack[503] = 0;
        proc->process.kstack[502] = 0; 
		proc->process.kstack[501] = 0; 
		proc->process.kstack[500] = 0; 
		proc->process.kstack[499] = 0;
        proc->process.kstack[498] = 0;
		proc->process.kstack[497] = 0;
		proc->process.kstack[496] = 0; 
		proc->process.kstack[495] = 0;
        proc->process.kstack[494] = 0; 
		proc->process.kstack[493] = 0; 
		proc->process.kstack[492] = 0;
*/
		memset(proc->process.kstack,'\0',4096);
		//proc->process.kstack[491] = (uint64_t)(&irq0+34);
        proc->process.rsp_ptr = (uint64_t)&proc->process.kstack[490];

		proc->process.kstack[511] = 0x23 ;                              //SS    
        proc->process.kstack[510] = (uint64_t)(&proc->process.stack[511]);      //  ESP
        proc->process.kstack[509] = 0x246;                           // EFLAGS
        proc->process.kstack[508] = 0x1b ;                           //CS
        
		elf_load(&(proc->process), binary);	
        //proc->process.heap_vma->vm_end = proc->process.heap_vma->vm_start;
		proc->process.kstack[507] = (uint64_t)proc->process.entry_pt;  //RIP
//	__asm __volatile("movq %0,%%cr3" : : "r" (cr3_addr));
	running_proc=proc;
	return proc;
}

void init_process(uint64_t *stack){
	//kernel.stack=(uint64_t)&(stack[0]);
	kernel.process_id=proc_cnt++;
	kernel.cr3_address=cr3_addr;
	kernel.pml4e_addr=(uint64_t)pml4e;
	
	struct run_queue *proc = create_process("bin/hello");
	while(gdb);
	__asm __volatile("movq %0, %%cr3":: "a"(proc->process.cr3_address));
	//__asm __volatile("movq %0,%%cr3" : : "r" (proc->process.cr3_address));
	__asm__ __volatile__ (
            "movq %0, %%rsp;" //load next's stack in rsp
            :
            :"r"(proc->process.rsp_ptr)
    );
	tss.rsp0 = (uint64_t)&(proc->process.stack[511]);
	__asm__ __volatile__("popq %r15");
	__asm__ __volatile__("popq %r14");
	__asm__ __volatile__("popq %r13");
	__asm__ __volatile__("popq %r12");
	__asm__ __volatile__("popq %r11");
	__asm__ __volatile__("popq %r10");
	__asm__ __volatile__("popq %r9");
	__asm__ __volatile__("popq %r8");
	__asm__ __volatile__("popq %rdi");
	__asm__ __volatile__("popq %rsi");
	__asm__ __volatile__("popq %rdx");
	__asm__ __volatile__("popq %rcx");
	__asm__ __volatile__("popq %rbx");
	__asm__ __volatile__("popq %rax");

	__asm__ __volatile__ ("mov $0x2b,%ax");
  	__asm__ __volatile__ ("ltr %ax");
	__asm__ __volatile__ ("add $8, %rsp");
	__asm__ __volatile__ ("add $8, %rsp");
	__asm__ __volatile__ ("add $8, %rsp");
	__asm__ __volatile__("iretq");
}

