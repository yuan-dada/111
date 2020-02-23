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
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);	//ʹ��USART5��GPIOA��ʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //PC12
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//PD2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

    //USART ��ʼ������

    USART_InitStructure.USART_BaudRate = baudrate;//���ڲ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(UART5, &USART_InitStructure); //��ʼ������5
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
    USART_Cmd(UART5, ENABLE);                    //ʹ�ܴ���5
}




////////////////////////////////////////////////////////����5���ͺ���
void u5_senddata(char* fmt,u8 size)  
{     
	int i;
	for(i=0;i<size;i++)							//ѭ����������
	{
		UART5->DR=fmt[i]; 
		while((UART5->SR&0X40)==0);			//ѭ������,ֱ���������   		
	}
}

////////////////////////////////////////////////////////����1���ͺ���
void u1_senddata(char* fmt,u8 size)  //���͵��������
{     
	int j;
	RS4851_TX_EN=1;
	delay_ms(1);
	
	for(j=0;j<size;j++)							//ѭ����������
	{
		while((USART1->SR&0X40)==0);			//ѭ������,ֱ��������� 				
		USART1->DR=fmt[j];  

	}
}

////////////////////////////////////////////////////////����1���ͺ���
void u11_senddata(char* fmt,u8 size)  //���Ͳ�ѯ��
{     
	int j;
	RS4851_TX_EN=1;
	delay_ms(1);
	
	for(j=0;j<size;j++)							//ѭ����������
	{
			
		USART1->DR=fmt[j];  
		while((USART1->SR&0X40)==0);			//ѭ������,ֱ��������� 	
	}
}
////////////////////////////////////////////////////////����2���ͺ���
void u2_senddata(char* fmt,u8 size)  
{     
	int i;
	RS4852_TX_EN=1;
	
	delay_ms(1);
	for(i=0;i<size;i++)							//ѭ����������
	{
		while((USART2->SR&0X40)==0);			//ѭ������,ֱ���������   			
		USART2->DR=fmt[i];  
	
	}
}

////////////////////////////////////////////////////////����3���ͺ���
void u3_senddata(char* fmt,u8 size) //����3���ݷ��ͺ��� 
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
void u33_senddata(char* fmt,u8 size) //����3���ݷ��ͺ��� 
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
/////////////////////////////////////////////////////////����4���ݷ��ͺ��� 
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
char Com5_CMD_BUF[100];//8λѡ��λ��16λλ��λ��16λ�ٶ�λ
u8 Com5_Flag_CMD=2;
u8 Com5_BufSize=0;//���������ָ��

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
	

/////////////////////////////////////////	//����5�жϷ������
void UART5_IRQHandler(void)                
{
	u8 Res;
//	move1=1;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
  {
      Res =USART_ReceiveData(UART5);	//��ȡ���յ������ݣ�һ���ַ���
		
			if(Res==0xAB&&Com5_Flag_CMD==2)//�жϽ���1
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
						
												Com5_Flag_CMD=2;    //һ֡���ݽ�����ɱ�־
												
												
												u5_senddata(Com5_CMD_BUF,6);//����
												
								
												if(((Com5_CMD_BUF[0]^0xcc)==0)&& ((Com5_CMD_BUF[1]^0xcc)==0)&& ((Com5_CMD_BUF[2]^0xcc)==0)&& ((Com5_CMD_BUF[3]^0xcc)==0)&& ((Com5_CMD_BUF[4]^0xcc)==0)&& ((Com5_CMD_BUF[5]^0xcc)==0))
													{//��ʼ������
														init_flag=2;
														GPIO_ResetBits(GPIOC,GPIO_Pin_0);   //��LED�˿���0������ߵ�ƽ�����ƶ
														delay_ms(30);
														Motor_Azimuth_Init();
														Motor_Pitch_Init();
														delay_ms(30);
													}

												else
													{				
														/////�����ַ����õ�ѡ��š��ٶ�����λ����		
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

														

								//��λ�ж�
														//���һ5 6 7 8
														//�����13 14 15 16
														actual_pos=( (u32)(*(POS_BACK+13)<<24)|(u32)(*(POS_BACK+14)<<16)|(u32)(*(POS_BACK+15)<<8)|(u32)(*(POS_BACK+16))  );
														down_pos=0x00012158;//�½�
														up_pos=0X0001a4fc;  //�Ͻ�
														
													if((given1.POS&0x80000000)!=0)//Ϊ��������
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
													else   //Ϊ��������
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
							//							//�����21 22 23 24




								//����ָ��
																				 //��λ��
																	Motor_Azimuth_Move(given0.POS,given0.SPEED);
																					//������
																	Motor_Pitch_Move(given1.POS,given1.SPEED);
																					 //CCD
																	if(given2.POS !=0x0000)
																					GPIO_ResetBits(GPIOC,GPIO_Pin_0);   //��LED�˿���0������ߵ�ƽ�����ƶ
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

























