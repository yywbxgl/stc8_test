/*                        -- �ɺ����ϵ��ӹ����� --                        */
/*
*   ˵    ��: CS1180����
*   ����ƽ̨: STC15W408S     
*   �Ա�����: 
*
//  �ļ�����config.h                                                              
//  ˵�������ͻ����Ե�Ƭ���ĸ����������                                                                 
//  ��д��Ա��Duhemayi                                                                   
//  ��д���ڣ�2018-09-16                                                               
//  ����ά����
//  ά����¼��
//	��    ��: V1.0
//                                                          
// �����������ó��������ѧϰ�뽻�� 
// (c) Duhemayi Corporation. All rights reserved.     
******************************************************************************/
#ifndef _CONFIG_H
#define _CONFIG_H

/* ͨ��ͷ�ļ� */
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

/* �������Ͷ��� */
typedef  signed    char    int8;    // 8λ�з���������
typedef  signed    int     int16;   //16λ�з���������
typedef  signed    long    int32;   //32λ�з���������
typedef  unsigned  char    uint8;   // 8λ�޷���������
typedef  unsigned  int     uint16;  //16λ�޷���������
typedef  unsigned  long    uint32;  //32λ�޷���������

/* ȫ�����в������� */
#define FOSC   11059200L  //ϵͳ��ʱ��Ƶ�ʣ�������Ƶ�ʡ�12
#define	BRT	(65536 - FOSC / 115200 / 4)

#define F125KHZ (65536-(FOSC/2/125000/12))


#define PGACOUNT      0x07

/* IO���ŷ��䶨�� */

sbit LED = P5^4;
sbit LED1 = P2^6;

#endif