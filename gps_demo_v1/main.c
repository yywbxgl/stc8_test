#include "STC8G.H"
#include <intrins.h>

#define PD          0x02
#define IDL         0x01


// ���趨��
//sbit LED = P2^6;
sbit pwkey = P3^0;
sbit power = P3^1;


// ����1����
bit busy;  // �����������͵ȴ����
unsigned int wptr; // bufferдָ��,��ǰλ��ָ��
# define BUFF_LEN 1024
//char buffer[BUFF_LEN]; //�������ݻ���
//char idata buffer[BUFF_LEN];  //�������ݻ���
char xdata buffer[BUFF_LEN];  //�������ݻ���

//��ս��ջ���
void clear_buffer(void)
{
	unsigned int i = 0;
	for(;i<BUFF_LEN; ++i)
	{
		buffer[i] = 0;
	}
	wptr = 0;
	
}

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
			  if (wptr > BUFF_LEN - 2)
				{
					wptr = 0; //���ָ�볬��buffer��С������ǰ������, ���һλ����Ϊ0.����ѭ�����ͽ���
				}
			  //wptr &= 0x03FE;  
    }
}


// ����1��ʼ��
void UartInit(void)		//115200bps@11.0592MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����, 1ֹͣλ,��У��λ
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����  // Ĭ��Ϊ��ʱ��1
	TMOD &= 0x0F;		//�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
	TL1 = 0xE8;		  //�趨��ʱ��ֵ
	TH1 = 0xFF;		  //�趨��ʱ��ֵ
	ET1 = 0;		    //��ֹ��ʱ��1�ж�
	TR1 = 1;		    //������ʱ��1
		
	wptr = 0x00;
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


// ��ʱ��0�ж�
unsigned int t0_n = 0;
void TM0_Isr() interrupt 1
{
		t0_n ++;
		if (t0_n == 1000)  // ���1��ιһ�ο��Ź� 
		{
			LED = !LED;   
			t0_n = 0;
			WDT_CONTR |= 0x10;  //�忴�Ź�,����ϵͳ��λ
		} 
}

//��ʱ��0��ʼ��
void Timer0Init(void)	 //1����@11.0592MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x66;		  //���ö�ʱ��ֵ
	TH0 = 0xFC;		  //���ö�ʱ��ֵ
	TF0 = 0;		    //���TF0��־
	TR0 = 1;		    //��ʱ��0��ʼ��ʱ
}

void Delay()		//@11.0592MHz
{
	unsigned char i, j, k;

	i = 169;
	j = 80;
	k = 87;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void app()
{
		int begin = 0;
		int end = 0;
		int i =0;
	
	  int temp_h =0 ;
	  int temp_l= 0;
	
	  //LED = 0 ;      // �豸����

		Delay();  // ��ʱ2��
		Delay();

	  
	  UartSendStr("AT+QGNSSC=1\r\n");  //����GNSS
		Delay();
	  UartSendStr("AT+QIFGCNT=0\r\n");  
		Delay();
	  UartSendStr("AT+QICSGP=1,\"CMNET\"\r\n"); 
	  Delay();	
		UartSendStr("AT+QGNSSTS?\r\n"); 
	  Delay();
	
	
	  UartSendStr("AT+QIREGAPP\r\n"); 
	  Delay();
	  UartSendStr("AT+QIACT\r\n"); 
	  Delay();
		UartSendStr("AT+QGAGPS\r\n");  //�ȴ�apgs�����������
	  Delay();
		Delay();
		Delay();
		UartSendStr("AT+QGAGPSAID\r\n");  //ע��agps����
	  Delay();
		Delay();
		Delay();
		UartSendStr("AT+QGNSSRD\r\n"); 
	  Delay();

	
	  UartSendStr("AT+QLBSCFG=\"token\",d73Xq0J9JZ8p4615\r\n"); 
	  Delay();
	  UartSendStr("AT+QLBS\r\n");    //��һ�λ�ȡʱ��Ƚϳ� 
	  Delay();
		Delay();
		Delay();
		Delay();
		
	 
	  UartSendStr("AT+QIOTREG=1\r\n"); 
	  Delay();
		Delay();
		
		// ��ջ���
		clear_buffer();
		UartSendStr("AT+QGNSSRD\r\n"); 
	  Delay();
		Delay();
	
		
		//��ȡ��һ�������Ϣ�ϱ�
		for (;i<BUFF_LEN; ++i)
		{
			if (buffer[i] == '$')
			{
			   begin = i;
				 break;
			}
		}
		
		for (;i < BUFF_LEN; ++i)
		{
			if (buffer[i] == '\r')
			{
			   end = i;
				 break;
			}	
		}

		
		// �ϱ����ݷ�ʽ1
	  UartSendStr("AT+QIOTSEND=0x0001,3,\"323232\"\r\n"); 
	  Delay();
		Delay();
		
		// �ϱ����ݷ�ʽ2
		UartSendStr("AT+QIOTSEND=0x0001\r\n"); 
	  Delay();
		//UartSendStr("\"32323233\"\r\n"); 
		//Delay();
		UartSend('\"');
	  i = begin;
		for (; i<end; ++i)
		{
			//UartSend(buffer[i]);
			temp_h = '0' + buffer[i]/16;
			temp_l = '0' + buffer[i]%16;
			if (temp_h > '9') 
				temp_h = temp_h + 7;
			if (temp_l > '9')
				temp_l = temp_l +7;
			UartSend(temp_h);
			UartSend(temp_l);
		}
		UartSendStr("\"\r\n");
		
		Delay();		
	
		//LED = 1 ;  // �豸�ϵ�
		
}

char up_cont = 0 ; // ��¼��ǰ���ѵĴ���
void test7(void)
{
  //P_SW1 = 0x40;  //�л�����1������ p3.6_p3.7   //stc8g-8pin��оƬ�л������� p3.1_p3.2
	UartInit();
	Timer0Init();
	
	ES = 1;     // ���������ж�
	ET0 = 1;    // ʹ�ܶ�ʱ��0�ж�
	EA = 1;     // �������ж�

	
	//WKTCH = 0xff; WKTCL = 0xfe;  // ʹ�ܵ��绽�Ѷ�ʱ������ʱ����Լ14~15��
	WKTCH = 0x8f; 	WKTCL = 0xfe;  // test 2s 
	//VOCTRL = 0x00;  // ����ģʽʱʹ���ڲ�SCCģ��,����Լ1.5uA  Ĭ��ֵ0  stc8g���߱��˼Ĵ���

	// ������������һ��ҵ��
	//UartSendStr("system inint success!\r\n");
	
	//WDT_CONTR = 0x23;                           //ʹ�ܿ��Ź�,���ʱ��ԼΪ0.5s
  //WDT_CONTR = 0x24;                           //ʹ�ܿ��Ź�,���ʱ��ԼΪ1s
  WDT_CONTR = 0x27;                             //ʹ�ܿ��Ź�,���ʱ��ԼΪ8s��
	
	
	app();
	
	while(1)
	{
		//UartSendStr("wake up !\r\n");
	  
		up_cont ++;        //���Ѵ�����1
		if (up_cont == 30)  //���Ѵﵽһ���Ļ��Ѵ���������ҵ��
		{
			// todo ���� 1. ��MC25����  2. ��ʼ��MC25 + ��ȡGNSS + �ϱ����� 3. �ػ�MC25
			app();
			up_cont = 0;
		}

		PCON = PD;    // �������ģʽ   CPU�������ֹͣ
		//PCON = IDL;    // �������ģʽ  CPUֹͣ����  �����������
		_nop_();        // �������1~2��NOP��STC8���г���
		_nop_();
	}
}



void main(void)
{
  test7();
}