/*
 * Referenced from osdever.net - Bran's Kernel Dev 
 */
#include <sys/defs.h>
#include <sys/console.h>
#include <sys/timer.h>

extern void outportb(uint16_t port, uint8_t val);
extern uint8_t inportb(uint16_t port);
char kbuffer[512];
int count=0;
bool flag=false;

unsigned char keyborard_character_set_normal[128] =
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

unsigned char Keyboard_character_set_shift[128] =
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
    int x, y;
	unsigned char check_code;
  
  	check_code = inportb(0x60);

	if (check_code & 0x80){
		if(check_code == 0x2A)
  		{
      			shift_key = 0;
		}
	}
	else
  	{
		if(check_code == 0x2A)
		{
      			shift_key = 1;
		}
		else{
            // Get current cursor position
            x = getcsr_x(); y = getcsr_y();
            // Move to bottom right corner
            gotoxy(60, 22);
			if(shift_key == 1){
				if(Keyboard_character_set_shift[check_code]=='\n')
					printk("\n");	
				else
					printk("%c",Keyboard_character_set_shift[check_code]);	
				shift_key = 0;
			}
			else{
				if(keyborard_character_set_normal[check_code]=='\n')
					printk("\n");	
				else
					printk("%c",keyborard_character_set_normal[check_code]);	
			}	
            // Reset cursor position
            gotoxy(x, y);
		}	
	}
	outportb(0x20, 0x20);
}

/*

int read(int fildes,void *buf,int nbyte){
    int i=0;
    Flag=true;
    count=0;
    
    for(i=0;i<512;i++)
        kbuffer[i]='\0';
    while(Flag);
    for(i=0;i<512;i++)
        buf[i]='\0';
        
    kbuffer[count] = '\0';
    strcpy(buf,kbuffer);

    for(i=0;i<512;i++)
        kbuffer[i]='\0';

    return count;
}






*/
