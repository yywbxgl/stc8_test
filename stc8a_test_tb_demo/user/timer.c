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
#include "timer.h"

//测试工作频率为11.0592MHz
uint16 Timer_CNT;

void Timer0_Init(void)
{
    TMOD = 0x00;                                //模式0
    TL0 = 0x66;                                 //65536-11.0592M/12/1000	定时1ms
    TH0 = 0xfc;
    TR0 = 1;                                    //启动定时器
    ET0 = 1;                                    //使能定时器中断
    EA = 1;
}

void TM0_Isr() interrupt 1 using 1
{
	Timer_CNT++;
	if(Timer_CNT >= 500)
	{
		Timer_CNT = 0;
		LED = !LED;			 //测试端口
	}                         
}


void TM1_Isr() interrupt 3 using 1
{
    Timer_CNT++;
	if(Timer_CNT >= 500)
	{
		Timer_CNT = 0;
		LED = !LED;			 //测试端口
	}                                  
}

void Timer1_Init(void)
{
    TMOD = 0x00;                                //模式0
    TL1 = 0x66;                                 //65536-11.0592M/12/1000
    TH1 = 0xfc;
    TR1 = 1;                                    //启动定时器
    ET1 = 1;                                    //使能定时器中断
    EA = 1;
}


void TM2_Isr() interrupt 12 using 1
{
    AUXINTIF &= ~T2IF;                          //清中断标志
	Timer_CNT++;
	if(Timer_CNT >= 300)
	{
		Timer_CNT = 0;
		LED = !LED;			 //测试端口
	} 
}

void Timer2_Init(void)
{
    T2L = 0x66;                                 //65536-11.0592M/12/1000
    T2H = 0xfc;
    AUXR = 0x10;                                //启动定时器
    IE2 = ET2;                                  //使能定时器中断
    EA = 1;
}

void TM3_Isr() interrupt 19 using 1
{
    AUXINTIF &= ~T3IF;                          //清中断标志

	Timer_CNT++;
	if(Timer_CNT >= 700)
	{
		Timer_CNT = 0;
		LED = !LED;			 //测试端口
	}
}

void Timer3_Init(void)
{
    T3L = 0x66;                                 //65536-11.0592M/12/1000
    T3H = 0xfc;
    T4T3M = 0x08;                               //启动定时器
    IE2 = ET3;                                  //使能定时器中断
    EA = 1;
}

void TM4_Isr() interrupt 20 using 1
{                                //测试端口
    AUXINTIF &= ~T4IF;                          //清中断标志
	Timer_CNT++;
	if(Timer_CNT >= 1000)
	{
		Timer_CNT = 0;
		LED = !LED;			 //测试端口
	}
}

void Timer4_Init(void)
{
    T4L = 0x66;                                 //65536-11.0592M/12/1000
    T4H = 0xfc;
    T4T3M = 0x80;                               //启动定时器
    IE2 = ET4;                                  //使能定时器中断
    EA = 1;
}








