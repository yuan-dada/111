#ifndef UART5_H
#define UART5_H


#include "sys.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"

#define UART5_REC_LEN  			200  	//�����������ֽ��� 200
	  	
extern u8  USART5_RX_BUF[UART5_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART5_RX_STA;         		//����״̬���

void uart5_Init(u32 baudrate);
void u5_senddata(char* fmt,u8 size);
void u1_senddata(char* fmt,u8 size);
void u11_senddata(char* fmt,u8 size);
void u2_senddata(char* fmt,u8 size);
void u3_senddata(char* fmt,u8 size);
void u33_senddata(char* fmt,u8 size); //����3���ݷ��ͺ��� 
void u4_senddata(char* fmt,u8 size);
void uart5_test(void);
void u5_printf(char *fmt,...);




#endif
