#ifndef _rs4851_H
#define _rs4851_H

#include "system.h"
#include "sys.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"

//ģʽ����
#define RS4851_TX_EN		PAout(8)	//485ģʽ����.0,����;1,����.
												 
void RS4851_Init(u32 bound);


#endif
