#ifndef __lunarCalendar_H_
#define __lunarCalendar_H_

extern unsigned char  idata code1,code2,code3,code4;
extern unsigned char idata SolarTerm[24];

extern unsigned char idata lunar_month;
extern unsigned char idata lunar_date;
extern unsigned char idata sign[13];
extern unsigned int idata symbol;

extern unsigned char convertBCDtoHex(unsigned char BCD);
extern unsigned char HexDoDex(unsigned char hex);
extern void calcSolarTermInit(unsigned char year);
extern void convert(unsigned char year,unsigned char month,unsigned char date);


#endif