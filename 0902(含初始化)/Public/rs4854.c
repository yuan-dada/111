#include "rs4854.h"
#include "SysTick.h"
#include "uart5.h"
/*******************************************************************************
* �� �� ��         : RS485_Init
* ��������		   : USART2��ʼ������
* ��    ��         : bound:������
* ��    ��         : ��
*******************************************************************************/  
void RS4854_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE); //ʹ��GPIOCʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//ʹ��UART4ʱ��

	
	//USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(UART4, &USART_InitStructure); //��ʼ������2
	
	USART_Cmd(UART4, ENABLE);  //ʹ�ܴ���4
	
	USART_ClearFlag(UART4, USART_FLAG_TC);
	
	
		
	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;	//TX-485	//�������PC10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;		  //�����������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_Init(GPIOC,&GPIO_InitStructure);		/* ��ʼ����������IO */
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;	//RX-485	   //��������PC11
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	    //ģ������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;	//CS-485
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	   //�������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
		
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//���������ж�

	//Usart2 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
	RS4854_TX_EN=1;				//Ĭ��Ϊ����ģʽ	
}

/*******************************************************************************
* �� �� ��         : USART2_IRQHandler
* ��������		   : USART2�жϺ���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/ 
extern	char POS_BACK[26];

char pos_buff[100];


int flag4=1;
int size4=0;
                                             int k4=0;
                                             int a=0;
                                             int a2=0;
                                             int a3=0;
                                             int a4=0;
u8 res4;
int BACK2=0;
void UART4_IRQHandler(void)
{
		u16 sum=0;
	int j;	
a++;	
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)//���յ�����
	{	
			res4 =USART_ReceiveData(UART4);//;��ȡ���յ�������UART4->DR
a2++;
			if (size4==5)	
			{
				pos_buff[size4]=res4;
a3++;
				for(j=0;j<5;j++)
				{
					sum+=pos_buff[j];
				}
				if((char)sum==pos_buff[5])
				{
				                                       //if(pos_buff[0]==(char)0xff && pos_buff[1]==(char)0x81)
					                                     //u5_senddata(pos_buff,6);//ceshi		
				//������ֵ
				*(POS_BACK+13)=0x00;
				*(POS_BACK+14)=pos_buff[2];
				*(POS_BACK+15)=pos_buff[3];
				*(POS_BACK+16)=pos_buff[4];		
				BACK2=1;//������ɱ�־					
				}
				sum=0;
				size4=0;							
				RS4854_TX_EN=1;					


			}
			else
					{
						pos_buff[size4]=res4;
						size4++;	
						k4++;		
a4++;						
					}
			if(k4==9)
					 {
						 RS4854_TX_EN=1; 
						 k4=0;
					 }
		
	}  
    USART_ClearFlag(UART4,USART_FLAG_TC);	
} 	
