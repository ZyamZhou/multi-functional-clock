#include<reg51.h>
#include<intrins.h>
#include <LCD.h>

#define ERROR 0
#define OK 1
#define uint unsigned int
#define uchar unsigned char 

sbit Bus=P1^3;//数据总线

uchar  idata value[5];    
uint idata check;			 //储存检测值 
uchar idata temprature,hum;
//***************************************
// 模块功能：读取DHT11的数据
// 该模块为温湿度传感器 当主机给启动信号后
// 该模块会连续发出5个字节的数据 共耗时约4MS
// 第一二个字节为湿度数据的整数与小数部分
// 第三四个字节为温度数据的整数与小数部分
// 第五个字节为校验码 它是前四个数据相加后的后8位的数值
// 程序带未传感器未连接检测 及读数据超时处理

//***************************************
 
void Delay10us(void);
void Delay20ms(void);
void Delay1s(void);
uchar Read_Temp();

uchar Read_Temp()	            
{
	uchar i,j=0,mask;
	Bus=0;                    //主机主动拉低并持续20ms（超过18ms）
	Delay20ms();
	Bus=1;                    //主机再主动拉高40us（20-40us）
	Delay10us();
	Delay10us();
	Delay10us();
	Delay10us();
	if(Bus==1)//如果总线没有响应
	{
		return ERROR;
	}
	else //如果总线有响应
	{
		while(!Bus);  //等待总线的低响应(80us)
		while(Bus);   //等待总线的高响应（80us）
		while(j<5)
		{
			mask=0x80;	  //1000 0000 从最高位开始
			for(i=0;i<8;i++)            //接收一个字节
			{
				while(!Bus);            //等待过一个低电平间隙
				Delay10us();            //0保持26-28us的高电平，1保持70us的高电平，取40us的处的电平监测
				Delay10us();
				Delay10us();
				Delay10us();
				if(Bus==0)	            //说明是数字0
				{
					value[j]&=(~mask);	//该位置0
				}
				else
				{
					value[j]|=mask;		//该位置1
				}
				mask>>=1;				//mask右移一位
				while(Bus);             //等待该位传送结束
			}
			j++;//继续接受下一个字节
		}
			
	}
	check=(value[0]+value[2])&0x00ff;  //计算校验和 小数位是0
	if(check==value[4])//如果校验和没错
	{
		return OK;
	}
	else
	{
		return ERROR;
	}
 
 
}


/*10us延时子函数*/
void Delay10us()
{
	uchar i;
	i=2;
	while(--i);
}
 
 
/*20ms延时子函数*/
void Delay20ms(void)   //误差 -0.000000000005us
{
    uchar a,b,c;
    for(c=1;c>0;c--)
        for(b=222;b>0;b--)
            for(a=40;a>0;a--);
}  
 
/*1s延时子函数*/
void Delay1s(void)   //误差 -0.000000000227us
{
    uchar a,b,c;
    for(c=13;c>0;c--)
        for(b=247;b>0;b--)
            for(a=142;a>0;a--);
    _nop_(); 
}


void DisplayTemp()
{
	DisplayNumber(2,6,1*16+2*8,temprature/10);
	DisplayNumber(2,6,1*16+3*8,temprature%10);
	DisplayNumber(2,6,1*16+4*8,hum/10);
	DisplayNumber(2,6,1*16+5*8,hum%10);

}

void DHT11_Init(){
	Delay1s();//越过1s不稳定期	
}
void DHT11(){
	uchar result;
	result=Read_Temp();
	if(result==OK)
	{
		temprature=value[2];   //温度
		hum=value[0];    //湿度
	}
}