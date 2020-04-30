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
#ifndef _CONFIG_H
#define _CONFIG_H

/* 通用头文件 */
#include <STC8A.h>
#include <intrins.h>

#include "uart.h"
#include "stc8aid.h"
#include "delay.h"
#include "IO.h"
#include "timer.h"
#include "adc.h"
#include "pwm.h"
#include "pca.h"

/* 数据类型定义 */
typedef  signed    char    int8;    // 8位有符号整型数
typedef  signed    int     int16;   //16位有符号整型数
typedef  signed    long    int32;   //32位有符号整型数
typedef  unsigned  char    uint8;   // 8位无符号整型数
typedef  unsigned  int     uint16;  //16位无符号整型数
typedef  unsigned  long    uint32;  //32位无符号整型数

/* 全局运行参数定义 */
#define FOSC   11059200L  //系统主时钟频率，即振荡器频率÷12
#define	BRT	(65536 - FOSC / 115200 / 4)

#define F125KHZ (65536-(FOSC/2/125000/12))


#define PGACOUNT      0x07

/* IO引脚分配定义 */

sbit LED = P5^4;
sbit LED1 = P2^6;

#endif