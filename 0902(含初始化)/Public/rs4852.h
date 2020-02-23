#ifndef _rs4852_H
#define _rs4852_H

#include "system.h"


//模式控制
#define RS4852_TX_EN		PAout(1)	//485模式控制.0,接收;1,发送.
														 
void RS4852_Init(u32 bound);


#endif
