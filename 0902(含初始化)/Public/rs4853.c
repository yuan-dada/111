#include "rs4853.h"
#include "SysTick.h"
#include "uart5.h"
#include "motor.h"
/*******************************************************************************
* 函 数 名         : RS485_Init
* 函数功能		   : USART2初始化函数
* 输    入         : bound:波特率
* 输    出         : 无
*******************************************************************************/  
void RS4853_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //使能GPIOA、GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART2时钟
	

	
	//USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART3, &USART_InitStructure); //初始化串口2
	
	USART_Cmd(USART3, ENABLE);  //使能串口 2
	
	USART_ClearFlag(USART3, USART_FLAG_TC);
	
	
		/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;	//TX-485	//串口输出PB10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;		  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStructure);		/* 初始化串口输入IO */
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;	//RX-485	   //串口输入PB11
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	    //模拟输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;	//CS-485
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	   //推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
		
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启接受中断

	//Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	
	RS4853_TX_EN=1;				//默认为接收模式	
}



char Res_pos3[100];
u8 flag3=1;
u8 pos3_size=0;
int BACK3=0;


                                                int k3=0;

extern	char POS_BACK[26];
/*******************************************************************************
* 函 数 名         : USART2_IRQHandler
* 函数功能		   : USART2中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/ 
void USART3_IRQHandler(void)
{
	u8 res3;	
	u16 a;
	char ah,al;
	int i;
	unsigned char temp[7];
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//接收到数据
	{	 	
		res3 =USART_ReceiveData(USART3);//;读取接收到的数据USART2->DR	
		if(flag3==1)
			{
				if(pos3_size==8) 
					 {
							flag3=0;    //一帧数据接收完成标志
							Res_pos3[pos3_size]=res3;
				 
						 	for (i=0;i<7;i++)
								   temp[i]=Res_pos3[i];					 
							a=CRC16_MODBUS(temp, 7);
							ah=a;
							al=a>>8;
							if((ah==Res_pos3[7])&& (al==Res_pos3[8]))
							 {
										//更新数值
									*(POS_BACK+21)=Res_pos3[3];
									*(POS_BACK+22)=Res_pos3[4];
									*(POS_BACK+23)=Res_pos3[5];
									*(POS_BACK+24)=Res_pos3[6];
									BACK3=1;  //更新完成标志

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
