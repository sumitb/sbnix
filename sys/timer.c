#include<sys/timer.h>
#include<sys/console.h>

extern void outportb(uint16_t port, uint8_t val);

uint64_t counter=0;
int time=0;
int secs=0;
int hours=0;
int mins=0;

void timer_set(){
    int divisor=1193180/100;
    outportb(0x43,0x36);
    outportb(0x40,divisor & 0xFF);
    outportb(0x40, divisor >> 8);
    //printk("divisor : %d\n",divisor);
}

void init_pic(){
   /*printk("PIC INIT \n");
   printk("PIC INIT \r");
   printk("INIT \n");
   printk("123P213IC INIT \t 123213\n");
   printk("PIC\b INIT \n");*/
   
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

void fillTimer(char* str, int num) {
    int temp = 0;

    if(num <= 9) {
        *str = '0'; str++;
        *str = num + 48; str++;
    }
    else {
        temp = (num % 10) + 48;
        *str = (num / 10) + 48; str++;
        *str = temp; str++;
    }
    return;
}

void call_timer(){
    int x, y;
    char str_time[10];
    
    counter++;
    outportb(0x20,0x20);
    if((counter%100)==0){
        time++;
        hours = time/3600;                // hours since boot
        mins = (time-(hours*3600))/60;    // mins since boot
        secs = time%60;                   // secs since boot
        // Reset hours after a day
        hours %= 24;
        fillTimer(str_time, hours);
        str_time[2] = ':'; str_time[3] = ':';
        fillTimer(str_time+4, mins);
        str_time[6] = ':'; str_time[7] = ':';
        fillTimer(str_time+8, secs);
        str_time[10] = '\0';
        // Get current cursor position
        x = getcsr_x(); y = getcsr_y();
        // Move to bottom right corner
        gotoxy(66, 22);
        printk("%s", str_time);
        // Reset cursor position
        gotoxy(x, y);
    }
}

void div_zero() {
    printk("div zero\n");
}
