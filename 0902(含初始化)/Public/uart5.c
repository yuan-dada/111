#include "uart5.h"
#include "SysTick.h"
#include "rs4851.h"
#include "rs4852.h"
#include "rs4853.h"
#include "rs4854.h"
#include "motor.h"

///////////////////////////////////////////////////////////////////
void uart5_Init(u32 baudrate)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
     
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);	//Ê¹ÄÜUSART5£¬GPIOA¡¢Ê±ÖÓ

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //PC12
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//¸´ÓÃÍÆÍìÊä³ö
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//PD2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//¸¡¿ÕÊäÈë
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    //Usart1 NVIC ÅäÖÃ
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//ÇÀÕ¼ÓÅÏÈ¼¶3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//×ÓÓÅÏÈ¼¶3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
    NVIC_Init(&NVIC_InitStructure);	//¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯VIC¼Ä´æÆ÷

    //USART ³õÊ¼»¯ÉèÖÃ

    USART_InitStructure.USART_BaudRate = baudrate;//´®¿Ú²¨ÌØÂÊ
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//×Ö³¤Îª8Î»Êý¾Ý¸ñÊ½
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//Ò»¸öÍ£Ö¹Î»
    USART_InitStructure.USART_Parity = USART_Parity_No;//ÎÞÆæÅ¼Ð£ÑéÎ»
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎÞÓ²¼þÊý¾ÝÁ÷¿ØÖÆ
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ÊÕ·¢Ä£Ê½

    USART_Init(UART5, &USART_InitStructure); //³õÊ¼»¯´®¿Ú5
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//¿ªÆô´®¿Ú½ÓÊÜÖÐ¶Ï
    USART_Cmd(UART5, ENABLE);                    //Ê¹ÄÜ´®¿Ú5
}




////////////////////////////////////////////////////////´®¿Ú5·¢ËÍº¯Êý
void u5_senddata(char* fmt,u8 size)  
{     
	int i;
	for(i=0;i<size;i++)							//Ñ­»··¢ËÍÊý¾Ý
	{
		UART5->DR=fmt[i]; 
		while((UART5->SR&0X40)==0);			//Ñ­»··¢ËÍ,Ö±µ½·¢ËÍÍê±Ï   		
	}
}

////////////////////////////////////////////////////////´®¿Ú1·¢ËÍº¯Êý
void u1_senddata(char* fmt,u8 size)  //·¢ËÍµç»ú¿ØÖÆÂð
{     
	int j;
	RS4851_TX_EN=1;
	delay_ms(1);
	
	for(j=0;j<size;j++)							//Ñ­»··¢ËÍÊý¾Ý
	{
		while((USART1->SR&0X40)==0);			//Ñ­»··¢ËÍ,Ö±µ½·¢ËÍÍê±Ï 				
		USART1->DR=fmt[j];  

	}
}

////////////////////////////////////////////////////////´®¿Ú1·¢ËÍº¯Êý
void u11_senddata(char* fmt,u8 size)  //·¢ËÍ²éÑ¯Âð
{     
	int j;
	RS4851_TX_EN=1;
	delay_ms(1);
	
	for(j=0;j<size;j++)							//Ñ­»··¢ËÍÊý¾Ý
	{
			
		USART1->DR=fmt[j];  
		while((USART1->SR&0X40)==0);			//Ñ­»··¢ËÍ,Ö±µ½·¢ËÍÍê±Ï 	
	}
}
////////////////////////////////////////////////////////´®¿Ú2·¢ËÍº¯Êý
void u2_senddata(char* fmt,u8 size)  
{     
	int i;
	RS4852_TX_EN=1;
	
	delay_ms(1);
	for(i=0;i<size;i++)							//Ñ­»··¢ËÍÊý¾Ý
	{
		while((USART2->SR&0X40)==0);			//Ñ­»··¢ËÍ,Ö±µ½·¢ËÍÍê±Ï   			
		USART2->DR=fmt[i];  
	
	}
}

////////////////////////////////////////////////////////´®¿Ú3·¢ËÍº¯Êý
void u3_senddata(char* fmt,u8 size) //´®¿Ú3Êý¾Ý·¢ËÍº¯Êý 
{     
	int i;
	RS4853_TX_EN=1;

	delay_ms(1);
	for(i=0;i<size;i++)						
	{
 			
		USART3->DR=fmt[i];  
		while((USART3->SR&0X40)==0);   	
	}
}
void u33_senddata(char* fmt,u8 size) //´®¿Ú3Êý¾Ý·¢ËÍº¯Êý 
{     
	int i;
	RS4853_TX_EN=1;

	delay_ms(1);
	for(i=0;i<size;i++)						
	{
		while((USART3->SR&0X40)==0);   			
		USART3->DR=fmt[i];  
 	
	}
}
/////////////////////////////////////////////////////////´®¿Ú4Êý¾Ý·¢ËÍº¯Êý 
void u4_senddata(char* fmt,u8 size) 
{     
	int i;
	RS4854_TX_EN=1;

	delay_ms(1);
	for(i=0;i<size;i++)						
	{
		while((UART4->SR&0X40)==0); 				
		UART4->DR=fmt[i];

	}
}
///////////////////////////////////////////////////////////////////////
char Com5_CMD_BUF[100];//8Î»Ñ¡ÔñÎ»£¬16Î»Î»ÖÃÎ»£¬16Î»ËÙ¶ÈÎ»
u8 Com5_Flag_CMD=2;
u8 Com5_BufSize=0;//¿ØÖÆÃüÁîµÄÖ¸Áî

struct CMD_BUFF
	{
		u8 CHOICE;
		u32 POS;
		u32 SPEED;
	};
 
struct CMD_BUFF given0,given1,given2;

extern char move1;
extern char POS_BACK[26];
u32 init_flag=1;	
u32 actual_pos;
u32 down_pos;
u32 up_pos;
u32 max_diff;
u32 max_step;
	

/////////////////////////////////////////	//´®¿Ú5ÖÐ¶Ï·þÎñ³ÌÐò
void UART5_IRQHandler(void)                
{
	u8 Res;
//	move1=1;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //½ÓÊÕÖÐ¶Ï(½ÓÊÕµ½µÄÊý¾Ý±ØÐëÊÇ0x0d 0x0a½áÎ²)
  {
      Res =USART_ReceiveData(UART5);	//¶ÁÈ¡½ÓÊÕµ½µÄÊý¾Ý£¨Ò»¸ö×Ö·û£©
		
			if(Res==0xAB&&Com5_Flag_CMD==2)//ÅÐ¶Ï½ÓÊÕ1
			{
				Com5_Flag_CMD=1;   Com5_BufSize=0;
			}
			else if(Com5_Flag_CMD==1)
			{		
				if(Res==0xBA)
				{
//					if((Com5_CMD_BUF[Com5_BufSize-1]^0xBA)==0)
//					{
//											if((Com5_CMD_BUF[Com5_BufSize-2]^0xBA)==0)
//											{
						
												Com5_Flag_CMD=2;    //Ò»Ö¡Êý¾Ý½ÓÊÕÍê³É±êÖ¾
												
												
												u5_senddata(Com5_CMD_BUF,6);//¼ì²â¿Ú
												
								
												if(((Com5_CMD_BUF[0]^0xcc)==0)&& ((Com5_CMD_BUF[1]^0xcc)==0)&& ((Com5_CMD_BUF[2]^0xcc)==0)&& ((Com5_CMD_BUF[3]^0xcc)==0)&& ((Com5_CMD_BUF[4]^0xcc)==0)&& ((Com5_CMD_BUF[5]^0xcc)==0))
													{//³õÊ¼»¯ÃüÁî
														init_flag=2;
														GPIO_ResetBits(GPIOC,GPIO_Pin_0);   //½«LED¶Ë¿ÚÖÃ0£¬Êä³ö¸ßµçÆ½£¬²»ÖÆ¶
														delay_ms(30);
														Motor_Azimuth_Init();
														Motor_Pitch_Init();
														delay_ms(30);
													}

												else
													{				
														/////½âÎö×Ö·û£¬µÃµ½Ñ¡ÔñºÅ¡¢ËÙ¶ÈÁ¿¡¢Î»ÖÃÁ¿		
														given0.CHOICE=Com5_CMD_BUF[0];

														if( (Com5_CMD_BUF[1]&0x80)==0x00 )
															given0.POS=(  0x00000000|(u32)Com5_CMD_BUF[1]<<8|(u32)Com5_CMD_BUF[2] );
														else 
															given0.POS=(  0xFFFF0000|(u32)Com5_CMD_BUF[1]<<8|(u32)Com5_CMD_BUF[2] );
														
														if( (Com5_CMD_BUF[3]&0x80)==0x00 )
															given0.SPEED=(  0x00000000|(u32)Com5_CMD_BUF[3]<<8|(u32)Com5_CMD_BUF[4] );
														else 
															given0.SPEED=(  0xFFFF0000|(u32)Com5_CMD_BUF[3]<<8|(u32)Com5_CMD_BUF[4] );
														
														
														given1.CHOICE=Com5_CMD_BUF[5];

														if( (Com5_CMD_BUF[6]&0x80)==0x00 )
															given1.POS=(  0x00000000|(u32)Com5_CMD_BUF[6]<<8|(u32)Com5_CMD_BUF[7] );
														else 
															given1.POS=(  0xFFFF0000|(u32)Com5_CMD_BUF[6]<<8|(u32)Com5_CMD_BUF[7] );
														
														if( (Com5_CMD_BUF[8]&0x80)==0x00 )
															given1.SPEED=(  0x00000000|(u32)Com5_CMD_BUF[8]<<8|(u32)Com5_CMD_BUF[9] );
														else 
															given1.SPEED=(  0xFFFF0000|(u32)Com5_CMD_BUF[8]<<8|(u32)Com5_CMD_BUF[9] );
																			
														
														
														given2.CHOICE=Com5_CMD_BUF[10];

														if( (Com5_CMD_BUF[11]&0x80)==0x00 )
															given2.POS=(  0x00000000|(u32)Com5_CMD_BUF[11]<<8|(u32)Com5_CMD_BUF[12] );
														else 
															given2.POS=(  0xFFFF0000|(u32)Com5_CMD_BUF[11]<<8|(u32)Com5_CMD_BUF[12] );
														
														if( (Com5_CMD_BUF[13]&0x80)==0x00 )
															given2.SPEED=(  0x00000000|(u32)Com5_CMD_BUF[13]<<8|(u32)Com5_CMD_BUF[14] );
														else 
															given2.SPEED=(  0xFFFF0000|(u32)Com5_CMD_BUF[13]<<8|(u32)Com5_CMD_BUF[14] );

														

								//ÏÞÎ»ÅÐ¶Ï
														//µç»úÒ»5 6 7 8
														//µç»ú¶þ13 14 15 16
														actual_pos=( (u32)(*(POS_BACK+13)<<24)|(u32)(*(POS_BACK+14)<<16)|(u32)(*(POS_BACK+15)<<8)|(u32)(*(POS_BACK+16))  );
														down_pos=0x00012158;//ÏÂ½ç
														up_pos=0X0001a4fc;  //ÉÏ½ç
														
													if((given1.POS&0x80000000)!=0)//Îª¸º£¬ÏÂÐÐ
														{
															if((actual_pos-down_pos)<0x00000010||(down_pos-actual_pos)<0x00000010){
																given1.POS=0x00000000;
															}
															else
																{
																	max_diff=actual_pos-down_pos ;
																	max_step=0x00000000-(max_diff*25000)/131072;
																	if(given1.POS < max_step)
																		{
																			given1.POS = max_step;
																		}
																}
														
														}
													else   //ÎªÕý£¬ÉÏÐÐ
														{
														
															if((up_pos-actual_pos)<0x00000010||(actual_pos-up_pos)<0x00000010){
																given1.POS=0x00000000;
															}
															else
																{
																	max_diff = up_pos - actual_pos;
																	max_step=(max_diff*25000)/131072;
																	if(given1.POS > max_step)
																	{
																		given1.POS = max_step;								
																	}
																}
														}
							//							//µç»úÈý21 22 23 24




								//·¢ËÍÖ¸Áî
																				 //·½Î»Ïò
																	Motor_Azimuth_Move(given0.POS,given0.SPEED);
																					//¸©ÑöÏò
																	Motor_Pitch_Move(given1.POS,given1.SPEED);
																					 //CCD
																	if(given2.POS !=0x0000)
																					GPIO_ResetBits(GPIOC,GPIO_Pin_0);   //½«LED¶Ë¿ÚÖÃ0£¬Êä³ö¸ßµçÆ½£¬²»ÖÆ¶
																			delay_ms(30);						
																	Motor_CCD_Move(given2.POS,given2.SPEED);										
													 }									
//											}		
//				  }	
//					else
//					{
//										Com5_CMD_BUF[Com5_BufSize]=Res;
//										Com5_BufSize++;
//						
//					}					
																
				}
				else 
				{

					 Com5_CMD_BUF[Com5_BufSize]=Res;
					 Com5_BufSize++;
				}
			}
	}
}

























