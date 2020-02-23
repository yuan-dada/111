#include "rs4853.h"
#include "SysTick.h"
#include "uart5.h"
#include "motor.h"
/*******************************************************************************
* �� �� ��         : RS485_Init
* ��������		   : USART2��ʼ������
* ��    ��         : bound:������
* ��    ��         : ��
*******************************************************************************/  
void RS4853_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //ʹ��GPIOA��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART2ʱ��
	

	
	//USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure); //��ʼ������2
	
	USART_Cmd(USART3, ENABLE);  //ʹ�ܴ��� 2
	
	USART_ClearFlag(USART3, USART_FLAG_TC);
	
	
		/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;	//TX-485	//�������PB10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;		  //�����������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStructure);		/* ��ʼ����������IO */
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;	//RX-485	   //��������PB11
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	    //ģ������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;	//CS-485
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	   //�������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
		
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//���������ж�

	//Usart2 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
	RS4853_TX_EN=1;				//Ĭ��Ϊ����ģʽ	
}



char Res_pos3[100];
u8 flag3=1;
u8 pos3_size=0;
int BACK3=0;


                                                int k3=0;

extern	char POS_BACK[26];
/*******************************************************************************
* �� �� ��         : USART2_IRQHandler
* ��������		   : USART2�жϺ���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/ 
void USART3_IRQHandler(void)
{
	u8 res3;	
	u16 a;
	char ah,al;
	int i;
	unsigned char temp[7];
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//���յ�����
	{	 	
		res3 =USART_ReceiveData(USART3);//;��ȡ���յ�������USART2->DR	
		if(flag3==1)
			{
				if(pos3_size==8) 
					 {
							flag3=0;    //һ֡���ݽ�����ɱ�־
							Res_pos3[pos3_size]=res3;
				 
						 	for (i=0;i<7;i++)
								   temp[i]=Res_pos3[i];					 
							a=CRC16_MODBUS(temp, 7);
							ah=a;
							al=a>>8;
							if((ah==Res_pos3[7])&& (al==Res_pos3[8]))
							 {
										//������ֵ
									*(POS_BACK+21)=Res_pos3[3];
									*(POS_BACK+22)=Res_pos3[4];
									*(POS_BACK+23)=Res_pos3[5];
									*(POS_BACK+24)=Res_pos3[6];
									BACK3=1;  //������ɱ�־

							 }	
							pos3_size=0;
							RS4853_TX_EN=1; 
					 }
				else 
					 {
							Res_pos3[pos3_size]=res3;	
							pos3_size++;
						 k3++;
					 }
				if(k3==9)
					 {
						 RS4853_TX_EN=1; 
						 k3=0;
					 }
			}
		

	}  
    USART_ClearFlag(USART3,USART_FLAG_TC);	
} 	
