/*变量定义*/
#include <reg52.h>
#include <intrins.h>
#include <lunarCalender.h>

#define uint unsigned int
#define uchar unsigned char 

#define DATA P0
sbit RS=P2^2;
sbit RW=P2^1;
sbit EN=P2^0;
sbit cs1=P2^3;
sbit cs2=P2^4;

uchar code words[]={
/*--  文字:  年 0 --*/
0x00,0x20,0x18,0xC7,0x44,0x44,0x44,0x44,0xFC,0x44,0x44,0x44,0x44,0x04,0x00,0x00,
0x04,0x04,0x04,0x07,0x04,0x04,0x04,0x04,0xFF,0x04,0x04,0x04,0x04,0x04,0x04,0x00,
/*--  文字:  月 1 --*/
0x00,0x00,0x00,0xFE,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0xFE,0x00,0x00,0x00,
0x80,0x40,0x30,0x0F,0x02,0x02,0x02,0x02,0x02,0x02,0x42,0x82,0x7F,0x00,0x00,0x00,
/*--  文字:  日 2 --*/
0x00,0x00,0x00,0xFE,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0xFE,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0xFF,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0xFF,0x00,0x00,0x00,0x00,
/*--  文字:  时 3 --*/
0x00,0xFC,0x84,0x84,0x84,0xFC,0x00,0x10,0x10,0x10,0x10,0x10,0xFF,0x10,0x10,0x00,
0x00,0x3F,0x10,0x10,0x10,0x3F,0x00,0x00,0x01,0x06,0x40,0x80,0x7F,0x00,0x00,0x00,
/*--  文字:  分 4 --*/
0x80,0x40,0x20,0x90,0x88,0x86,0x80,0x80,0x80,0x83,0x8C,0x10,0x20,0x40,0x80,0x00,
0x00,0x80,0x40,0x20,0x18,0x07,0x00,0x40,0x80,0x40,0x3F,0x00,0x00,0x00,0x00,0x00,
/*--  文字:  秒 5--*/
0x24,0x24,0xA4,0xFE,0x23,0x22,0x00,0xC0,0x38,0x00,0xFF,0x00,0x08,0x10,0x60,0x00,
0x08,0x06,0x01,0xFF,0x01,0x06,0x81,0x80,0x40,0x40,0x27,0x10,0x0C,0x03,0x00,0x00,
/*--  文字:  星 6 --*/
0x00,0x00,0x00,0xBE,0x2A,0x2A,0x2A,0xEA,0x2A,0x2A,0x2A,0x3E,0x00,0x00,0x00,0x00,
0x00,0x44,0x42,0x49,0x49,0x49,0x49,0x7F,0x49,0x49,0x49,0x49,0x41,0x40,0x00,0x00,
/*--  文字:  期 7--*/
0x00,0x04,0xFF,0x24,0x24,0x24,0xFF,0x04,0x00,0xFE,0x22,0x22,0x22,0xFE,0x00,0x00,
0x88,0x48,0x2F,0x09,0x09,0x19,0xAF,0x48,0x30,0x0F,0x02,0x42,0x82,0x7F,0x00,0x00,
/*--  文字:  一  --*/
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/*--  文字:  二  --*/
0x00,0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00,0x00,
0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,
/*--  文字:  三  --*/
0x00,0x04,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x04,0x00,0x00,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00,
/*--  文字:  四  --*/
0x00,0xFC,0x04,0x04,0x04,0xFC,0x04,0x04,0x04,0xFC,0x04,0x04,0x04,0xFC,0x00,0x00,
0x00,0x7F,0x28,0x24,0x23,0x20,0x20,0x20,0x20,0x21,0x22,0x22,0x22,0x7F,0x00,0x00,
/*--  文字:  五  --*/
0x00,0x02,0x42,0x42,0x42,0xC2,0x7E,0x42,0x42,0x42,0x42,0xC2,0x02,0x02,0x00,0x00,
0x40,0x40,0x40,0x40,0x78,0x47,0x40,0x40,0x40,0x40,0x40,0x7F,0x40,0x40,0x40,0x00,
/*--  文字:  六  --*/
0x20,0x20,0x20,0x20,0x20,0x20,0x21,0x22,0x2C,0x20,0x20,0x20,0x20,0x20,0x20,0x00,
0x00,0x40,0x20,0x10,0x0C,0x03,0x00,0x00,0x00,0x01,0x02,0x04,0x18,0x60,0x00,0x00,
/*--  文字:  七  --*/
0x80,0x80,0x80,0x80,0x80,0x40,0xFF,0x40,0x40,0x40,0x20,0x20,0x20,0x20,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x40,0x40,0x40,0x40,0x40,0x40,0x78,0x00,0x00
};               

uchar code numbers[]={	/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
/*--  文字:  0  --*/
0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00,
/*--  文字:  1  --*/
0x00,0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,
/*--  文字:  2  --*/
0x00,0x70,0x08,0x08,0x08,0x08,0xF0,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,
/*--  文字:  3  --*/
0x00,0x30,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x18,0x20,0x21,0x21,0x22,0x1C,0x00,
/*--  文字:  4  --*/
0x00,0x00,0x80,0x40,0x30,0xF8,0x00,0x00,0x00,0x06,0x05,0x24,0x24,0x3F,0x24,0x24,
/*--  文字:  5  --*/
0x00,0xF8,0x88,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x20,0x20,0x20,0x11,0x0E,0x00,
/*--  文字:  6  --*/
0x00,0xE0,0x10,0x88,0x88,0x90,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x20,0x1F,0x00,
/*--  文字:  7  --*/
0x00,0x18,0x08,0x08,0x88,0x68,0x18,0x00,0x00,0x00,0x00,0x3E,0x01,0x00,0x00,0x00,
/*--  文字:  8  --*/
0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,
/*--  文字:  9  --*/
0x00,0xF0,0x08,0x08,0x08,0x10,0xE0,0x00,0x00,0x01,0x12,0x22,0x22,0x11,0x0F,0x00,
/*--  符号:  “：”  --*/
0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,
/*--  文字:  A  --*/
0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00,0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20,
/*--  文字:  P  --*/
0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00,0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00,
/*--  文字:  M  --*/
0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00,0x20,0x3F,0x01,0x3E,0x01,0x3F,0x20,0x00
};

uchar code lunarDate[]={
/*--  文字:  初  --*/
0x08,0x08,0x89,0xEA,0x18,0x88,0x00,0x04,0x04,0xFC,0x04,0x04,0x04,0xFC,0x00,0x00,
0x02,0x01,0x00,0xFF,0x01,0x86,0x40,0x20,0x18,0x07,0x40,0x80,0x40,0x3F,0x00,0x00,
/*--  文字:  一  --*/
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/*--  文字:  二  --*/
0x00,0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00,0x00,
0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,
/*--  文字:  三  --*/
0x00,0x04,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x04,0x00,0x00,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00,
/*--  文字:  四  --*/
0x00,0xFC,0x04,0x04,0x04,0xFC,0x04,0x04,0x04,0xFC,0x04,0x04,0x04,0xFC,0x00,0x00,
0x00,0x7F,0x28,0x24,0x23,0x20,0x20,0x20,0x20,0x21,0x22,0x22,0x22,0x7F,0x00,0x00,
/*--  文字:  五  --*/
0x00,0x02,0x42,0x42,0x42,0xC2,0x7E,0x42,0x42,0x42,0x42,0xC2,0x02,0x02,0x00,0x00,
0x40,0x40,0x40,0x40,0x78,0x47,0x40,0x40,0x40,0x40,0x40,0x7F,0x40,0x40,0x40,0x00,
/*--  文字:  六  --*/
0x20,0x20,0x20,0x20,0x20,0x20,0x21,0x22,0x2C,0x20,0x20,0x20,0x20,0x20,0x20,0x00,
0x00,0x40,0x20,0x10,0x0C,0x03,0x00,0x00,0x00,0x01,0x02,0x04,0x18,0x60,0x00,0x00,
/*--  文字:  七  --*/
0x80,0x80,0x80,0x80,0x80,0x40,0xFF,0x40,0x40,0x40,0x20,0x20,0x20,0x20,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x40,0x40,0x40,0x40,0x40,0x40,0x78,0x00,0x00,
/*--  文字:  八  --*/
0x00,0x00,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0x7E,0x80,0x00,0x00,0x00,0x00,0x00,
0x00,0x80,0x60,0x18,0x07,0x00,0x00,0x00,0x00,0x00,0x03,0x0C,0x30,0x40,0x80,0x00,
/*--  文字:  九  --*/
0x00,0x10,0x10,0x10,0x10,0xFF,0x10,0x10,0x10,0x10,0xF0,0x00,0x00,0x00,0x00,0x00,
0x80,0x40,0x20,0x18,0x07,0x00,0x00,0x00,0x00,0x00,0x3F,0x40,0x40,0x40,0x78,0x00,
/*--  文字:  十  --*/
0x40,0x40,0x40,0x40,0x40,0x40,0x40,0xFF,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/*--  文字:  廿  --*/
0x20,0x20,0x20,0x20,0xFF,0x20,0x20,0x20,0x20,0x20,0x20,0xFF,0x20,0x20,0x20,0x00,
0x00,0x00,0x00,0x00,0xFF,0x40,0x40,0x40,0x40,0x40,0x40,0xFF,0x00,0x00,0x00,0x00,
/*--  文字:  腊  --*/
0x00,0xFE,0x22,0x22,0xFE,0x40,0x48,0x48,0x7F,0x48,0x48,0x48,0x7F,0x48,0x48,0x00,
0x80,0x7F,0x02,0x82,0xFF,0x00,0x00,0xFF,0x49,0x49,0x49,0x49,0x49,0xFF,0x00,0x00,
/*--  文字:  正  --*/
0x00,0x02,0x02,0xC2,0x02,0x02,0x02,0xFE,0x82,0x82,0x82,0x82,0x82,0x02,0x00,0x00,
0x40,0x40,0x40,0x7F,0x40,0x40,0x40,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00
};

uchar code theTerms[]={
/*--  文字:  小  --*/
0x00,0x00,0x00,0xE0,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x20,0x40,0x80,0x00,0x00,
0x08,0x04,0x03,0x00,0x00,0x40,0x80,0x7F,0x00,0x00,0x00,0x00,0x00,0x01,0x0E,0x00,
/*--  文字:  寒  --*/
0x10,0x0C,0x44,0x54,0x54,0xFC,0x55,0x56,0x54,0xFC,0x54,0x54,0x44,0x14,0x0C,0x00,
0x11,0x11,0x09,0x05,0x03,0x21,0x25,0x45,0x49,0x91,0x03,0x05,0x09,0x11,0x11,0x00,
/*--  文字:  大  --*/
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0xFF,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00,
0x80,0x80,0x40,0x20,0x10,0x0C,0x03,0x00,0x03,0x0C,0x10,0x20,0x40,0x80,0x80,0x00,
/*--  文字:  寒  --*/
0x10,0x0C,0x44,0x54,0x54,0xFC,0x55,0x56,0x54,0xFC,0x54,0x54,0x44,0x14,0x0C,0x00,
0x11,0x11,0x09,0x05,0x03,0x21,0x25,0x45,0x49,0x91,0x03,0x05,0x09,0x11,0x11,0x00,
/*--  文字:  立  --*/
0x00,0x10,0x10,0x90,0x10,0x10,0x11,0x16,0x10,0x10,0x10,0xD0,0x10,0x10,0x00,0x00,
0x40,0x40,0x40,0x40,0x43,0x5C,0x40,0x40,0x50,0x4C,0x43,0x40,0x40,0x40,0x40,0x00,
/*--  文字:  春  --*/
0x40,0x44,0x54,0x54,0xD4,0x54,0x74,0x5F,0x54,0x54,0xD4,0x54,0x54,0x44,0x40,0x00,
0x04,0x04,0x02,0x01,0xFE,0x92,0x92,0x92,0x92,0x92,0xFE,0x01,0x02,0x04,0x04,0x00,
/*--  文字:  雨  --*/
0x02,0xE2,0x22,0x22,0x22,0x22,0x22,0xFE,0x22,0x22,0x22,0x22,0x22,0xE2,0x02,0x00,
0x00,0xFF,0x00,0x00,0x09,0x12,0x00,0x7F,0x00,0x09,0x12,0x40,0x80,0x7F,0x00,0x00,
/*--  文字:  水  --*/
0x00,0x20,0x20,0x20,0xA0,0x60,0x00,0xFF,0x60,0x80,0x40,0x20,0x18,0x00,0x00,0x00,
0x20,0x10,0x08,0x06,0x01,0x40,0x80,0x7F,0x00,0x01,0x02,0x04,0x08,0x10,0x10,0x00,
/*--  文字:  惊  --*/
0x00,0xE0,0x00,0xFF,0x10,0x20,0x04,0xE4,0x24,0x25,0x26,0x24,0x24,0xE4,0x04,0x00,
0x01,0x00,0x00,0xFF,0x00,0x20,0x10,0x0D,0x41,0x81,0x7F,0x01,0x05,0x09,0x30,0x00,
/*--  文字:  蛰  --*/
0x24,0xA4,0x24,0xFF,0x14,0x00,0x84,0x54,0x3F,0x44,0x04,0x7C,0x80,0xE0,0x00,0x00,
0x00,0x40,0xC1,0x5E,0x52,0x53,0x52,0x7F,0x52,0x52,0x52,0x5E,0x60,0xC1,0x00,0x00,
/*--  文字:  春  --*/
0x40,0x44,0x54,0x54,0xD4,0x54,0x74,0x5F,0x54,0x54,0xD4,0x54,0x54,0x44,0x40,0x00,
0x04,0x04,0x02,0x01,0xFE,0x92,0x92,0x92,0x92,0x92,0xFE,0x01,0x02,0x04,0x04,0x00,
/*--  文字:  分  --*/
0x80,0x40,0x20,0x90,0x88,0x86,0x80,0x80,0x80,0x83,0x8C,0x10,0x20,0x40,0x80,0x00,
0x00,0x80,0x40,0x20,0x18,0x07,0x00,0x40,0x80,0x40,0x3F,0x00,0x00,0x00,0x00,0x00,
/*--  文字:  清  --*/
0x10,0x60,0x02,0x8C,0x00,0x44,0x54,0x54,0x54,0x7F,0x54,0x54,0x54,0x44,0x40,0x00,
0x04,0x04,0x7E,0x01,0x00,0x00,0xFF,0x15,0x15,0x15,0x55,0x95,0x7F,0x00,0x00,0x00,
/*--  文字:  明  --*/
0x00,0xFC,0x44,0x44,0x44,0xFC,0x00,0x00,0xFE,0x22,0x22,0x22,0x22,0xFE,0x00,0x00,
0x00,0x0F,0x04,0x04,0x04,0x8F,0x40,0x30,0x0F,0x02,0x02,0x42,0x82,0x7F,0x00,0x00,
/*--  文字:  谷  --*/
0x00,0x10,0x88,0x84,0x43,0x20,0x10,0x0C,0x10,0x20,0x41,0x82,0x84,0x08,0x00,0x00,
0x01,0x01,0x00,0xFE,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0xFE,0x00,0x01,0x01,0x00,
/*--  文字:  雨  --*/
0x02,0xE2,0x22,0x22,0x22,0x22,0x22,0xFE,0x22,0x22,0x22,0x22,0x22,0xE2,0x02,0x00,
0x00,0xFF,0x00,0x00,0x09,0x12,0x00,0x7F,0x00,0x09,0x12,0x40,0x80,0x7F,0x00,0x00,
/*--  文字:  立  --*/
0x00,0x10,0x10,0x90,0x10,0x10,0x11,0x16,0x10,0x10,0x10,0xD0,0x10,0x10,0x00,0x00,
0x40,0x40,0x40,0x40,0x43,0x5C,0x40,0x40,0x50,0x4C,0x43,0x40,0x40,0x40,0x40,0x00,
/*--  文字:  夏  --*/
0x00,0x01,0x01,0xFD,0x55,0x55,0x57,0x55,0x55,0x55,0x55,0xFD,0x01,0x01,0x00,0x00,
0x80,0x90,0x88,0x45,0x4F,0x55,0x25,0x25,0x25,0x55,0x4D,0x45,0x80,0x80,0x80,0x00,
/*--  文字:  小  --*/
0x00,0x00,0x00,0xE0,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x20,0x40,0x80,0x00,0x00,
0x08,0x04,0x03,0x00,0x00,0x40,0x80,0x7F,0x00,0x00,0x00,0x00,0x00,0x01,0x0E,0x00,
/*--  文字:  满  --*/
0x10,0x60,0x02,0x8C,0x00,0x24,0x24,0x2F,0xE4,0x24,0x24,0xE4,0x2F,0x24,0x24,0x00,
0x04,0x04,0x7E,0x01,0x00,0xFF,0x11,0x09,0x07,0x19,0x09,0x07,0x49,0x91,0x7F,0x00,
/*--  文字:  芒  --*/
0x84,0x84,0x84,0x84,0x9F,0x84,0xA4,0xC4,0x84,0x84,0x9F,0x84,0x84,0x84,0x84,0x00,
0x00,0x00,0x00,0x00,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x00,
/*--  文字:  种  --*/
0x24,0x24,0xA4,0xFE,0x23,0x22,0x00,0xF0,0x10,0x10,0xFF,0x10,0x10,0xF0,0x00,0x00,
0x08,0x06,0x01,0xFF,0x01,0x06,0x00,0x07,0x02,0x02,0xFF,0x02,0x02,0x07,0x00,0x00,
/*--  文字:  夏  --*/
0x00,0x01,0x01,0xFD,0x55,0x55,0x57,0x55,0x55,0x55,0x55,0xFD,0x01,0x01,0x00,0x00,
0x80,0x90,0x88,0x45,0x4F,0x55,0x25,0x25,0x25,0x55,0x4D,0x45,0x80,0x80,0x80,0x00,
/*--  文字:  至  --*/
0x00,0x02,0x42,0x62,0x52,0x4A,0x46,0x42,0x42,0x42,0x52,0x62,0xC2,0x02,0x00,0x00,
0x40,0x40,0x44,0x44,0x44,0x44,0x44,0x7F,0x44,0x44,0x44,0x44,0x44,0x40,0x40,0x00,
/*--  文字:  小  --*/
0x00,0x00,0x00,0xE0,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x20,0x40,0x80,0x00,0x00,
0x08,0x04,0x03,0x00,0x00,0x40,0x80,0x7F,0x00,0x00,0x00,0x00,0x00,0x01,0x0E,0x00,
/*--  文字:  暑  --*/
0x00,0x00,0x00,0x5F,0x55,0x55,0xF5,0x55,0x55,0x55,0x95,0x5F,0x00,0x00,0x00,0x00,
0x11,0x11,0x09,0x09,0xFD,0x55,0x57,0x57,0x55,0x55,0x55,0xFD,0x01,0x01,0x01,0x00,
/*--  文字:  大  --*/
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0xFF,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00,
0x80,0x80,0x40,0x20,0x10,0x0C,0x03,0x00,0x03,0x0C,0x10,0x20,0x40,0x80,0x80,0x00,
/*--  文字:  暑  --*/
0x00,0x00,0x00,0x5F,0x55,0x55,0xF5,0x55,0x55,0x55,0x95,0x5F,0x00,0x00,0x00,0x00,
0x11,0x11,0x09,0x09,0xFD,0x55,0x57,0x57,0x55,0x55,0x55,0xFD,0x01,0x01,0x01,0x00,
/*--  文字:  立  --*/
0x00,0x10,0x10,0x90,0x10,0x10,0x11,0x16,0x10,0x10,0x10,0xD0,0x10,0x10,0x00,0x00,
0x40,0x40,0x40,0x40,0x43,0x5C,0x40,0x40,0x50,0x4C,0x43,0x40,0x40,0x40,0x40,0x00,
/*--  文字:  秋  --*/
0x24,0x24,0xA4,0xFE,0x23,0x22,0x80,0x70,0x00,0xFF,0x00,0x80,0x40,0x30,0x00,0x00,
0x08,0x06,0x01,0xFF,0x01,0x86,0x40,0x30,0x0E,0x01,0x06,0x18,0x20,0x40,0x80,0x00,
/*--  文字:  处  --*/
0x00,0x80,0x60,0x9F,0x08,0x08,0xF8,0x00,0x00,0xFF,0x10,0x20,0x40,0x80,0x00,0x00,
0x81,0x40,0x20,0x13,0x0C,0x13,0x20,0x20,0x40,0x5F,0x40,0x40,0x40,0x41,0x40,0x00,
/*--  文字:  暑  --*/
0x00,0x00,0x00,0x5F,0x55,0x55,0xF5,0x55,0x55,0x55,0x95,0x5F,0x00,0x00,0x00,0x00,
0x11,0x11,0x09,0x09,0xFD,0x55,0x57,0x57,0x55,0x55,0x55,0xFD,0x01,0x01,0x01,0x00,
/*--  文字:  白  --*/
0x00,0x00,0xF8,0x08,0x08,0x0C,0x0A,0x09,0x08,0x08,0x08,0x08,0xF8,0x00,0x00,0x00,
0x00,0x00,0xFF,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0xFF,0x00,0x00,0x00,
/*--  文字:  露  --*/
0x10,0x0C,0x05,0x55,0x55,0x55,0x05,0x7F,0x05,0xD5,0x55,0x55,0x05,0x14,0x0C,0x00,
0x80,0xF7,0x85,0x7D,0x55,0x57,0x00,0x0A,0xE9,0xAB,0xA5,0xA5,0xEB,0x08,0x08,0x00,
/*--  文字:  秋  --*/
0x24,0x24,0xA4,0xFE,0x23,0x22,0x80,0x70,0x00,0xFF,0x00,0x80,0x40,0x30,0x00,0x00,
0x08,0x06,0x01,0xFF,0x01,0x86,0x40,0x30,0x0E,0x01,0x06,0x18,0x20,0x40,0x80,0x00,
/*--  文字:  分  --*/
0x80,0x40,0x20,0x90,0x88,0x86,0x80,0x80,0x80,0x83,0x8C,0x10,0x20,0x40,0x80,0x00,
0x00,0x80,0x40,0x20,0x18,0x07,0x00,0x40,0x80,0x40,0x3F,0x00,0x00,0x00,0x00,0x00,
/*--  文字:  寒  --*/
0x10,0x0C,0x44,0x54,0x54,0xFC,0x55,0x56,0x54,0xFC,0x54,0x54,0x44,0x14,0x0C,0x00,
0x11,0x11,0x09,0x05,0x03,0x21,0x25,0x45,0x49,0x91,0x03,0x05,0x09,0x11,0x11,0x00,
/*--  文字:  露  --*/
0x10,0x0C,0x05,0x55,0x55,0x55,0x05,0x7F,0x05,0xD5,0x55,0x55,0x05,0x14,0x0C,0x00,
0x80,0xF7,0x85,0x7D,0x55,0x57,0x00,0x0A,0xE9,0xAB,0xA5,0xA5,0xEB,0x08,0x08,0x00,
/*--  文字:  霜  --*/
0x10,0x0C,0x05,0x55,0x55,0x55,0x05,0x7F,0x05,0x55,0x55,0x55,0x05,0x14,0x0C,0x00,
0x20,0x12,0x0A,0x06,0xFF,0x0A,0x12,0x00,0xFF,0x55,0x55,0x55,0x55,0xFF,0x00,0x00,
/*--  文字:  降  --*/
0x00,0xFE,0x02,0x22,0xDA,0x86,0x90,0x48,0x57,0x24,0xA4,0x54,0x4C,0x84,0x80,0x00,
0x00,0xFF,0x08,0x10,0x08,0x07,0x18,0x12,0x12,0x12,0xFF,0x12,0x12,0x12,0x10,0x00,
/*--  文字:  立  --*/
0x00,0x10,0x10,0x90,0x10,0x10,0x11,0x16,0x10,0x10,0x10,0xD0,0x10,0x10,0x00,0x00,
0x40,0x40,0x40,0x40,0x43,0x5C,0x40,0x40,0x50,0x4C,0x43,0x40,0x40,0x40,0x40,0x00,
/*--  文字:  冬  --*/
0x00,0x20,0x10,0x08,0x94,0xA7,0x44,0x44,0x44,0xA4,0x94,0x0C,0x00,0x00,0x00,0x00,
0x02,0x02,0x01,0x01,0x20,0x24,0x24,0x44,0x48,0x88,0x10,0x01,0x01,0x02,0x02,0x00,
/*--  文字:  小  --*/
0x00,0x00,0x00,0xE0,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x20,0x40,0x80,0x00,0x00,
0x08,0x04,0x03,0x00,0x00,0x40,0x80,0x7F,0x00,0x00,0x00,0x00,0x00,0x01,0x0E,0x00,
/*--  文字:  雪  --*/
0x10,0x0C,0x05,0x55,0x55,0x55,0x05,0x7F,0x05,0x55,0x55,0x55,0x05,0x14,0x0C,0x00,
0x00,0x00,0x41,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0xFF,0x00,0x00,0x00,
/*--  文字:  大  --*/
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0xFF,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00,
0x80,0x80,0x40,0x20,0x10,0x0C,0x03,0x00,0x03,0x0C,0x10,0x20,0x40,0x80,0x80,0x00,
/*--  文字:  雪  --*/
0x10,0x0C,0x05,0x55,0x55,0x55,0x05,0x7F,0x05,0x55,0x55,0x55,0x05,0x14,0x0C,0x00,
0x00,0x00,0x41,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0xFF,0x00,0x00,0x00,
/*--  文字:  冬  --*/
0x00,0x20,0x10,0x08,0x94,0xA7,0x44,0x44,0x44,0xA4,0x94,0x0C,0x00,0x00,0x00,0x00,
0x02,0x02,0x01,0x01,0x20,0x24,0x24,0x44,0x48,0x88,0x10,0x01,0x01,0x02,0x02,0x00,
/*--  文字:  至  --*/
0x00,0x02,0x42,0x62,0x52,0x4A,0x46,0x42,0x42,0x42,0x52,0x62,0xC2,0x02,0x00,0x00,
0x40,0x40,0x44,0x44,0x44,0x44,0x44,0x7F,0x44,0x44,0x44,0x44,0x44,0x40,0x40,0x00,
};


/*******************************************************************
延时子程序
*******************************************************************/
void delay(uint xms){
uint i,j;
for(i=xms;i>0;i--)
	for(j=110;j>0;j--) ;
}

/*******************************************************************
LCD忙检查子程序
*******************************************************************/
void CheckState(){
uchar dat;
RS=0;
RW=1;
do{
DATA=0x00;
EN=1;
_nop_();
dat=DATA;
EN=0;
dat=0x80&dat;
}while(!(dat==0x00));
}

/*******************************************************************
函数名称：SendCommandToLCD()
函数功能：向LCD写命令
输入参数：com 输入的命令 
返回值：  无
*******************************************************************/
void SendCommandToLCD(uchar com){
CheckState();
RS=0;
RW=0;	           //MPU写指令到指令暂存器IR
DATA=com;		   //要写入的命令
EN=1;	           //E从高到低，进行写指令
_nop_();           //延迟指令，时间为1us
_nop_();
EN=0;
}

/*******************************************************************
设置页码
*******************************************************************/
void SetLine(uchar page){	 
page=0xb8|page;
SendCommandToLCD(page);
}

/*******************************************************************
设置起始行
*******************************************************************/
void SetStartLine(uchar startline){	
startline=0xc0|startline;
SendCommandToLCD(startline);
}

/*******************************************************************
设置起始列，列号为0到63
*******************************************************************/
void SetColumn(uchar column){	  
column=column&0x3f;
column=0x40|column;
SendCommandToLCD(column);
}


/*******************************************************************
开关显示屏，onoff只能为0或1
onoff为1则开，onoff为0则关
*******************************************************************/
void SetOnOff(uchar onoff){
	onoff=0x3e|onoff;
	SendCommandToLCD(onoff);
}

/*******************************************************************
写数据
*******************************************************************/
void WriteByte(uchar dat){	          
CheckState();
RS=1;
RW=0;
DATA=dat;	                          //dat应该就是要写的数据
EN=1;
_nop_();
_nop_();
EN=0;
}

/*******************************************************************
选屏，screen=0，1，2
*******************************************************************/
void SelectScreen(uchar screen){
switch(screen){
case 0:cs1=0;	//全屏显示
		_nop_();
		_nop_();
		_nop_();
		cs2=0;
		_nop_();
		_nop_();
		_nop_();
		break;
case 1:cs1=0;     //左屏显示
		_nop_();
		_nop_();
		_nop_();
		cs2=1;
		_nop_();
		_nop_();
		_nop_();
		break;
case 2:cs1=1;      //右屏显示
		_nop_();
		_nop_();
		_nop_();
		cs2=0;
		_nop_();
		_nop_();
		_nop_();
		break;
}
}


/*******************************************************************
清屏，screen=0，1，2
*******************************************************************/
void ClearScreen(uchar screen){
uchar i,j;
SelectScreen(screen);
for(i=0;i<8;i++)
{
	SetLine(i);
	SetColumn(0);
	for(j=0;j<64;j++)
	{
		WriteByte(0x00);        //写数据列地址将自动+1
	}
}
}

/*******************************************************************
初始化子程序
*******************************************************************/
void InitLCD(){
CheckState();
SelectScreen(0);
SetOnOff(0);	     //关屏
SelectScreen(0);
SetOnOff(1);         //开屏
SelectScreen(0);
ClearScreen(0);	     //清屏
SetStartLine(0);     //开始行为0
}

/*******************************************************************
函数名称：Display()
函数功能：lcd设定显示位置并显示字符串子程序，包括中英文、数字、特殊字符。
输入参数：ss:屏号  column：列号  number：字号   page：页号（可理解为要显示的汉字位于屏幕的page行）
返回值：  无
*******************************************************************/
void DisplayNumber(uchar ss, uchar page,uchar column,uchar number){
	int i;
	SelectScreen(ss);
	SetLine(page);	                        //显示第number个汉字的上半部分
	SetColumn(column);
	for(i=0;i<8;i++){	                    //i为一个字里面的各个列
		WriteByte(numbers[i+16*number]);
	}
	SetLine(page+1);                        //显示第number个汉字的下半部分
	SetColumn(column);
	for(i=0;i<8;i++){
		WriteByte(numbers[i+16*number+8]);
	}
}

void DisplayWord1(uchar ss, uchar page,uchar column,uchar number){
	int i;
	SelectScreen(ss);
	SetLine(page);	                        //显示第number个汉字的上半部分
	SetColumn(column);                        
	for(i=0;i<16;i++){	                    //i为一个字里面的各个列
		WriteByte(words[i+32*number]);
	}
	SetLine(page+1);                        //显示第number个汉字的下半部分
	SetColumn(column);
	for(i=0;i<16;i++){
		WriteByte(words[i+32*number+16]);
}
}


void DisplaySolarTerm(uchar ss, uchar page,uchar column,uchar number){
	int i;
	SelectScreen(ss);
	SetLine(page);	                        //显示第number个汉字的上半部分
	SetColumn(column);                        
	for(i=0;i<16;i++){	                    //i为一个字里面的各个列
		WriteByte(theTerms[i+32*number]);
	}
	SetLine(page+1);                        //显示第number个汉字的下半部分
	SetColumn(column);
	for(i=0;i<16;i++){
		WriteByte(theTerms[i+32*number+16]);
}
}
/*显示天干地支*/
/*
void Displaytiangan(uchar ss, uchar page,uchar column,uchar number){
	int i;
	SelectScreen(ss);
	SetLine(page);	                        //显示第number个汉字的上半部分
	SetColumn(column);                        
	for(i=0;i<16;i++){	                    //i为一个字里面的各个列
		WriteByte(tiangan[i+32*number]);
	}
	SetLine(page+1);                        //显示第number个汉字的下半部分
	SetColumn(column);
	for(i=0;i<16;i++){
		WriteByte(tiangan[i+32*number+16]);
}
}

void Displaydizhi(uchar ss, uchar page,uchar column,uchar number){
	int i;
	SelectScreen(ss);
	SetLine(page);	                        //显示第number个汉字的上半部分
	SetColumn(column);                        
	for(i=0;i<16;i++){	                    //i为一个字里面的各个列
		WriteByte(dizhi[i+32*number]);
	}
	SetLine(page+1);                        //显示第number个汉字的下半部分
	SetColumn(column);
	for(i=0;i<16;i++){
		WriteByte(dizhi[i+32*number+16]);
}
}
*/
void DisplaySunToLunar(uchar ss, uchar page,uchar column,uchar number){
	int i;
	SelectScreen(ss);
	SetLine(page);	                        //显示第number个汉字的上半部分
	SetColumn(column);                        
	for(i=0;i<16;i++){	                    //i为一个字里面的各个列
		WriteByte(lunarDate[i+32*number]);
	}
	SetLine(page+1);                        //显示第number个汉字的下半部分
	SetColumn(column);
	for(i=0;i<16;i++){
		WriteByte(lunarDate[i+32*number+16]);
}
}