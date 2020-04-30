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
#include "timer.h"

//���Թ���Ƶ��Ϊ11.0592MHz
uint16 Timer_CNT;

void Timer0_Init(void)
{
    TMOD = 0x00;                                //ģʽ0
    TL0 = 0x66;                                 //65536-11.0592M/12/1000	��ʱ1ms
    TH0 = 0xfc;
    TR0 = 1;                                    //������ʱ��
    ET0 = 1;                                    //ʹ�ܶ�ʱ���ж�
    EA = 1;
}

void TM0_Isr() interrupt 1 using 1
{
	Timer_CNT++;
	if(Timer_CNT >= 500)
	{
		Timer_CNT = 0;
		LED = !LED;			 //���Զ˿�
	}                         
}


void TM1_Isr() interrupt 3 using 1
{
    Timer_CNT++;
	if(Timer_CNT >= 500)
	{
		Timer_CNT = 0;
		LED = !LED;			 //���Զ˿�
	}                                  
}

void Timer1_Init(void)
{
    TMOD = 0x00;                                //ģʽ0
    TL1 = 0x66;                                 //65536-11.0592M/12/1000
    TH1 = 0xfc;
    TR1 = 1;                                    //������ʱ��
    ET1 = 1;                                    //ʹ�ܶ�ʱ���ж�
    EA = 1;
}


void TM2_Isr() interrupt 12 using 1
{
    AUXINTIF &= ~T2IF;                          //���жϱ�־
	Timer_CNT++;
	if(Timer_CNT >= 300)
	{
		Timer_CNT = 0;
		LED = !LED;			 //���Զ˿�
	} 
}

void Timer2_Init(void)
{
    T2L = 0x66;                                 //65536-11.0592M/12/1000
    T2H = 0xfc;
    AUXR = 0x10;                                //������ʱ��
    IE2 = ET2;                                  //ʹ�ܶ�ʱ���ж�
    EA = 1;
}

void TM3_Isr() interrupt 19 using 1
{
    AUXINTIF &= ~T3IF;                          //���жϱ�־

	Timer_CNT++;
	if(Timer_CNT >= 700)
	{
		Timer_CNT = 0;
		LED = !LED;			 //���Զ˿�
	}
}

void Timer3_Init(void)
{
    T3L = 0x66;                                 //65536-11.0592M/12/1000
    T3H = 0xfc;
    T4T3M = 0x08;                               //������ʱ��
    IE2 = ET3;                                  //ʹ�ܶ�ʱ���ж�
    EA = 1;
}

void TM4_Isr() interrupt 20 using 1
{                                //���Զ˿�
    AUXINTIF &= ~T4IF;                          //���жϱ�־
	Timer_CNT++;
	if(Timer_CNT >= 1000)
	{
		Timer_CNT = 0;
		LED = !LED;			 //���Զ˿�
	}
}

void Timer4_Init(void)
{
    T4L = 0x66;                                 //65536-11.0592M/12/1000
    T4H = 0xfc;
    T4T3M = 0x80;                               //������ʱ��
    IE2 = ET4;                                  //ʹ�ܶ�ʱ���ж�
    EA = 1;
}








