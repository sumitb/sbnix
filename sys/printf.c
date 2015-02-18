#define A_VIDEO (char*)0xb8000
#define intSize     20      // We don't expect any number to be greater than 2^64

//GLOBALS
char *av_vid = A_VIDEO;       // Current addr of video buffer

/* Resets the global addr of video buffer to start */
// TODO: Write clear func and test it's functionality
void reset() {
    av_vid = A_VIDEO;
}

/* Print a single char to video buffer */
void c_printf(char ch) {
	// If it's a integer, obtain ASCII val
    if (ch >= 0 && ch <= 9)
        ch += 48;
    *av_vid = ch;
	av_vid++;
    *av_vid = 0x07;
    av_vid++;
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
void i_printf(long num, int base, int addr) {
    int i=0, flag=0, digit;
    int numArr[intSize] = {0};    // Initialize arr with 0's

    // If number is addr, append 0x to it
    if(addr == 1) {
        i_printf(0, 2, 0);
        c_printf('x');
    }
    // If number is 0, return back from here only
    if(num == 0) {
        c_printf(num);
        return;
    }
    // Added support for signed numbers
    else if (num < 0) {
        num *= -1;
        c_printf('-');
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
        c_printf(numArr[i]);
    }
    return;
}


int printf(const char *format, ...) {
	int p;
    i_printf((long)&p, 16, 1);
    //*((char*)0xb8000)=65;
	//*((char*)0xb8001)=0x07;
    //c_printf('H');
    //c_printf('i');
	//i_printf(63, 16, 0);
    //i_printf(-40, 10, 0);
    //s_printf("Sumit");
    return 0;
}


/*
 * value of a_vid = 0xb80000
 * increment it by 1 byte
 */
