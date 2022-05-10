#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char 
#define uint unsigned int

uchar year;
uchar hour;
uchar day;
uchar month;
uchar date;
uchar minute;
uchar second;
uchar AM_PM;
//DS1302写入地址命令,顺序为秒分时日月周年，最低位读写位，读取的地址在写入的基础上加1
uchar code WRITE_RTC_ADDR[7] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};
uchar year0=0x20;//年份头两位
//DS1302时钟初始化2020年8月4日星期二12点00分00秒。存储顺序是秒分时日月周年,存储格式是用BCD码
uchar TIME[7] = {0x30, 0x59, 0x04, 0x21, 0x06, 0x02, 0x21};

sbit RST=P2^7;
sbit SCLK=P2^5;
sbit DSIO=P2^6;
/*******************************************************************
函数名称：Ds1302Write()
函数功能：向DS1302命令（地址+数据）。
输入参数：addr：写入数据的地址  dat：写入的数据  
返回值：  无
*******************************************************************/
void Ds1302Write(uchar addr, uchar dat)
{
	uchar n;
	RST = 0;
	_nop_();
	SCLK = 0;		//先将SCLK置低电平。
	_nop_();
	RST = 1; 		//然后将RST(CE)置高电平。
	_nop_();
	for (n=0; n<8; n++)		//开始传送八位地址命令
	{
		DSIO = addr & 0x01;		//数据从低位开始传送
		addr >>= 1;
		SCLK = 1;		//数据在上升沿时，DS1302读取数据
		_nop_();
		SCLK = 0;
		_nop_();
	}
	for (n=0; n<8; n++)		//写入8位数据
	{
		DSIO = dat & 0x01;
		dat >>= 1;
		SCLK = 1;		//数据在上升沿时，DS1302读取数据  q
		_nop_();
		SCLK = 0;
		_nop_();	
	}			 
	RST = 0;		//传送数据结束
	_nop_();
} 

/*******************************************************************
函数名称：Ds1302Read()
函数功能：读取一个地址的数据。
输入参数：addr：需要读取数据的地址   
返回值：  dat：读出的数据 
*******************************************************************/
uchar Ds1302Read(uchar addr)
{
	uchar n,dat,dat1;
	RST = 0;
	_nop_();

	SCLK = 0;		//先将SCLK置低电平。
	_nop_();

	RST = 1;		//然后将RST(CE)置高电平。
	_nop_();
	for(n=0; n<8; n++)		//开始传送八位地址命令
	{
		DSIO = addr & 0x01;		//数据从低位开始传送
		addr >>= 1;
		SCLK = 1;		//数据在上升沿时，DS1302读取数据
		_nop_();
		SCLK = 0;		//DS1302下降沿时，放置数据
		_nop_();
	}
	_nop_();
	for(n=0; n<8; n++)		//读取8位数据
	{
		dat1 = DSIO;		//从最低位开始接收
		dat = (dat>>1) | (dat1<<7);
		SCLK = 1;
		_nop_();
		SCLK = 0;		//DS1302下降沿时，放置数据
		_nop_();
	}
	RST = 0;
	_nop_();		//DS1302复位的稳定时间。
	SCLK = 1;
	_nop_();
	DSIO = 0;
	_nop_();
	DSIO = 1;
	_nop_();
	return dat;	
}

/*******************************************************************
函数名称：Ds1302Init()
函数功能：初始化DS1302。
输入参数：无   
返回值：  无 
*******************************************************************/
void Ds1302Init()
{
	uchar n;
	Ds1302Write(0x8E,0X00);		 //关闭写保护功能
	for (n=0; n<7; n++)		     //写入7个字节的时钟信号：分秒时日月周年
	{
		Ds1302Write(WRITE_RTC_ADDR[n],TIME[n]); 	
	}
	Ds1302Write(WRITE_RTC_ADDR[2],TIME[2]|0x20);           //设置AM和PM
	Ds1302Write(0x8E,0x80);		 //打开写保护功能
}

/*******************************************************************
函数名称：Ds1302ReadTime()
函数功能：读取时钟的日期、时间数据。
输入参数：无   
返回值：  无 
*******************************************************************/
void Ds1302ReadTime()
{
	TIME[6]=Ds1302Read(WRITE_RTC_ADDR[6]|0x01);
	TIME[5]=Ds1302Read(WRITE_RTC_ADDR[5]|0x01);
	TIME[5]=TIME[5]&0x07;
	TIME[4]=Ds1302Read(WRITE_RTC_ADDR[4]|0x01);
	TIME[3]=Ds1302Read(WRITE_RTC_ADDR[3]|0x01);
	TIME[2]=Ds1302Read(WRITE_RTC_ADDR[2]|0x01);
	TIME[2] =TIME[2] & 0x1f;      
	TIME[1]=Ds1302Read(WRITE_RTC_ADDR[1]|0x01);
	TIME[0]=Ds1302Read(WRITE_RTC_ADDR[0]|0x01);
	//更新TIME[]数组里面的初始化数据
	second=TIME[0];
	minute=TIME[1];
	hour=TIME[2];
	date=TIME[3];
	month=TIME[4];
	day=TIME[5];
	year=TIME[6];

}

