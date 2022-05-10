#ifndef __DS1302_H_
#define __DS1302_H_

//---包含头文件---//
#include<reg52.h>
#include<intrins.h>

sbit RST=P2^7;
sbit SCLK=P2^5;
sbit DSIO=P2^6;

extern unsigned char year;
extern unsigned char hour;
extern unsigned char day;
extern unsigned char month;
extern unsigned char date;
extern unsigned char minute;
extern unsigned char second;
extern unsigned char AM_PM;
extern unsigned char code WRITE_RTC_ADDR[7];
extern unsigned char TIME[7];

extern void Ds1302Write(unsigned char, unsigned char dat);
extern unsigned char Ds1302Read(unsigned char addr);
extern void Ds1302Init();
extern void Ds1302ReadTime();

#endif