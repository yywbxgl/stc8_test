/*                        -- 渡河蚂蚁电子工作室 --                        */
/*
*   说    明: STC8A8KS4A12 DEMO程序
*   开发平台: STC15W408S     
*   淘宝网店: 
*
//  文件名：PWM.h                                                              
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
#include "pwm.h"

//测试工作频率为11.0592MHz

/******************************************************************************/
// 函数名称：void PWM0_INIT(void) 
// 输入参数： 
// 输出参数： 
// 函数功能：
// 备    注：周期的算法，1/晶体频率*计数周期 1/11059200*4096 ＝ 370ms 
/******************************************************************************/
void PWM0_INIT(void)
{
    P_SW2 = 0x80;
    PWMCKS = 0x00;                              // PWM时钟为系统时钟
    PWMC = CYCLE;                               //设置PWM周期为
    PWM0T1= 0x0000;
    PWM0T2= 0x0001;
    PWM0CR= 0x80;                               //使能PWM0输出
    P_SW2 = 0x00;

    PWMCR = 0xc0;                               //启动PWM模块
    EA = 1;
}

void PWM_Isr() interrupt 22 using 1
{
    static bit dir = 1;
    static int val = 0;

    if (PWMCFG & 0x80)							//当计数器归零，产生中断
    {
        PWMCFG &= ~0x80;                        //清中断标志
        if (dir)
        {
            val++;
            if (val >= CYCLE) dir = 0;
        }
        else
        {
            val--;
            if (val <= 1) dir = 1;
        }
        _push_(P_SW2);
        P_SW2 |= 0x80;
        PWM0T2 = val;
        _pop_(P_SW2);
    }
}


