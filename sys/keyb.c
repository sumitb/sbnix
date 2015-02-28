#include <sys/defs.h>
#include <sys/sbunix.h>
#include <sys/timer.h>

extern void outportb(uint16_t port, uint8_t val);
extern uint8_t inportb(uint16_t port);

char kbuff[128];
int kbuff_count = 0;

void reset_keyborad_buffer(){
	int i=0;
	for(i=0;i<128;i++)
		kbuff[i]='\0';
}

unsigned char keyboard_map[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0, // 67
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */ //88
    '!', '@', '#', '$', '%', '^', '&', '*',	/* 9 */
  '(', ')', '_', '+'
 
};		

unsigned char keyboard_map_shift[128] =
{
    0,  27, 
    '!', '@', '#', '$', '%', '^', '&', '*',	/* 9 */
  '(', ')', '_', '+',
    '\b',	/* Backspace */
  '\t',			/* Tab */
  'Q', 'W', 'E', 'R',	/* 19 */
  'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',	/* 39 */
 '\"', '~',   0,		/* Left shift */
 '|', 'Z', 'X', 'C', 'V', 'B', 'N',			/* 49 */
  'M', '<', '>', '?',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0, // 67
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */ //88
 
};		

int shift_key=0;

void kb_handler()
{
	unsigned char scancode;
  	unsigned char status;
  
  	status = inportb(0x64);
  	scancode = status;
  
  	/* Read from the keyboard's data buffer */
  	scancode = inportb(0x60);
	if (scancode & 0x80)
  	{
		if(scancode == 0x2A)
		{
      			shift_key = 0;
		}	
	}
	else
  	{
		if(scancode == 0x2A)
		{
      			shift_key = 1;
		}
		else{
			if(shift_key == 1){
				printk("%c",keyboard_map_shift[scancode]);	
				//char kbuff[1024];
				//int kbuff_count = 0;
				kbuff_count++;
				kbuff[kbuff_count]=keyboard_map_shift[scancode];
			 	kbuff_count = kbuff_count%128;	
			}
			else{
				printk("%c",keyboard_map[scancode]);	
				kbuff_count++;
				kbuff[kbuff_count]=keyboard_map[scancode];
			 	kbuff_count = kbuff_count%128;	
			}	
		}	
	}
	outportb(0x20, 0x20);
}

