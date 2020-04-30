#include "STC8A.H"
#include <intrins.h>


// 外设定义
sbit LED = P2^6;


// 串口1控制
bit busy;  // 串口连续发送等待标记
char wptr; // buffer写指针
char rptr; // buffer读指针
char buffer[16]; //串口数据缓存

// 串口1中断
void UartIsr() interrupt 4
{
	if (TI) // 发送中断
    {
				TI = 0;  //发送中断标志清零
        busy = 0;
    }
    if (RI) //接收中断
    {
        RI = 0;  //接收中断标志清零
        buffer[wptr++] = SBUF; //将接收到的数据放入BUFF缓存中，buff缓存当前指针加1,一次接收一个1个字节
        wptr &= 0x0f;  //如果指针超过buffer大小，写指针wptr=0  重新覆盖
    }
}


// 串口1初始化
void UartInit(void)		//115200bps@11.0592MHz
{
	SCON = 0x50;		//8位数据,可变波特率, 1停止位,无校验位
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0xE8;		  //设定定时初值
	TH1 = 0xFF;		  //设定定时初值
	ET1 = 0;		    //禁止定时器1中断
	TR1 = 1;		    //启动定时器1
	
	//ES = 1;     // 开启串口中断
	//EA = 1;     // 开启总中断
	
	wptr = 0x00;
	rptr = 0x00;
	busy = 0;
}

// 串口2控制
bit busy2;  // 串口连续发送等待标记
char wptr2; // buffer写指针
char rptr2; // buffer读指针
char buffer2[16]; //串口数据缓存

//串口2中断
void Uart2Isr() interrupt 8
{
    if (S2CON & 0x02)
    {
        S2CON &= ~0x02;
        busy2 = 0;
    }
    if (S2CON & 0x01)
    {
        S2CON &= ~0x01;
        buffer2[wptr2++] = S2BUF;
        wptr2 &= 0x0f;
    }
}


// 串口2初始化
void Uart2Init(void)		//115200bps@11.0592MHz
{
	S2CON = 0x50;		//8位数据,可变波特率, 1停止位,无校验位
	AUXR |= 0x14;		//定时器2时钟为Fosc,即1T
	//AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	//TMOD &= 0x0F;		//设定定时器2为16位自动重装方式
	T2L = 0xE8;		  //设定定时器2初值
	T2H = 0xFF;		  //设定定时器2初值
	
  wptr2 = 0x00;
  rptr2 = 0x00;
  busy2 = 0;
}


//串口2发送一个字符
void Uart2Send(char dat)
{
    while (busy2);
    busy2 = 1;
    S2BUF = dat;
}

//串口2发送字符串
void Uart2SendStr(char *p)
{
    while (*p)
    {
        Uart2Send(*p++);
    }
}


//发送一个字符
void UartSend(char dat)
{
	  while (busy); // 等待上一个字符发送完毕
    busy = 1;
	  SBUF = dat;  //往SBUF写数据，TXD引脚自动发送数据，发送完毕触发TI发送中断
}

//发送字符串
void UartSendStr(char *p)
{
    while (*p){
        UartSend(*p++);
    }
}

//延时
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

//定时器0中断
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

//定时器0初始化
void Timer0Init(void)		//1毫秒@24.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x40;		//设置定时初值
	TH0 = 0xA2;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	
	ET0 = 1;   //使能定时器0中断
	EA = 1;    //是能中断
}


// LED测试
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

// 定时器0测试
void test2(void)
{
	Timer0Init();
	while(1);
}


// 掉电唤醒测试
void test3(void)
{
	// 使能掉电唤醒定时器，定时周期约14~15秒
	WKTCL = 0xfe;
	WKTCH = 0xff;  //0xff有问题？
	while(1)
	{
		LED = !LED;
		VOCTRL = 0x00;  // 掉电模式时使用内部SCC模块,功耗约1.5uA
		PCON = PD;      // 进入掉电模式
		_nop_();    //必须包含1~2个NOP，STC8具有迟滞
		_nop_();
	}
}


// 串口通信测试，发送接收到的数据
void test4(void)
{
		UartInit();
    UartSendStr("Uart Test !\r\n");

    while (1)
    {
		if (rptr != wptr)//当buffer里有新的数据未发送
        {
			//发送buffer的1个字节数据，读指针加1
            UartSend(buffer[rptr++]);
				rptr &= 0x0f; //如果指针超过buffer大小，ptr=0 重新覆盖
        }
    }
}

// 切换串口测试
void test5(void)
{
		P_SW1 = 0x40;  //切换串口1到引脚 p3.6_p3.7   //8pin的芯片切换到引脚 p3.1_p3.2
		UartInit();
    UartSendStr("Uart Test !\r\n");
	
		while (1)
    {
			if (rptr != wptr)//当buffer里有新的数据未发送
        {
					  //发送buffer的1个字节数据，读指针加1
            UartSend(buffer[rptr++]);
						rptr &= 0x0f; //如果指针超过buffer大小，ptr=0 重新覆盖
        }
    }
}

// 掉电唤醒 发送指令
void test6(void)
{
	//P_SW1 = 0x40;  //切换串口1到引脚 p3.6_p3.7   //8pin的芯片切换到引脚 p3.1_p3.2
	UartInit();
	
	//WKTCH = 0xff; WKTCL = 0xfe;  // 使能掉电唤醒定时器，定时周期约14~15秒
	WKTCH = 0x8f; 	WKTCL = 0xfe;  // test 2s 
	VOCTRL = 0x00;  // 掉电模式时使用内部SCC模块,功耗约1.5uA

	while(1)
	{
		LED = !LED;
		UartSendStr("Uart Test !\r\n");
		// todo 流程 1. 共MC25供电  2. 初始化MC25 + 获取GNSS + 上报地理 3. 关系MC25
		PCON = PD;      // 进入掉电模式
		_nop_();        // 必须包含1~2个NOP，STC8具有迟滞
		_nop_();
	}
}


// 双串口通信测试
void test7(void)
{
		UartInit();
	  Uart2Init();
		ES = 1;     // 开启串口中断  	
		IE2 |= ES2;
    EA = 1;   // 开启总中断
    
		Uart2SendStr("Uart Test !\r\n");

    while(1)
		{
			UartSendStr("ATI\r\n");
			Uart2SendStr("ATI\r\n");  //打印发送的命令
			
		  if (rptr != wptr)  //如果收到命令回复，打印回复
			{
					Uart2Send(buffer[rptr++]);
					rptr &= 0x0f;
			}
		  Delay100ms();
			Delay100ms();
			Delay100ms();
			Delay100ms();
			Delay100ms();
		}
}


void main(void)
{
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	//test6();
	test7();
}