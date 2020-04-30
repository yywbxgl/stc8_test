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
#include "adc.h"

//测试工作频率为11.0592MHz

void ADC_Init(void)
{
    P1M0 = 0x00;                                //设置P1.0为ADC口 高阻输入
    P1M1 = 0x01;
    ADCCFG = 0x2f;                              //设置ADC时钟为系统时钟/2/16/16 转换结果右对齐
    ADC_CONTR = 0x80;                           //使能ADC模块 打开ADC_POWER
}

uint16 Get_ADC_RES(void)
{
	uint16 adc_val;
	
	ADC_CONTR |= 0x40;                      //启动AD转换  ADC_START = 1
    _nop_();
    _nop_();
    while (!(ADC_CONTR & 0x20));            //查询ADC完成标志
    ADC_CONTR &= ~0x20;                     //清完成标志

    adc_val = (ADC_RES<<8)|ADC_RESL;						//取转换结果高8位
	 					//取转换结果低8位
//	adc_val = adc_val>>4;					//因为是左对齐

	return adc_val;
}
