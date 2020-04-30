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
#include "adc.h"

//���Թ���Ƶ��Ϊ11.0592MHz

void ADC_Init(void)
{
    P1M0 = 0x00;                                //����P1.0ΪADC�� ��������
    P1M1 = 0x01;
    ADCCFG = 0x2f;                              //����ADCʱ��Ϊϵͳʱ��/2/16/16 ת������Ҷ���
    ADC_CONTR = 0x80;                           //ʹ��ADCģ�� ��ADC_POWER
}

uint16 Get_ADC_RES(void)
{
	uint16 adc_val;
	
	ADC_CONTR |= 0x40;                      //����ADת��  ADC_START = 1
    _nop_();
    _nop_();
    while (!(ADC_CONTR & 0x20));            //��ѯADC��ɱ�־
    ADC_CONTR &= ~0x20;                     //����ɱ�־

    adc_val = (ADC_RES<<8)|ADC_RESL;						//ȡת�������8λ
	 					//ȡת�������8λ
//	adc_val = adc_val>>4;					//��Ϊ�������

	return adc_val;
}
