#ifndef __LCDNew_H_
#define __LCDNew_H_

extern void LCD12864_Delay1ms(unsigned int n);
extern unsigned char LCD12864_Busy(void);
extern void LCD12864_WriteCmd(unsigned char cmd);
extern void LCD12864_WriteData(unsigned char dat);
extern void LCD12864_Init();
extern void LCDSetPos(unsigned char x, unsigned char y);
extern void DisplayWord(unsigned char *word);

#endif