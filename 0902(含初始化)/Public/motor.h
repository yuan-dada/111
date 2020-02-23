#include "SysTick.h"



void Motor_Azimuth_Init(void);//�ϵ�ʱ����λ������ʼ��
void Motor_Pitch_Init(void);

void Motor_Azimuth_Move(u32 Location_Pulse,u32 Speed_Pulse);//��λ��������
void Motor_Pitch_Move(u32 Location_Pulse,u32 Speed_Pulse);//������������
void Motor_CCD_Move(u32 Location_Pulse,u32 Speed_Pulse);//CCD�������
void Read_AziEnco(void);//��������
             //CRC16����
unsigned short CRC16_MODBUS(unsigned char *puchMsg, unsigned int usDataLen);
void Invert_Uint8(unsigned char *dBuf, unsigned char *srcBuf);
void Invert_Uint16(unsigned short *dBuf, unsigned short *srcBuf) ;



