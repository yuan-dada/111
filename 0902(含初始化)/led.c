#include "led.h"

void LED_Init()
{
  GPIO_InitTypeDef GPIO_InitStructure;//����ṹ�����
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;  //ѡ����Ҫ���õ�IO��
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //�����������ģʽ
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
  GPIO_Init(GPIOB,&GPIO_InitStructure); 	   //��ʼ��GPIO 
	
  GPIO_ResetBits(GPIOB,GPIO_Pin_8);   //��LED�˿����ߣ�Ϩ������LED
} 
 
