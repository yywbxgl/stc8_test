#include "STC8A.H"
#include <intrins.h>


// 外设定义
sbit LED = P2^6;
sbit pwkey = P3^3;   // mc25 pwdkey键盘
sbit power = P3^4;   // mc25供电


// 串口1控制
bit busy;  // 串口连续发送等待标记
unsigned int wptr; // buffer写指针,当前位置指针
# define BUFF_LEN 1024
//char buffer[BUFF_LEN]; //串口数据缓存
//char idata buffer[BUFF_LEN];  //串口数据缓存
char xdata buffer[BUFF_LEN];  //串口数据缓存

//清空接收换冲
void clear_buffer(void)
{
	unsigned int i = 0;
	for(;i<BUFF_LEN; ++i)
	{
		buffer[i] = 0;
	}
	wptr = 0;
}


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
		if (wptr > BUFF_LEN - 2)
		{
			wptr = 0; //如果指针超过buffer大小，覆盖前面数据, 最后一位必须为0.用于循环发送结束
		}
			  //wptr &= 0x03FE;  
    }
}


// 串口1初始化
void UartInit(void)		//115200bps@11.0592MHz
{
	SCON = 0x50;		//8位数据,可变波特率, 1停止位,无校验位
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器  // 默认为定时器1
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0xE8;		  //设定定时初值
	TH1 = 0xFF;		  //设定定时初值
	ET1 = 0;		    //禁止定时器1中断
	TR1 = 1;		    //启动定时器1
		
	wptr = 0x00;
	busy = 0;
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


// 定时器0中断
unsigned int t0_n = 0;
void TM0_Isr() interrupt 1
{
	t0_n ++;
	if (t0_n == 1000)  // 间隔1秒喂一次看门狗 
	{
		LED = !LED;   
		t0_n = 0;
		WDT_CONTR |= 0x10;  //清看门狗,否则系统复位
	} 
}

//定时器0初始化
void Timer0Init(void)	 //1毫秒@11.0592MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x66;		  //设置定时初值
	TH0 = 0xFC;		  //设置定时初值
	TF0 = 0;		    //清除TF0标志
	TR0 = 1;		    //定时器0开始计时
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

//发送前100Byte 数据
void send_buffer_100(void)
{
	int i = 0;
	int temp_h = 0;
	int temp_l = 0;
	UartSendStr("AT+QIOTSEND=0x0001,100,\""); 
	for (;i<100;++i)
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
	
	clear_buffer();
}

//发送当前所有数据
void send_buffer(void)
{
	int i =0;
	
	UartSendStr("AT+QIOTSEND=0x0001\r\n");  // 上报数据方式2  不指定数据长度
	Delay();   // 延迟

	while(buffer[i] != 0)
	{
		UartSend(buffer[i]);
		++i;
	}
	UartSend(0x1A); // crtl+z = 0x1A 结束符 发送发送数据
	UartSendStr("\r\n");
	
	clear_buffer();
}

void app()
{
	int begin = 0;
	int end = 0;
	int i =0;
	int n=0;
	
	int temp_h =0 ;
	int temp_l= 0;
	
//		LED = 0 ;      // 设备供电
//		power = 1;
//	    pwkey = 0;
//		Delay();
//		pwkey = 1;  // pwkey拉低2秒
//		Delay();  
//		Delay();
//		pwkey = 0;   // 开机后等待4秒
	Delay();
	Delay();
	Delay();
	Delay();
	
	clear_buffer();

	UartSendStr("ATE0\r\n"); 
	Delay();
	
	UartSendStr("AT+QGNSSC=1\r\n");  //开启GNSS
	Delay();
	Delay();
	UartSendStr("AT+QIFGCNT=0\r\n");  
	Delay();
	UartSendStr("AT+QICSGP=1,\"CMNET\"\r\n"); 
	Delay();
	Delay();	
	UartSendStr("AT+QGNSSTS?\r\n");  //同步时间
	Delay();
	UartSendStr("AT+QIREGAPP\r\n");  //未知命令
	Delay();
	UartSendStr("AT+QIACT\r\n");  // 未知命令
	Delay();
	UartSendStr("AT+QGAGPS\r\n");    //等待apgs数据下载完毕
	Delay();
	Delay();
	Delay();
	Delay();
	UartSendStr("AT+QGAGPSAID\r\n");  //注入agps数据
	Delay();
	Delay();
	Delay();
	//UartSendStr("AT+QGNSSRD?\r\n"); 
	//Delay();


	UartSendStr("AT+QIOTREG=1\r\n");  //注册设备到云平台
	Delay();
	Delay();
	Delay();
	UartSendStr("ATI\r\n"); 
	Delay();
	// send_buffer();
	send_buffer_100();
	
			
	//    上报数据方式1
	//	  UartSendStr("AT+QIOTSEND=0x0001,3,\"323232\"\r\n"); 
	//	  Delay();
	//    Delay();
		
		
	clear_buffer();   // 清空缓存
	UartSendStr("AT+QGNSSRD?\r\n");  //获取GPS信息
	Delay();
	Delay();
	
		
	//获取第一行地理信息上报
	i=0;
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
		
		
	// 上报GPS数据
	UartSendStr("AT+QIOTSEND=0x0001\r\n"); 
	Delay();
	i = begin;
	for (; i<end; ++i)
	{
		  UartSend(buffer[i]);
	}
	UartSend(0x1A); // crtl+z = 0x1A 结束符 发送发送数据
	UartSendStr("\r\n");
		
	Delay();
	Delay();
	Delay();
		
		
	UartSendStr("AT+QLBSCFG=\"token\",d73Xq0J9JZ8p4615\r\n"); 
	Delay();
	clear_buffer();  // 清空缓存  获取Qlocal信息
	UartSendStr("AT+QLBS\r\n"); 
	Delay();
	Delay();
	Delay();
	Delay();
	//send_buffer_100();
	send_buffer();
		
	//LED = 1 ;  // 设备断电
	Delay();
	Delay();	
}

char up_cont = 0 ; // 记录当前唤醒的次数
void test7(void)
{
	// IO初始化  配置为双向输入  stc8g默认为高阻态

	//P3M0 = 0x00;
	//P3M1 = 0x00;

	
	P_SW1 = 0x40;  //切换串口1到引脚 p3.6_p3.7   //stc8g-8pin的芯片切换到引脚 p3.1_p3.2
	UartInit();
	Timer0Init();
	
	ES = 1;     // 开启串口中断
	ET0 = 1;    // 使能定时器0中断
	EA = 1;     // 开启总中断

	
	//WKTCH = 0xff; WKTCL = 0xfe;  // 使能掉电唤醒定时器，定时周期约14~15秒
	WKTCH = 0x8f; 	WKTCL = 0xfe;  // test 2s 
	//VOCTRL = 0x00;  // 掉电模式时使用内部SCC模块,功耗约1.5uA  默认值0  stc8g不具备此寄存器

	// 启动后先运行一次业务
	//UartSendStr("system inint success!\r\n");
	
	//WDT_CONTR = 0x23;                           //使能看门狗,溢出时间约为0.5s
    //WDT_CONTR = 0x24;                           //使能看门狗,溢出时间约为1s
	WDT_CONTR = 0x27;                             //使能看门狗,溢出时间约为8s、
	
	
	app();
	
	while(1)
	{
		//UartSendStr("wake up !\r\n");
	  
		up_cont ++;
		if (up_cont == 30)  //唤醒次数累计到半小时，运行业务
		{
			app();
			up_cont = 0;
		}

		PCON = PD;    // 进入掉电模式   CPU和外设均停止
		//PCON = IDL;    // 进入空闲模式  CPU停止工作  外设继续运行
		_nop_();        // 必须包含1~2个NOP，STC8具有迟滞
		_nop_();
	}
}



void main(void)
{
  test7();
}