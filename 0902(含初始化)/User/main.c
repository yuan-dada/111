#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "uart5.h"
#include "rs4851.h"
#include "rs4852.h"
#include "rs4853.h"
#include "rs4854.h"
#include "timer.h"
#include "motor.h"

void OUT1_Init()
{
  GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;  //选择你要设置的IO口         PC0
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //设置推挽输出模式
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
  GPIO_Init(GPIOC,&GPIO_InitStructure); 	   //初始化GPIO 

  GPIO_ResetBits(GPIOC,GPIO_Pin_0);   //将LED端口置0，输出高电平，不制动
} 

/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
 
char POS_BACK[26];//返回的位置信息存储数组
extern int BACK1;
extern int BACK2;
extern int BACK3;


extern u8 flag1;
extern u8 flag3;
extern u8 Speed_size;
extern char Motor_Azimuth_READ[8];
extern char Motor_CCD_READ[8];
extern char move1;

char pos;





int main()
{
	POS_BACK[0]=0XAA;
	POS_BACK[1]=0XFF;
	POS_BACK[2]=0XCC;
	POS_BACK[3]=0XDD;
	POS_BACK[4]=0X01;
	POS_BACK[9]=0XFF;
	POS_BACK[10]=0XCC;
	POS_BACK[11]=0XDD;
	POS_BACK[12]=0X02;
	POS_BACK[17]=0XFF;
	POS_BACK[18]=0XCC;
	POS_BACK[19]=0XDD;
	POS_BACK[20]=0X03;	
	POS_BACK[25]=0XBB;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	LED_Init();     //PB8
	OUT1_Init();  //PC0
	uart5_Init(115200);
	RS4851_Init(115200);
	RS4852_Init(115200);
	RS4853_Init(115200);
	RS4854_Init(57600);
//	TIM2_Int_Init(1000,36000-1);//40mm中断。10Khz的计数频率，计数到5000为500ms 
	
	delay_ms(1000);
	delay_ms(1000);
	
	Motor_Azimuth_Init();	
	Motor_Pitch_Init();
	GPIO_SetBits(GPIOC,GPIO_Pin_0);   //将LED端口置1，输出低电平，制动CCD
	
	while(1)
	{

//发送查询编码器位置(电机2)
		pos=0x55;	
		RS4854_TX_EN=1;		
		delay_ms(1);
		USART_SendData(UART4,pos);
		while((UART4->SR&0X40)==0) ;
		RS4854_TX_EN=0;
		delay_ms(5);
//		RS4854_TX_EN=1;	
//发送查询电机1位置	

		u11_senddata(Motor_Azimuth_READ,8);

	//	while((USART1->SR&0X40)==0) ;
		flag1=1;
		RS4851_TX_EN=0;
		delay_ms(5);


//发送查询电机3位置	


		u3_senddata(Motor_CCD_READ,8);

		//while((USART3->SR&0X40)==0) ;
		flag3=1;
		RS4853_TX_EN=0;			
		delay_ms(5);			


	delay_ms(5);//最小15ms
		
		
		if(BACK1==1&&BACK3==1&&BACK2==1)
			{
				BACK1=0;
				BACK2=0;
				BACK3=0;
				u5_senddata(POS_BACK,26);
			}
  	led1=!led1;
			
	}
}
