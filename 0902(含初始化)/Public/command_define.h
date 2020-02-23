
#include "stdio.h"	

char req[8];//读取当前的位置

void command_define(void);
void command_define(void)
{

				req[0] = 0x01;
				req[1] = 0x03;
				req[2] = 0x00;
				req[3] = 0xC6;
				req[4] = 0x00;
				req[5] = 0x02;
				req[6] = 0x24;
				req[7] = 0x36;
	
}
