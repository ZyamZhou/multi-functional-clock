#ifndef __pressKey_H_
#define __pressKey_H_

#include <reg52.h>
#include <intrins.h>
#include <DS1302.h>
#include <LCD.h>

sbit K1=P3^0;  		     //选择要改变的时间参数
sbit K2=P3^1;  		     //设置时间的数据
sbit K5=P1^6;
sbit SET_AM_PM=P1^7;   

extern unsigned char idata SetState;               
extern unsigned char idata SetPlace;
extern unsigned char idata SetAlarm;

extern unsigned char idata alarm[3];
extern unsigned char idata alarm_AM_PM;

   

extern void IntConfiguration();
extern void Delay10ms();
extern void scanButton();
extern void scanButton_alarm();
#endif