#ifndef _rs4853_H
#define _rs4853_H

#include "system.h"


//模式控制
#define  RS4853_TX_EN		PBout(1)	//485模式控制.0,接收;1,发送.
														 
void RS4853_Init(u32 bound);


#endif
