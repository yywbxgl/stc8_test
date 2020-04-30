/*                        -- 渡河蚂蚁电子工作室 --                        */
/*
*   说    明: CS1180驱动
*   开发平台: STC15W408S     
*   淘宝网店: 
*
//  文件名：uart.h                                                              
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
#ifndef __UART_H
#define __UART_H

#include "config.h"

void Uart1Init(void);
void UartSend(unsigned char dat);
void UART1_IOPORT_CHANGE(void);
void UartSendStr(unsigned char *p);




#endif
