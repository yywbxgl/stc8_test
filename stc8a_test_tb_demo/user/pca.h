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
#ifndef _PCA_H
#define _PCA_H

#include "config.h"
//unsigned char cnt;                              //存储PCA计时溢出次数
//unsigned long count0;                           //记录上一次的捕获值
//unsigned long count1;                           //记录本次的捕获值
extern unsigned long length;                           //存储信号的时间长度

void PCA_INIT(void);

#endif