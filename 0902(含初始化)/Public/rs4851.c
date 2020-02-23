#include "rs4851.h"
#include "SysTick.h"
#include "uart5.h"
#include "motor.h"
/*******************************************************************************
* 函 数 名         : RS485_Init
* 函数功能		   : USART2初始化函数
* 输    入         : bound:波特率
* 输    出         : 无
*******************************************************************************/  
void RS4851_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //使能GPIOA\G时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART2时钟
	

	
	
	
	
	
	//USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口2
	
	USART_Cmd(USART1, ENABLE);  //使能串口 2
	
	USART_ClearFlag(USART1, USART_FLAG_TC);


		/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;	//TX-485	//串口输出PA9
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;		  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);		/* 初始化串口输入IO */
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;	//RX-485	   //串口输入PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	    //模拟输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;	//CS-485
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	   //推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

		
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启接受中断	
		//Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	
	
	
	
	RS4851_TX_EN=1;				//默认为接收模式	
}

/*******************************************************************************
* 函 数 名         : USART2_IRQHandler
* 函数功能		   : USART2中断函数
* 输    入         : 无
* 输    出         : 无
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

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//接收到数据
	{	 
		res1 =USART_ReceiveData(USART1);//;读取接收到的数据USART2->DR	
//k++;
		 if(flag1==1)
			{
				if(pos1_size==8) 
					 {
						flag1=0;    //一帧数据接收完成标志 
						Res_pos1[pos1_size]=res1;	
						
//k2++;						 
						for (i=0;i<7;i++)
								temp[i]=Res_pos1[i];					 
						a=CRC16_MODBUS(temp, 7);
						ah=a;
						al=a>>8;
						if((ah==Res_pos1[7])&& (al==Res_pos1[8]))
							 {
							           //更新数值
									*(POS_BACK+5)=Res_pos1[3];
									*(POS_BACK+6)=Res_pos1[4];
									*(POS_BACK+7)=Res_pos1[5];
									*(POS_BACK+8)=Res_pos1[6];
									BACK1=1;  //更新完成标志
//k3++;
								}
						pos1_size=0;
						RS4851_TX_EN=1; //返回发送状态
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

