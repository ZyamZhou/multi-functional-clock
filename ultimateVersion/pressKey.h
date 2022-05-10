#ifndef __pressKey_H_
#define __pressKey_H_

#include <reg52.h>
#include <intrins.h>
#include <DS1302.h>
#include <LCDNew.h>

sbit K1=P1^2;  		     //ѡ��Ҫ�ı��ʱ�����	   SW1
sbit K2=P1^3;  		     //����ʱ������� �Ӱ���     SW2
sbit K5=P1^4;			 //������	SW4
sbit SET_AM_PM=P1^6;     //SW6
sbit beep=P1^5;          //BEEP��   

extern unsigned char idata SetState;               
extern unsigned char idata SetPlace;
extern unsigned char idata SetAlarm;

extern unsigned char idata alarm[3];
extern unsigned char idata alarm_AM_PM;

extern void Delay10ms();
extern void scanButton();
extern void scanButton_alarm();
extern void DisplayAlarmButton(unsigned char SetPlace);
extern void DisplayButton(unsigned char SetPlace);
extern unsigned char BCDToDex(unsigned char num);
extern unsigned char DexToBCD(unsigned char num);
#endif