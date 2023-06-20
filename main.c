#include "LCD12864.h"
#include "DS1302.h"
#include "Timer0.h"
#include "Key.h"
#include "Delay.h"
sbit BEEP = P3 ^ 4;
unsigned char KeyNum, MODE = 0, TimeSetSelect=0, TimeSetFlashFlag = 1,alarmEnabled = 0;
int hour = 20, min = 20;
void alarm(void);
void setAlarm(void);
void TimeSet(void);
void TimeShow(void);
void showHZ(void);
void showMiao();
void showString(unsigned char Line,unsigned char Column,char * a);

void main()
{
	InitLCD();
	DS1302_Init();
  DS1302_SetTime();				// 设置时间
	Timer0_Init();
	showHZ();
	BEEP = 1;
	while (1)
	{
		KeyNum = Key(); // 按键检测
		if (KeyNum == 1)
		{
			if (MODE == 0)
			{
				MODE = 1;
				TimeSetSelect = 0;
				KeyNum = 0;
			} // 功能切换
			else if (MODE == 1)
				{
					TimeSetSelect++;
					TimeSetSelect %= 7;
				}
			else {
				TimeSetSelect = TimeSetSelect == 4 ? 3 : 4;
			}
		}
		if (KeyNum == 2 && MODE == 0)
		{
			MODE = 2;
			TimeSetSelect = 3;
			KeyNum = 0;
		}
		switch (MODE)
		{
		case 0:
			TimeShow();
			break; // 根据不同的功能执行不同的函数
		case 1:
			TimeSet();
			break;
		case 2:
			setAlarm();
			break;
		}
		if (DS1302_Time[3] == hour && DS1302_Time[4] == min && DS1302_Time[5] == 0)
			alarm();
		if (KeyNum == 4 && MODE == 0 ){
			alarmEnabled = !alarmEnabled;
			showMiao();
		}
	}
}

void Timer0_Routine() interrupt 1
{
	static unsigned int n = 0;
	TL0 = 0x18; // 设置定时初始值
	TH0 = 0xFC; // 设置定时初始值
	n++;
	if (n >= 500)
	{
		n = 0;
		TimeSetFlashFlag = !TimeSetFlashFlag;
	}
}
void TimeSet(void)
{
	/*
	if (KeyNum == 1)
	{
		TimeSetSelect++;
		TimeSetSelect %= 7;
	}*/
	if (KeyNum == 3) // 按键3按下
	{
		DS1302_Time[TimeSetSelect]++; // 时间设置位数值加1
		if (DS1302_Time[0] > 99)
		{
			DS1302_Time[0] = 0;
		} // 年越界判断
		if (DS1302_Time[1] > 12)
		{
			DS1302_Time[1] = 1;
		} // 月越界判断
		if (DS1302_Time[1] == 1 || DS1302_Time[1] == 3 || DS1302_Time[1] == 5 || DS1302_Time[1] == 7 ||
			DS1302_Time[1] == 8 || DS1302_Time[1] == 10 || DS1302_Time[1] == 12)
		{
			if (DS1302_Time[2] > 31)
			{
				DS1302_Time[2] = 1;
			}
		} // 大月
		else if (DS1302_Time[1] == 4 || DS1302_Time[1] == 6 ||
				 DS1302_Time[1] == 9 || DS1302_Time[1] == 11)
		{
			if (DS1302_Time[2] > 30)
			{
				DS1302_Time[2] = 1;
			}
		} // 小月
		else if (DS1302_Time[1] == 2)
		{
			if (DS1302_Time[0] % 4 == 0)
			{
				if (DS1302_Time[2] > 29)
				{
					DS1302_Time[2] = 1;
				}
			} // 闰年2月
			else
			{
				if (DS1302_Time[2] > 28)
				{
					DS1302_Time[2] = 1;
				}
			} // 平年2月
		}	  // 日越界判断
		if (DS1302_Time[3] > 23)
		{
			DS1302_Time[3] = 0;
		} // 时越界判断
		if (DS1302_Time[4] > 59)
		{
			DS1302_Time[4] = 0;
		} // 分越界判断
		if (DS1302_Time[5] > 59)
		{
			DS1302_Time[5] = 0;
		} // 秒越界判断
		if (DS1302_Time[6] > 7)
		{
			DS1302_Time[6] = 1;
		} // 星期越界判断
	}
	if (KeyNum == 4 )
	{
		MODE = 0;
		DS1302_SetTime();
		showString(2,7,"DONE");
		KeyNum = 0;
	}
	// 更新显示，根据TimeSetSelect和TimeSetFlashFlag判断可完成闪烁功能
	if (TimeSetSelect == 0 && TimeSetFlashFlag == 1)
	{
		LCD_ShowString(1, 1, "  ");
	}
	else
	{
		LCD_ShowNum(1, 1, DS1302_Time[0], 2);
	}
	if (TimeSetSelect == 1 && TimeSetFlashFlag == 1)
	{
		LCD_ShowString(1, 5, "  ");
	}
	else
	{
		LCD_ShowNum(1, 5, DS1302_Time[1], 2);
	}
	if (TimeSetSelect == 2 && TimeSetFlashFlag == 1)
	{
		LCD_ShowString(1, 9, "  ");
	}
	else
	{
		LCD_ShowNum(1, 9, DS1302_Time[2], 2);
	}
	if (TimeSetSelect == 3 && TimeSetFlashFlag == 1)
	{
		LCD_ShowString(2, 1, "  ");
	}
	else
	{
		LCD_ShowNum(2, 1, DS1302_Time[3], 2);
	}
	if (TimeSetSelect == 4 && TimeSetFlashFlag == 1)
	{
		LCD_ShowString(2, 5, "  ");
	}
	else
	{
		LCD_ShowNum(2, 5, DS1302_Time[4], 2);
	}
	if (TimeSetSelect == 5 && TimeSetFlashFlag == 1)
	{
		LCD_ShowString(2, 9, "  ");
	}
	else
	{
		LCD_ShowNum(2, 9, DS1302_Time[5], 2);
	}
	if (TimeSetSelect == 6 && TimeSetFlashFlag == 1)
	{
		LCD_ShowString(3, 5, " ");
	}
	else
	{
		LCD_ShowNum(3, 5, DS1302_Time[6], 1);
	}
}
void showHZ(){
	Display_HZ(2,0,16,0);	
	Display_HZ(2,0,48,1);	
	Display_HZ(1,0,16,2);
	Display_HZ(2,2,16,3);	
	Display_HZ(2,2,48,4);
	Display_HZ(1,2,16,5);
	Display_HZ(2,4,0,6);		
	Display_HZ(2,4,16,7);
}
void showMiao(){
	unsigned char i;
	alarmEnabled == 1 ? 0 : 1;
	InitLCD();
	Display_HZ(2,2,48,8);
	if (alarmEnabled == 1) {

		LCD_ShowZMS(2,9,"ON");
		for(i=0;i<3;i++){
			BEEP=0;
			Delay(100);
			BEEP=1;
			Delay(100);
		}
	}
	else {
		LCD_ShowZMS(2,9,"OFF");
		BEEP=0;
		Delay(100);
		BEEP=1;
		Delay(100);

	}
	Delay(1000); 
	InitLCD();
	showHZ();
}
void showString(unsigned char Line,unsigned char Column,char * a){
	InitLCD();
	LCD_ShowZMS(Line,Column,a);
	Delay(800);
	InitLCD();
	showHZ();
}
void TimeShow(void) // 时间显示功能
{
	DS1302_ReadTime();					   // 读取时间
	LCD_ShowNum(1, 1, DS1302_Time[0], 2);  // 显示年
	LCD_ShowNum(1, 5, DS1302_Time[1], 2);  // 显示月
	LCD_ShowNum(1, 9, DS1302_Time[2], 2);  // 显示日
	LCD_ShowNum(2, 1, DS1302_Time[3], 2);  // 显示时
	LCD_ShowNum(2, 5, DS1302_Time[4], 2);  // 显示分
	LCD_ShowNum(2, 9, DS1302_Time[5], 2);  // 显示秒
	LCD_ShowNum(3, 5, DS1302_Time[6], 1); // 显示星期
}
void alarm(void)
{
	unsigned char isClick;
	while(1){
		isClick = Key();
		if (isClick == 4)
			break;
		BEEP=0;
		Delay(100);
		BEEP=1;
		Delay(100);
	}
}


float T;
void setAlarm(){
	if (KeyNum == 3)
	{
		if (TimeSetSelect == 3)
		{
			hour++;
			if (hour == 24)
				hour = 0;
		}
		else if (TimeSetSelect == 4)
		{
			min++;
			if (min == 60)
				min = 0;

		}
	}
	if (KeyNum == 4){
		MODE = 0;
		alarmEnabled = 1;
		showString(2,7,"DONE");
		KeyNum = 0;
	}
	if (TimeSetSelect == 3 && TimeSetFlashFlag == 1)
	{
		LCD_ShowString(2, 1, "  ");
	}
	else
	{
		LCD_ShowNum(2, 1, hour, 2);
	}
	if (TimeSetSelect == 4 && TimeSetFlashFlag == 1)
	{
		LCD_ShowString(2, 5, "  ");
	}
	else
	{
		LCD_ShowNum(2, 5, min, 2);
	}
}