/*                        -- 渡河蚂蚁电子工作室 --                        */
/*
*   说    明: CS1180驱动
*   开发平台: STC15W408S     
*   淘宝网店: 
*
//  文件名：config.h                                                              
//  说明：供客户测试单片机的各项基本功能                                                                 
//  编写人员：Duhemayi                                                                   
//  编写日期：2018-09-16                                                               
//  程序维护：
//  维护记录：
//	版    本: V1.0
//                                                          
// 免责声明：该程序仅用于学习与交流 
// (c) Duhemayi Corporation. All rights reserved.     
******************************************************************************/
//测试工作频率为11.0592MHz
#include "pca.h"

unsigned char cnt;                              //存储PCA计时溢出次数
unsigned long count0;                           //记录上一次的捕获值
unsigned long count1;                           //记录本次的捕获值
unsigned long length;                           //存储信号的时间长度

/******************************************************************************/
// 函数名称：void PCA_INIT(void) 
// 输入参数： 
// 输出参数： 
// 函数功能：
// 备    注：测量脉冲的宽度	，测量50HZ 25HZ 500HZ 123HZ 5KHZ还是很准确的
/******************************************************************************/
void PCA_INIT(void)
{
    cnt = 0;                                    //用户变量初始化
    count0 = 0;
    count1 = 0;
    length = 0;
    CCON = 0x00;								//清PCA中断溢出标志位，停止计数，清模块中断溢出标志位
    CMOD = 0x09;                                //PCA时钟为系统时钟,使能PCA计时中断
    CL = 0x00;
    CH = 0x00;
//    CCAPM0 = 0x11;                              //PCA模块0为16位捕获模式（下降沿捕获）
    CCAPM0 = 0x21;                              //PCA模块0为16位捕获模式（上升沿捕获）
//    CCAPM0 = 0x31;                              //PCA模块0为16位捕获模式（上升沿下降沿捕获）
    CCAP0L = 0x00;
    CCAP0H = 0x00;
    CR = 1;                                     //启动PCA计时器
    EA = 1;
}
/*脉冲宽度的算法：
(65536*cnt+CCAP0H:CCAP0L)/11059200 = 脉冲宽度
*/
void PCA_Isr() interrupt 7 using 1
{
    if (CF)										//PCA计数器溢出中断
    {
        CF = 0;
        cnt++;                                  //PCA计时溢出次数+1
    }
    if (CCF0)									//PCA模块溢出中断
    {
        CCF0 = 0;
        count0 = count1;                        //备份上一次的捕获值
        ((unsigned char *)&count1)[3] = CCAP0L;
        ((unsigned char *)&count1)[2] = CCAP0H;
        ((unsigned char *)&count1)[1] = cnt;
        ((unsigned char *)&count1)[0] = 0;
        length = count1 - count0;              //length保存的即为捕获的脉冲宽度
    }
}