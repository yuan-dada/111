#include "SysTick.h"



void Motor_Azimuth_Init(void);//上电时，方位向电机初始化
void Motor_Pitch_Init(void);

void Motor_Azimuth_Move(u32 Location_Pulse,u32 Speed_Pulse);//方位向电机控制
void Motor_Pitch_Move(u32 Location_Pulse,u32 Speed_Pulse);//俯仰向电机控制
void Motor_CCD_Move(u32 Location_Pulse,u32 Speed_Pulse);//CCD电机控制
void Read_AziEnco(void);//读编码器
             //CRC16计算
unsigned short CRC16_MODBUS(unsigned char *puchMsg, unsigned int usDataLen);
void Invert_Uint8(unsigned char *dBuf, unsigned char *srcBuf);
void Invert_Uint16(unsigned short *dBuf, unsigned short *srcBuf) ;



