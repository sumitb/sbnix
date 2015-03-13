#include <sys/defs.h>
#include <stdarg.h>
#define A_VIDEO (char*)0xb8000
#define intSize     20      // We don't expect any number to be greater than 2^64

//GLOBALS
//char *av_vid = A_VIDEO;       // Current addr of video buffer
int csr_x = 0, csr_y = 0;     // Current cursor position

/*
 * value of a_vid = 0xb80000
 * increment it by 1 byte
 */

/* Resets the global addr of video buffer to start */
// TODO: Write clear func and test it's functionality
void reset() {
    //av_vid = A_VIDEO;
    return;
 } 
/*
void mov_csr(void){
unsigned blank;
temp = csr_y * 80 + csr_x;
outportb(0x3D4, 14);
outportb(0x3D5, temp >> 8);
outportb(0x3D4, 15);
outportb(0x3D5, temp);
}*/

int getcsr_x() {
    return csr_x;
}
int getcsr_y() {
    return csr_y;
}
void gotoxy(int x, int y) {
    csr_y = y;
    csr_x = x;
    return;
}

/* Print a single char to video buffer */
void c_printf(char ch) {
    int offset = 0;
    char *av_vid = A_VIDEO;
    
    if(ch == '\b') {
        csr_x--;
        c_printf(' ');
        csr_x--;
    }    
    else if(ch == '\n') {
        csr_x = 0;
        csr_y++;
    }
    else if(ch == '\t') {
        csr_x += 8;
    }
    else if(ch == '\r') {
        csr_x = 0;
    }
    else {
        if(csr_x >= 0 && csr_x < 80 && csr_y >=0 && csr_y < 25) {
            offset = csr_x + (csr_y * 80);
            while(offset--)
                av_vid += 2;
            *av_vid = ch;
            av_vid++;
            *av_vid = 0x07;
            av_vid++;
        }
        // Update cursor after writing a char
        if(csr_x >= 80) {
            csr_x = 0;
            csr_y++;
        }
        else
            csr_x++;
    }
    return;
}

/* Print a string to video buffer */
void s_printf(char str[]) {
    char *ch;
    ch = str;
    while(*ch != '\0') {
        c_printf(*ch);
        ch++;
    }
    return;
}

/* Print a n-digit to video buffer */
/* Print a hexadecimal number to video buffer */
void i_printf(uint64_t num, int base, bool xflag) {
    int i=0, flag=0, digit;
    int numArr[intSize] = {0};    // Initialize arr with 0's

    // If number is addr, append 0x to it
    if(xflag) {
        i_printf(0, 2, 0);
        c_printf('x');
    }
    // If number is 0, return back from here only
    if(num == 0) {
        // Pass ASCII val of integer
        c_printf(num + 48);
        return;
    }
    while(num > 0) {
        digit = num % base;
        if (digit > 9) {
            digit += 55;
        }
        numArr[i] = digit;
        num /= base;
        i++;
    }

    for(i=intSize; i>=0; i--) {
        if(numArr[i] != 0 || flag == 1)
            flag = 1;
        else
            continue;
        // Pass ASCII val of integer
        // If it's a integer, obtain ASCII val
        c_printf(numArr[i] + 48);
    }
    return;
}

/*
int printf(const char *format, ...) {
	int p;
    *((char*)0xb8000)=65;
	*((char*)0xb8001)=0x07;
    //c_printf('H');
    //c_printf('i');
	//i_printf(63, 16, 0);
    //i_printf(-40, 10, 0);
    //s_printf("Sumit");
    return 0;
}*/

int printk(const char *format, ...) {
	va_list val;
	int printed = 0;
    int64_t num = 0; uint64_t addr = 0;
	char *t;
	
	va_start(val, format);

	while(*format) {
		if(*format == '%'){
			++format;
			switch(*format){
				case 'd':
					num = va_arg (val, int);
                    // Added support for signed numbers
                    if (num < 0) c_printf('-');
                    i_printf((uint64_t)num, 10, 0);
					break;
				case 's':
					t = va_arg (val, char *);
					for(;*t;t++)
                        c_printf(*t);
					break;
				case 'x':
					num = va_arg (val, int);
                    if (num < 0) c_printf('-');
                    i_printf((uint64_t)num, 16, 0);
					break;
				case 'p':
					addr = va_arg (val, int);
                    i_printf(addr, 16, 1);
					break;
				case 'c':
					num = va_arg (val, int);
                    c_printf(num);
					break;
				case '%':
                    c_printf(*format);
			}
		}
		else{
            c_printf(*format);
		}
		++printed;
		++format;
	}
	va_end(val);
	return printed;
}

