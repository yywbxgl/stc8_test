/*                        -- 渡河蚂蚁电子工作室 --                        */
/*
*   说    明: STC8A8KS4A12 DEMO程序
*   开发平台: STC15W408S     
*   淘宝网店: 
*
//  文件名：main.c                                                              
//  说明：供客户测试模块通信使用程序                                                                  
//  编写人员：Duhemayi                                                                   
//  编写日期：2018-09-16                                                               
//  程序维护：
//  维护记录：
//	版    本: V1.0
//                                                          
// 免责声明：该程序仅用于学习与交流 
// (c) Duhemayi Corporation. All rights reserved.     
******************************************************************************/
#include "config.h"

/******************************************************************************/
// 函数名称：main 
// 输入参数： 
// 输出参数： 
// 函数功能： 
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

