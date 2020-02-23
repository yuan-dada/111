#ifndef _rs4851_H
#define _rs4851_H

#include "system.h"
#include "sys.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"

//模式控制
#define RS4851_TX_EN		PAout(8)	//485模式控制.0,接收;1,发送.
												 
void RS4851_Init(u32 bound);


#endif
