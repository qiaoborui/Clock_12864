#ifndef __12864_H__
#define __12864_H__

#include <regx51.h>

#define uint unsigned int
#define uchar unsigned char
	
#define DATA P2 //LCD12864数据线
sbit RS=P1^2; // 数据、指令 选择
sbit RW=P1^1; // 读、写选择
sbit EN=P1^0; // 读、写使能，下降沿
sbit cs1=P1^4; // 片选1
sbit cs2=P1^3; // 片选2	
//字模数组
uchar code Hzk[];
uchar code Szk[];
uchar code Fhk[];
uchar code Zmk[];

void CheckState();
void SendCommandToLCD(uchar com);
void SetLine(uchar page);
void SetStartLine(uchar startline);
void SetColumn(uchar column);
void SetOnOff(uchar onoff);
void WriteByte(uchar dat)	;
void SelectScreen(uchar screen);
void ClearScreen(uchar screen);	
void InitLCD() ;
void Display_HZ(uchar screen,uchar page,uchar column,uchar number);
void Display_SZ(uchar screen,uchar page,uchar column,uchar number);
void Display_ZM(uchar screen,uchar page,uchar column,uchar number);
void Display_FH(uchar screen,uchar page,uchar column,uchar number);
void LCD_ShowNum(uchar Line,uchar Column,uint Number,uchar Length);
void LCD_ShowString(uchar Line,uchar Column,char *String);
void LCD_ShowHZ(uchar Line,uchar Column,uchar HZ);
#endif