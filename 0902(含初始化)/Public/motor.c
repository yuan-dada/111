#include "SysTick.h"
#include "uart5.h"
#include "motor.h"
#include "rs4851.h"
#include "rs4853.h"
#include "rs4854.h"
//////////////////////方位向电机指令
char Motor_Azimuth_Control[17]={0x01,0x10,0x18,0x02,0x00,0x04,0x08};
char Motor_Azimuth_ON[8]={0x01,0x06,0x00,0x7D,0x00,0x08,0x18,0x14};//ON
char Motor_Azimuth_OFF[8]={0x01,0x06,0x00,0x7D,0x00,0x00,0x19,0xD2};//OFF
char Motor_Azimuth_ForwMove[8]={0x01,0x06,0x00,0x7D,0x80,0x00,0x78,0x12};//连续正传
char Motor_Azimuth_BackMove[8]={0x01,0x06,0x00,0x7D,0x40,0x00,0x28,0x12};//连续反转
char Motor_Azimuth_READ[8]={0x01,0x03,0x00,0xCC,0x00,0x02,0x04,0x34};//查询位置 01 03 00 CC 00 02 04 34

//////////////////////方位向初始化压推指令
char Motor_init_forward[8]={0x01,0x06,0x00,0x7D,0x40,0x11,0xE8,0x1E};//01 06 00 7D  40 11 E8 1E (连续正转)
char Motor_init_back[8]={0x01,0x06,0x00,0x7D,0x80,0x11,0xB8,0x1E};//01 06 00 7D  80 11 B8 1E (连续反转)
char Motor_init_off[8]={0x01,0x06,0x00,0x7D,0x00,0x00,0x19,0xD2};//01 06 00 7D 00 00 19 D2(OFF)
char Motor_middle[17]={0x01,0x10,0x18,0x02,0x00,0x04,0x08};//转至中点的指令
char Motor_init_startpoint[8]={0x01,0x06,0x00,0x7D,0x00,0x10,0x18,0x1E};//01 06 00 7D 00 10 18 1E(回到原点)

//////////////////////俯仰向电机指令，电机不同
//	励磁、位移、速度、ON OFF
char Motor_POSITION_Control[13]={0x01,0x10,0x04,0x02,0x00,0x02,0X04};//01 10 04 02 00 02 04 位移
char Motor_SPEED_Control[13]={0x01,0x10,0x05,0x02,0x00,0x02,0x04};   //01 10 05 02 00 02 04 速度
char Motor_Pitch_ON[8]={0x01,0x06,0x00,0x1E,0x21,0x01,0x30,0x5C};//ON 01 06 00 1E 21 01 30 5C
char Motor_Pitch_OFF[8]={0x01,0x06,0x00,0x1E,0x20,0x01,0x31,0xCC};//OFF 01 06 00 1E 20 01 31 CC
char Motor_ACTIVE[8]={0x01,0x06,0x00,0x1E,0x20,0x00,0xF0,0x0C};//励磁 01 06 00 1E 20 00 F0 0C

//////////////////////CCD向电机指令，TODO：改变寄存器地址
char Motor_CCD_Control[17]={0x01,0x10,0x18,0x02,0x00,0x04,0x08};
char Motor_CCD_ON[8]={0x01,0x06,0x00,0x7D,0x00,0x08,0x18,0x14};//ON
char Motor_CCD_OFF[9]={0x01,0x06,0x00,0x7D,0x00,0x00,0x19,0xD2};//OFF
char Motor_CCD_ForwMove[9]={0x01,0x06,0x00,0x7D,0x40,0x00,0x28,0x12};//连续正传
char Motor_CCD_BackMove[8]={0x01,0x06,0x00,0x7D,0x40,0x00,0x28,0x12};//连续反转
char Motor_CCD_READ[8]={0x01,0x03,0x00,0xCC,0x00,0x02,0x04,0x34};//查询位置 01 03 00 CC 00 02 04 34

char move1=0;
extern	char POS_BACK[26];
extern u8 flag1;
extern char pos;
extern int BACK2;
extern u32 init_flag;	
	u32 start_pos;
	u32 set_pos;
	u32 diff_pos;
	u32 step;
int x,x1=0;
//////////////////////////////////////////////////////////////////////////////
void Motor_Azimuth_Init(void)//上电时，方位向电机初始化，转向一侧，至限位处，然后反向移动至中点；
{		
	if(init_flag==2)
	//////////ccd
		{

			init_flag=1;
			u3_senddata(Motor_init_startpoint,8);
			//while((USART3->SR&0X40)==0);
			delay_ms(5);
			
			u3_senddata(Motor_CCD_OFF,8);
			//while((USART3->SR&0X40)==0);
			delay_ms(5);
			
			
		////////方位向
			RS4851_TX_EN=1;
			u11_senddata(Motor_init_startpoint,8);
			//while((USART1->SR&0X40)==0);
			delay_ms(5);
			
			u11_senddata(Motor_Azimuth_OFF,8);
			//while((USART1->SR&0X40)==0);
			delay_ms(5);
		}	
	else
		{
			u33_senddata(Motor_init_startpoint,8);
			while((USART3->SR&0X40)==0);
			delay_ms(5);
			
			u33_senddata(Motor_CCD_OFF,8);
			while((USART3->SR&0X40)==0);
			delay_ms(5);
			
			
		////////方位向
			RS4851_TX_EN=1;
			u1_senddata(Motor_init_startpoint,8);
			while((USART1->SR&0X40)==0);
			delay_ms(5);
			
			u1_senddata(Motor_Azimuth_OFF,8);
			while((USART1->SR&0X40)==0);
			delay_ms(5);	
		}
			///////俯仰向
		u2_senddata(Motor_ACTIVE , 8);	//励磁	
					delay_ms(5);		


}

	
void Motor_Pitch_Init(void)
{
		//发送查询编码器位置(电机2)
	while(1)
		{
				RS4854_TX_EN=1;		
				delay_ms(1);		
				USART_SendData(UART4,0x55);
				while((UART4->SR&0X40)==0) ;
				RS4854_TX_EN=0;
				delay_ms(5);
				if(BACK2==1)
				{
					break;
				}
																			x1++;
				if(x1==50) break;
			}


			if(BACK2==1)
				{
					BACK2=0;
					start_pos= ( (u32)(*(POS_BACK+13)<<24)|(u32)(*(POS_BACK+14)<<16)|(u32)(*(POS_BACK+15)<<8)|(u32)(*(POS_BACK+16))  );
					set_pos= 0x00018b96;
					if(((start_pos - set_pos)&(0x80000000))!=0){
							diff_pos= set_pos - start_pos;
							step=(diff_pos * 25000) / 131072;
					}
					else
					{
						diff_pos=(start_pos-set_pos);				
						step= 0x00000000-((diff_pos * 25000) / 131072);
					}
					delay_ms(5);
					Motor_Pitch_Move(step,0x00000318);
					
				}

}	





/////////////////////////////////////////////////////

void Invert_Uint8(unsigned char *dBuf, unsigned char *srcBuf)
	{

		int i;
		unsigned char tmp[4];
		tmp[0] = 0;
		for (i = 0; i < 8; i++)
		{
			if (srcBuf[0] & (1 << i))
				tmp[0] |= 1 << (7 - i);
		}
		dBuf[0] = tmp[0];
	}

void Invert_Uint16(unsigned short *dBuf, unsigned short *srcBuf) 
	{ 
		int i;    
		unsigned short tmp[4];    
		tmp[0] = 0;    
		for (i = 0; i< 16; i++) 
		{ 
			if (srcBuf[0] & (1 << i))
			tmp[0] |= 1 << (15 - i); 
		}    
		dBuf[0] = tmp[0]; 
	}
	
	
unsigned short CRC16_MODBUS(unsigned char *puchMsg, unsigned int usDataLen)
{
	unsigned short wCRCin = 0xFFFF;
	unsigned short wCPoly = 0x8005;
	unsigned char wChar = 0;
	int i;
	while (usDataLen--)
	{
		wChar = *(puchMsg++);
		Invert_Uint8(&wChar, &wChar);
		wCRCin ^= (wChar << 8);
		for ( i = 0; i < 8; i++)
		{
			if (wCRCin & 0x8000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	Invert_Uint16(&wCRCin, &wCRCin);
	return (wCRCin);
}

///////////////////////////////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////
void Motor_Azimuth_Move(u32 Location_Pulse,u32 Speed_Pulse)
{
    char Loca[4];
	  char Speed[4];
		int i;
		u16 CRC16;
		char CRC_H,CRC_L;
	
	  unsigned char Motor_Azimuth_Control_temp[15];
	  
	  Loca[0]=Location_Pulse>>24;
	  Loca[1]=Location_Pulse>>16;
	  Loca[2]=Location_Pulse>>8;
	  Loca[3]=Location_Pulse;
	      
	  Speed[0]= Speed_Pulse>>24;
	  Speed[1]= Speed_Pulse>>16;
	  Speed[2]= Speed_Pulse>>8;
	  Speed[3]= Speed_Pulse;
	
	  Motor_Azimuth_Control[7]=Loca[0];
	  Motor_Azimuth_Control[8]=Loca[1];
	  Motor_Azimuth_Control[9]=Loca[2];
	  Motor_Azimuth_Control[10]=Loca[3];
	  Motor_Azimuth_Control[11]=Speed[0];
	  Motor_Azimuth_Control[12]=Speed[1];
	  Motor_Azimuth_Control[13]=Speed[2];
	  Motor_Azimuth_Control[14]=Speed[3];
		for (i=0;i<15;i++)
		{
			Motor_Azimuth_Control_temp[i] = Motor_Azimuth_Control[i];
			
		}
		
		CRC16 = CRC16_MODBUS(Motor_Azimuth_Control_temp, 15);
		CRC_H=CRC16;
		CRC_L=CRC16>>8;
		Motor_Azimuth_Control[15]=CRC_H;
		Motor_Azimuth_Control[16]=CRC_L;	
		//u1_senddata(Motor_Azimuth_OFF , 8);//需要延时		
		//delay_ms(3);  
		u5_senddata(Motor_Azimuth_Control , 17);//需要延时	
				delay_ms(3);

		u11_senddata(Motor_Azimuth_Control , 17);//需要延时
		delay_ms(10);
		u11_senddata(Motor_Azimuth_ON , 8);
		delay_ms(10);
		u11_senddata(Motor_Azimuth_OFF , 8);
		while((USART1->SR&0X40)==0);
		delay_ms(3);
		
		move1=0;


}



///////////////////////////////////////////////////////////
void Motor_Pitch_Move(u32 Location_Pulse,u32 Speed_Pulse)
{
    char Loca[4];
	  char Speed[4];
		int i;
		u16 CRC16;
		char CRC_H,CRC_L;
	
	  unsigned char Motor_POSITION_Control_temp[11];
		unsigned char Motor_SPEED_Control_temp[11];
	  
	  Loca[0]=Location_Pulse>>24;
	  Loca[1]=Location_Pulse>>16;
	  Loca[2]=Location_Pulse>>8;
	  Loca[3]=Location_Pulse;
	      
	  Speed[0]= Speed_Pulse>>24;
	  Speed[1]= Speed_Pulse>>16;
	  Speed[2]= Speed_Pulse>>8;
	  Speed[3]= Speed_Pulse;
	//
	  Motor_POSITION_Control[7]=Loca[0];
	  Motor_POSITION_Control[8]=Loca[1];
	  Motor_POSITION_Control[9]=Loca[2];
	  Motor_POSITION_Control[10]=Loca[3];

	  Motor_SPEED_Control[7]=Speed[0];
		Motor_SPEED_Control[8]=Speed[1];
		Motor_SPEED_Control[9]=Speed[2];
		Motor_SPEED_Control[10]=Speed[3];
		
		
		for (i=0;i<11;i++)
		{
			Motor_POSITION_Control_temp[i] = Motor_POSITION_Control[i];
			
		}
		
		CRC16 = CRC16_MODBUS(Motor_POSITION_Control_temp, 11);
		CRC_H=CRC16;
		CRC_L=CRC16>>8;
		Motor_POSITION_Control[11]=CRC_H;
		Motor_POSITION_Control[12]=CRC_L;	
		
		for (i=0;i<11;i++)
		{
			Motor_SPEED_Control_temp[i] = Motor_SPEED_Control[i];
			
		}
		
		CRC16 = CRC16_MODBUS(Motor_SPEED_Control_temp, 11);
		CRC_H=CRC16;
		CRC_L=CRC16>>8;
		Motor_SPEED_Control[11]=CRC_H;
		Motor_SPEED_Control[12]=CRC_L;	

		

		u2_senddata(Motor_POSITION_Control , 13);//位置
				delay_ms(10);
		u2_senddata(Motor_SPEED_Control , 13);//速度
				delay_ms(10);
		u2_senddata(Motor_Pitch_ON , 8);//on
				delay_ms(10);
		u2_senddata(Motor_Pitch_OFF , 8);//OFF
		while((USART2->SR&0X40)==0);
		delay_ms(3);
		u2_senddata(Motor_ACTIVE , 8);	//励磁	
				delay_ms(10);

}


///////////////////////////////////////////////////////////
void Motor_CCD_Move(u32 Location_Pulse,u32 Speed_Pulse)
{
    char Loca[4];
	  char Speed[4];
		int i;
		u16 CRC16;
		char CRC_H,CRC_L;
	
	  unsigned char Motor_CCD_Control_temp[15];
	  
	  Loca[0]=Location_Pulse>>24;
	  Loca[1]=Location_Pulse>>16;
	  Loca[2]=Location_Pulse>>8;
	  Loca[3]=Location_Pulse;
	      
	  Speed[0]= Speed_Pulse>>24;
	  Speed[1]= Speed_Pulse>>16;
	  Speed[2]= Speed_Pulse>>8;
	  Speed[3]= Speed_Pulse;
	
	  Motor_CCD_Control[7]=Loca[0];
	  Motor_CCD_Control[8]=Loca[1];
	  Motor_CCD_Control[9]=Loca[2];
	  Motor_CCD_Control[10]=Loca[3];
	  Motor_CCD_Control[11]=Speed[0];
	  Motor_CCD_Control[12]=Speed[1];
	  Motor_CCD_Control[13]=Speed[2];
	  Motor_CCD_Control[14]=Speed[3];
		for (i=0;i<15;i++)
		{
			Motor_CCD_Control_temp[i] = Motor_CCD_Control[i];
			
		}
		
		CRC16 = CRC16_MODBUS(Motor_CCD_Control_temp, 15);
		CRC_H=CRC16;
		CRC_L=CRC16>>8;
		Motor_CCD_Control[15]=CRC_H;
		Motor_CCD_Control[16]=CRC_L;	
		
		
//		u5_senddata(Motor_CCD_Control , 17);		
		u3_senddata(Motor_CCD_Control , 17);
						delay_ms(10);
		u3_senddata(Motor_CCD_ON , 8);
						delay_ms(10);
		u3_senddata(Motor_CCD_OFF , 8);
		while((USART3->SR&0X40)==0);
		delay_ms(3);
		
}
