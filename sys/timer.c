#include <sys/irq.h>
#include <sys/timer.h>
#include <sys/sched.h>
#include <sys/console.h>

extern void outportb(uint16_t port, uint8_t val);

uint64_t counter=0;
int time=0;
int secs=0;
int hours=0;
int mins=0;

/* This will keep track of how many ticks that the system
*  has been running for */
int timer_ticks = 0;

void timer_set() {
    int divisor = 1193180 / HZ;
    outportb(0x43, 0x36);
    outportb(0x40, divisor & 0xFF);
    outportb(0x40, divisor >> 8);
}

/* Handles the timer. In this case, it's very simple: We
*  increment the 'timer_ticks' variable every time the
*  timer fires. By default, the timer fires 18.222 times
*  per second. Why 18.222Hz? Some engineer at IBM must've
*  been smoking something funky */
void timer_handler()
{
    /* Increment our 'tick count' */
    timer_ticks++;

    /* Every 18 clocks (approximately 1 second), we will
    *  display a message on the screen */
    /*if (timer_ticks % 18 == 0) {
        //printk("One second has passed\n");
    }*/
}

/* This will continuously loop until the given time has
*  been reached */
void timer_wait(int ticks)
{
    unsigned long eticks;

    eticks = timer_ticks + ticks;
    while(timer_ticks < eticks);
}

/* Sets up the system clock by installing the timer handler
*  into IRQ0 */
void timer_install()
{
    /* Installs 'timer_handler' to IRQ0 */
    irq_install_handler(0, timer_handler);
    
    timer_set();
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

void call_timer() {
    int x, y;
    char str_time[10];
    
    counter++;
    if((counter % 100) == 0) {
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
    /* Send an EOI to the master interrupt controller */
    outportb(0x20, 0x20);
    /* Every 18 clocks (approximately 1 second), we will
     * display a message on the screen */
    if (!scan_flag && (counter % 18 == 0)) {
        //printk("One second has passed\n");
        /* Wake any sleeping processes from wait queue */
        awake();
        /* Context switch */
        schedule();
    }
}

void div_zero() {
    printk("div zero\n");
}
