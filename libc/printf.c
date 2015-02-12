#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define MAXLEN	1024
int printf(const char *format, ...) {
	va_list val;
	int printed = 0;int num=0;
//	char buffer[MAXLEN]="\0";
	char tmp[15]="\0";
	char *t;
	
	va_start(val, format);

	while(*format) {
		if(*format == '%'){
			++format;
			switch(*format){
				case 'd':
					num = va_arg (val, int);
					itoa(num,tmp);
					write(1,tmp,15);
					break;
				case 's':
					t = va_arg (val, char *);
					for(;*t;t++)
						write(1,t,1);
					break;
				case 'x':
					num = va_arg (val, int);
					xtoa(num,tmp);
					write(1,tmp,15);
					break;
				case 'c':
					num = va_arg (val, int);
					tmp[0]=num;
					tmp[1]='\0';
					write(1,tmp,1);
					break;
				case '%':
					write(1,format,1);
			}
		}
		else{
			write(1,format,1);
		}
		++printed;
		++format;
	}
	va_end(val);
	return printed;
}
