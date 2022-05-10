#ifndef __LCD_H_
#define __LCD_H_

extern void delay(unsigned int xms);
extern void CheckState();
extern void SendCommandToLCD(unsigned char com);
extern void SetLine(unsigned char page);
extern void SetStartLine(unsigned char startline);
extern void SetColumn(unsigned char column);
extern void SetOnOff(unsigned char onoff);
extern void WriteByte(unsigned char dat);
extern void SelectScreen(unsigned char screen);
extern void ClearScreen(unsigned char screen);
extern void InitLCD();
extern void DisplayNumber(unsigned char ss,unsigned char page,unsigned char column,unsigned char number);
extern void DisplayWord1(unsigned char ss,unsigned char page,unsigned char column,unsigned char number);
extern void DisplaySolarTerm(unsigned char ss,unsigned char page,unsigned char column,unsigned char number);
extern void Displaytiangan(unsigned char ss,unsigned char page,unsigned char column,unsigned char number);
extern void Displaydizhi(unsigned char ss,unsigned char page,unsigned char column,unsigned char number);
extern void DisplaySunToLunar(unsigned char ss,unsigned char page,unsigned char column,unsigned char number);
#endif