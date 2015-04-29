#include <stdlib.h>
#include <syscall.h>
#include <sys/defs.h>
#include <sys/syscall.h>

struct node{
    size_t size;
    bool flag;
    struct node *next;
};

int set=0;
struct node *malloc_head=NULL;//, *malloc_tail=NULL, *start, *end;

void* malloc(size_t size)
{
    uint64_t start;
    // Naive allocator
    start = syscall_1(SYS_brk, 0);
   start = 0xffffff0f80000000;
    syscall_1(SYS_brk, ((uint64_t)start + size ));
    return (void*)start;
}


/*void* malloc(size_t size)
{
    struct node *temp;
    if(malloc_head == NULL) {
        start = (struct node *) syscall_1(SYS_brk, 0);
        end = (struct node *) syscall_1(SYS_brk, ((uint64_t)start + size + sizeof(struct node)));
        start->size = size;
        start->flag = true;
        start->next = end;
        malloc_head = start;
        temp = start;
        start = end;
    }
    else {
        malloc_tail = malloc_head;
        do {
            if(malloc_tail->flag == false && malloc_tail->size >= size) {
                set=1;
                break;
            malloc_tail = malloc_tail->next;
            }
        } while(malloc_tail != NULL);
    
        if(set == 0) {
            // end=(struct node *)(syscall_1(12,0));
            end= (struct node *) syscall_1(SYS_brk, ((uint64_t)end + size + sizeof(struct node)));
            // start->next=end;
            // start=end;
            start->size = size;
            start->flag = true;
            start->next = end;
            temp = start;
            start = end;
        }
        else {
            set = 0;
            malloc_tail->flag = true;
            malloc_tail->size = size;    //check whether to modify size or not
            temp = malloc_tail;
        }
    }
    return temp+1;
}*/


void free(void *ptr)
{
    struct node *ptr1 = malloc_head;
    struct node *ptr2 = ((struct node *)ptr) - 1;
    while(ptr1 != ptr2) {
        ptr1 = ptr1->next;
    }
    if(ptr1 == ptr2) {
        ptr1->flag = false;
    }
}
