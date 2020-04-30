#include "STC8G.H"
#include <intrins.h>


#define IDL  0x01 //PCON.0 
#define PD  0x02 //PCON.1 


// ���趨��
sbit LED = P2^6;

// ���ڿ���
bit busy;  // �����������͵ȴ����
char wptr; // bufferдָ��
char rptr; // buffer��ָ��
char buffer[16]; //�������ݻ���

// ����1�ж�
void UartIsr() interrupt 4
{
	if (TI) // �����ж�
    {
				TI = 0;  //�����жϱ�־����
        busy = 0;
    }
    if (RI) //�����ж�
    {
        RI = 0;  //�����жϱ�־����
        buffer[wptr++] = SBUF; //�����յ������ݷ���BUFF�����У�buff���浱ǰָ���1,һ�ν���һ��1���ֽ�
        wptr &= 0x0f;  //���ָ�볬��buffer��С��дָ��wptr=0  ���¸���
    }
}


// ���ڳ�ʼ��
void UartInit(void)		//115200bps@11.0592MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����, 1ֹͣλ,��У��λ
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
	TL1 = 0xE8;		  //�趨��ʱ��ֵ
	TH1 = 0xFF;		  //�趨��ʱ��ֵ
	ET1 = 0;		    //��ֹ��ʱ��1�ж�
	TR1 = 1;		    //������ʱ��1
	
	ES = 1;     // ���������ж�
	EA = 1;     // �������ж�
	
	wptr = 0x00;
	rptr = 0x00;
	busy = 0;
}

//����һ���ַ�
void UartSend(char dat)
{
	  while (busy); // �ȴ���һ���ַ��������
    busy = 1;
	  SBUF = dat;  //��SBUFд���ݣ�TXD�����Զ��������ݣ�������ϴ���TI�����ж�
}

//�����ַ���
void UartSendStr(char *p)
{
    while (*p){
        UartSend(*p++);
    }
}

//��ʱ
void Delay100ms()		//@24.000MHz
{
	unsigned char i, j, k;
	_nop_();
	_nop_();
	i = 13;
	j = 45;
	k = 214;
	do{
		do{
			while (--k);
		} while (--j);
	} while (--i);
}

//��ʱ��0�ж�
unsigned int t0_n = 0;
void TM0_Isr() interrupt 1
{
		t0_n ++;
		if (t0_n == 500) 
		{
			LED = !LED;   
			t0_n = 0;
		}
}

//��ʱ��0��ʼ��
void Timer0Init(void)		//1����@24.000MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x40;		//���ö�ʱ��ֵ
	TH0 = 0xA2;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	
	ET0 = 1;   //ʹ�ܶ�ʱ��0�ж�
	EA = 1;    //�����ж�
}


// LED����
void test1(void)
{
	while(1){
		LED = 0;
		Delay100ms();
		Delay100ms();
		Delay100ms();
		Delay100ms();
		Delay100ms();
		LED = 1;
		Delay100ms();
		Delay100ms();
		Delay100ms();
		Delay100ms();
		Delay100ms();
	}
}

// ��ʱ��0����
void test2(void)
{
	Timer0Init();
	while(1);
}


// ���绽�Ѳ���
void test3(void)
{
	// ʹ�ܵ��绽�Ѷ�ʱ������ʱ����Լ14~15��
	WKTCL = 0xfe;
	WKTCH = 0xff;  //0xff�����⣿
	while(1)
	{
		LED = ~LED;
		//VOCTRL = 0x00;  // ����ģʽʱʹ���ڲ�SCCģ��,����Լ1.5uA  //stc8g�޴˹���
		PCON = PD;      // �������ģʽ
		_nop_();    //�������1~2��NOP��STC8���г���
		_nop_();
	}
}


// ����ͨ�Ų��ԣ����ͽ��յ�������
void test4(void)
{
		UartInit();
    UartSendStr("Uart Test !\r\n");

    while (1)
    {
			if (rptr != wptr)//��buffer�����µ�����δ����
        {
					  //����buffer��1���ֽ����ݣ���ָ���1
            UartSend(buffer[rptr++]);
						rptr &= 0x0f; //���ָ�볬��buffer��С��ptr=0 ���¸���
        }
    }
}

// �л����ڲ���
void test5(void)
{
		P_SW1 = 0x40;  //�л�����1������ p3.6_p3.7   //8pin��оƬ�л������� p3.1_p3.2
		UartInit();
    UartSendStr("Uart Test !\r\n");
	
		while (1)
    {
			if (rptr != wptr)//��buffer�����µ�����δ����
        {
					  //����buffer��1���ֽ����ݣ���ָ���1
            UartSend(buffer[rptr++]);
						rptr &= 0x0f; //���ָ�볬��buffer��С��ptr=0 ���¸���
        }
    }
}

// ���绽�� ����ָ��
void test6(void)
{
	//P_SW1 = 0x40;  //�л�����1������ p3.6_p3.7   //8pin��оƬ�л������� p3.1_p3.2
	UartInit();
	
	//WKTCH = 0xff; WKTCL = 0xfe;  // ʹ�ܵ��绽�Ѷ�ʱ������ʱ����Լ14~15��
	WKTCH = 0x8f; 	WKTCL = 0xfe;  // test 2s 
	//VOCTRL = 0x00;  // ����ģʽʱʹ���ڲ�SCCģ��,����Լ1.5uA

	while(1)
	{
		LED = ~LED;
		UartSendStr("Uart Test !\r\n");
		// todo ���� 1. ��MC25����  2. ��ʼ��MC25 + ��ȡGNSS + �ϱ����� 3. ��ϵMC25
		PCON = PD;      // �������ģʽ
		_nop_();        // �������1~2��NOP��STC8���г���
		_nop_();
	}
}


void main(void)
{
	//test1();
	//test2();
	//test3();
	test4();
	//test5();
	//test6();
}