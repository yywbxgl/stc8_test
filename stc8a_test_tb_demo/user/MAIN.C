/*                        -- �ɺ����ϵ��ӹ����� --                        */
/*
*   ˵    ��: STC8A8KS4A12 DEMO����
*   ����ƽ̨: STC15W408S     
*   �Ա�����: 
*
//  �ļ�����main.c                                                              
//  ˵�������ͻ�����ģ��ͨ��ʹ�ó���                                                                  
//  ��д��Ա��Duhemayi                                                                   
//  ��д���ڣ�2018-09-16                                                               
//  ����ά����
//  ά����¼��
//	��    ��: V1.0
//                                                          
// �����������ó��������ѧϰ�뽻�� 
// (c) Duhemayi Corporation. All rights reserved.     
******************************************************************************/
#include "config.h"

/******************************************************************************/
// �������ƣ�main 
// ��������� 
// ��������� 
// �������ܣ� 
/******************************************************************************/
	
void main(void)
{
	//uint16 ADC_RES;
		
//	Uart1Init();
////	Timer4_Init();
//    ES = 1;
//    EA = 1;
//    UartSendStr("Uart Test !\r\n");
//	Read_ID_fromROM();	
//	ADC_Init();
//	PWM0_INIT();
//	PCA_INIT();
 P7 =0XFF;

	while(1)
	{
		LED1 = 0;
		Delay100ms();
		Delay100ms();
		Delay100ms();
		Delay100ms();
		Delay100ms();
		LED1 = 1;
		Delay100ms();
		Delay100ms();
		Delay100ms();
		Delay100ms();
		Delay100ms();
//		ADC_RES = Get_ADC_RES();
//		UartSend(ADC_RES);
//		ADC_RES = ADC_RES>>8;
//		UartSend(ADC_RES>>8);
//		UartSend(length);
//		UartSend(length>>8);
//		UartSend(length>>16);
//		UartSend(length>>24);
	}
}
