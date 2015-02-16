#include<sys/defs.h>
#include<stdlib.h>
#include<syscall.h>
#include<stdio.h>
#define BLOCK_SIZE 20;

typedef enum { false, true } bool;

struct node{
    size_t size;
    bool flag;
    struct node *next;
   
};

struct node *malloc_head=NULL,*malloc_tail=NULL,*start;

int set=0;
void* malloc(size_t size){
       struct node *end,*temp;
        if(malloc_head==NULL){
        start=(struct node *)(syscall_1(12,0));
       start=(struct node *) syscall_1(12,(((uint64_t)start)+size+(sizeof(struct node))));
        start->size=size;
        start->flag=true;
        start->next=NULL;
        malloc_head=start;
        temp=start;
        }
        else{

        malloc_tail=malloc_head;
        while(malloc_tail!=NULL){
            if(malloc_tail->flag==false && malloc_tail->size>=size){
                set=1;
                break;
            }
            malloc_tail=malloc_tail->next;
        }
        if(set==0){
             end=(struct node *)(syscall_1(12,0));
           end=(struct node *) syscall_1(12,(((uint64_t)end)+size+(sizeof(struct node))));
            start->next=end;
            start=end;
            start->size=size;
            start->flag=true;
            start->next=NULL;
            temp=start;
        }
        else{
            set=0;
            malloc_tail->flag=true;
            malloc_tail->size=size;    //check whether to modify size or not
            temp=malloc_tail;
       //     printf("using free block");
        }
        }
    return temp+1;
}


void free(void *ptr){

   struct node *ptr1=malloc_head;
    struct node *ptr2=((struct node *)ptr)-1;
    while(ptr1!=ptr2){
        ptr1=ptr1->next;
     }
    if(ptr1==ptr2){
        ptr1->flag=false;   
    
    }




}
