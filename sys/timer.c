#include<sys/defs.h>
#include<sys/sbunix.h>
#include<sys/timer.h>
#include<stdio.h>
#include<stdlib.h>
extern void outportb(uint16_t port, uint8_t val);

uint64_t counter=0;
int secs=0;
int hours=0;
int mins=0;

void timer_set(){
    int freq=100;
    int divisor=1193180/freq;
    outportb(0x43,0x36);
    outportb(0x40,divisor & 0xFF);
    outportb(0x40, divisor >> 8);
     printk("divisor : %d\n",divisor);
}

void init_pic(){
   printk("PIC INIT \n");
   outportb(0x20, 0x11);
   outportb(0xA0, 0x11);
   outportb(0x21, 0x20);  
   outportb(0xA1, 0x28); 
   outportb(0x21, 0x04);
   outportb(0xA1, 0x02);
   outportb(0x21, 0x01);
   outportb(0xA1, 0x01);
   outportb(0x21, 0x0);
   outportb(0xA1, 0x0);
}
void call_timer(){
//    printk("timer : %d\n",counter);
    counter++;
    outportb(0x20,0x20);
    if((counter%100)==0){
//        secs=counter/100;
        secs++;
        hours=secs/3600; //hours since boot
    //calculating mins since boot  
        if(secs>=3600)
            mins=secs%3600;
        else if((secs>=60) && (secs<3600))
            mins=secs/60;
        else
            mins=0;
    //calculating secs since boot
        if(secs==60)
            secs=0;
        else if(secs>60)
            secs=secs%60;
//       if(secs%10==0) 
//       printk("Time since boot (hh:mm:ss):%d:%d:%d", hours,mins,secs);
    }
}
void div_zero(){
printk("div zero\n");
}
