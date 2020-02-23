#include "rs4854.h"
#include "SysTick.h"
#include "uart5.h"
/*******************************************************************************
* 函 数 名         : RS485_Init
* 函数功能		   : USART2初始化函数
* 输    入         : bound:波特率
* 输    出         : 无
*******************************************************************************/  
void RS4854_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE); //使能GPIOC时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//使能UART4时钟

	
	//USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(UART4, &USART_InitStructure); //初始化串口2
	
	USART_Cmd(UART4, ENABLE);  //使能串口4
	
	USART_ClearFlag(UART4, USART_FLAG_TC);
	
	
		
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;	//TX-485	//串口输出PC10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;		  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_Init(GPIOC,&GPIO_InitStructure);		/* 初始化串口输入IO */
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;	//RX-485	   //串口输入PC11
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	    //模拟输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;	//CS-485
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	   //推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
		
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启接受中断

	//Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	
	RS4854_TX_EN=1;				//默认为接收模式	
}

/*******************************************************************************
* 函 数 名         : USART2_IRQHandler
* 函数功能		   : USART2中断函数
* 输    入         : 无
* 输    出         : 无
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
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)//接收到数据
	{	
			res4 =USART_ReceiveData(UART4);//;读取接收到的数据UART4->DR
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
				//更新数值
				*(POS_BACK+13)=0x00;
				*(POS_BACK+14)=pos_buff[2];
				*(POS_BACK+15)=pos_buff[3];
				*(POS_BACK+16)=pos_buff[4];		
				BACK2=1;//更新完成标志					
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
