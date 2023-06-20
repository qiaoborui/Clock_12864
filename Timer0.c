#include <REGX51.H>

void Timer0_Init()		//1毫秒@12.000MHz
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x18;		//设置定时初始值
	TH0 = 0xFC;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0=1;
	PT0=0;
	EA=1;
}


/* 定时器中断模板
void Timer0_Routine() interrupt 1
{
	static unsigned int n=0;
	TL0 = 0x18;		//设置定时初始值
	TH0 = 0xFC;		//设置定时初始值
 	n++;
	if(n>=1000)
	{
		n=0;
	}
}
*/