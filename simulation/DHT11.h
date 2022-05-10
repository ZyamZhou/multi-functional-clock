#ifndef __DHT11_H_
#define __DHT11_H_

#include <reg52.h>
#include <intrins.h>
#include <LCD.h>

sbit Bus=P2^3;//数据总线

extern void Delay10us(void);
extern void Delay20ms(void);
extern void Delay1s(void);
extern unsigned char Read_Temp();
extern void DisplayTemp();
extern void DHT11_Init();
extern void DHT11();


#endif