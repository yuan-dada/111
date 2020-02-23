#ifndef _rs4854_H
#define _rs4854_H

#include "system.h"


//模式控制
#define RS4854_TX_EN		PAout(12)	//485模式控制.0,接收;1,发送.
														 
void RS4854_Init(u32 bound);


#endif
