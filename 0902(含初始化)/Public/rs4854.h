#ifndef _rs4854_H
#define _rs4854_H

#include "system.h"


//ģʽ����
#define RS4854_TX_EN		PAout(12)	//485ģʽ����.0,����;1,����.
														 
void RS4854_Init(u32 bound);


#endif
