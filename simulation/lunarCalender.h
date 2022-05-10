#ifndef __lunarCalendar_H_
#define __lunarCalendar_H_

extern unsigned char code1,code2,code3,code4;
extern unsigned char SolarTerm[24];

extern unsigned char lunar_month;
extern unsigned char lunar_date;
extern unsigned char sign[13];
extern unsigned int symbol;
extern unsigned char SolarTerm[24];

extern unsigned char convertBCDtoHex(unsigned char BCD);
extern unsigned char HexDoDex(unsigned char hex);
extern void calcSolarTermInit(unsigned char year);
extern void convert(unsigned char year,unsigned char month,unsigned char date);






#endif