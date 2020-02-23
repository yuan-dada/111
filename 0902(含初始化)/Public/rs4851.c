#include "rs4851.h"
#include "SysTick.h"
#include "uart5.h"
#include "motor.h"
/*******************************************************************************
* �� �� ��         : RS485_Init
* ��������		   : USART2��ʼ������
* ��    ��         : bound:������
* ��    ��         : ��
*******************************************************************************/  
void RS4851_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //ʹ��GPIOA\Gʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART2ʱ��
	

	
	
	
	
	
	//USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������2
	
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ��� 2
	
	USART_ClearFlag(USART1, USART_FLAG_TC);


		/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;	//TX-485	//�������PA9
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;		  //�����������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);		/* ��ʼ����������IO */
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;	//RX-485	   //��������PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	    //ģ������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;	//CS-485
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	   //�������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

		
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//���������ж�	
		//Usart2 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
	
	
	
	RS4851_TX_EN=1;				//Ĭ��Ϊ����ģʽ	
}

/*******************************************************************************
* �� �� ��         : USART2_IRQHandler
* ��������		   : USART2�жϺ���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/ 
char Res_pos1[100];
u8 pos1_size=0;
u8 flag1=1;
int BACK1=0;
                                                int k1=0;
//                                                int k2=0;
//                                                int k3=0;
extern	char POS_BACK[26];
void USART1_IRQHandler(void)
{
	u8 res1;
	u16 a;
	char ah,al;
	int i;
	unsigned char temp[7];

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//���յ�����
	{	 
		res1 =USART_ReceiveData(USART1);//;��ȡ���յ�������USART2->DR	
//k++;
		 if(flag1==1)
			{
				if(pos1_size==8) 
					 {
						flag1=0;    //һ֡���ݽ�����ɱ�־ 
						Res_pos1[pos1_size]=res1;	
						
//k2++;						 
						for (i=0;i<7;i++)
								temp[i]=Res_pos1[i];					 
						a=CRC16_MODBUS(temp, 7);
						ah=a;
						al=a>>8;
						if((ah==Res_pos1[7])&& (al==Res_pos1[8]))
							 {
							           //������ֵ
									*(POS_BACK+5)=Res_pos1[3];
									*(POS_BACK+6)=Res_pos1[4];
									*(POS_BACK+7)=Res_pos1[5];
									*(POS_BACK+8)=Res_pos1[6];
									BACK1=1;  //������ɱ�־
//k3++;
								}
						pos1_size=0;
						RS4851_TX_EN=1; //���ط���״̬
					 }
				else 
					 {
							Res_pos1[pos1_size]=res1;
							pos1_size++;
						 k1++;
					 }
				if(k1==9)
					 {
						 RS4851_TX_EN=1; 
						 k1=0;
					 }					 
			}

		
	}	
	USART_ClearFlag(USART1,USART_FLAG_TC);	
}

