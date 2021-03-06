/*变量定义*/
#include <reg52.h>
#include <intrins.h>

#define uint unsigned int
#define uchar unsigned char 
#define LCD12864_DATAPORT P0 

sbit LCD12864_RS=P2^2;  //选择命令
sbit LCD12864_RW=P2^1;  //选择读取
sbit LCD12864_EN=P2^0;  //使能
sbit LCD12864_PSB=P1^7; //串并行口选择

/*****************************延时1ms函数*************************************/
void LCD12864_Delay1ms(uint n){
    unsigned int i=0,j=0;
    for(i=0;i<n;i++)
        for(j=0;j<123;j++);	
}

/*******************************************************************************
* 函 数 名         : LCD12864_Busy
* 函数功能       : 检测LCD是否忙
* 输    入         : 无
* 输    出         : 1或0（1表示不忙，0表示忙）
*******************************************************************************/
uchar LCD12864_Busy(void)
{
  uchar i = 0;
  LCD12864_RS = 0;     //选择命令
  LCD12864_RW = 1;  //选择读取
  LCD12864_EN = 1;
  LCD12864_Delay1ms(1);
  while((LCD12864_DATAPORT & 0x80) == 0x80)  //检测读取到的值
  {
    i++;
    if(i > 100)
    {
      LCD12864_EN = 0;
      return 0;     //超过等待时间返回0表示失败
    }
  }
  LCD12864_EN = 0;
  return 1;
}
/*******************************************************************************
* 函 数 名         : LCD12864_WriteCmd
* 函数功能       : 写命令
* 输    入         : cmd
* 输    出         : 无
*******************************************************************************/
void LCD12864_WriteCmd(uchar cmd)
{
  uchar i;
  i = 0;
  while( LCD12864_Busy() == 0)
  {
    LCD12864_Delay1ms(1);
    i++;
    if( i>100)
    {
      return;     //超过等待退出
    }  
  }  
  LCD12864_RS = 0;     //选择命令
  LCD12864_RW = 0;     //选择写入
  LCD12864_EN = 0;     //初始化使能端
  LCD12864_DATAPORT = cmd;   //放置数据
  LCD12864_EN = 1;       //写时序
  LCD12864_Delay1ms(5);
  LCD12864_EN = 0;              
}

/*******************************************************************************
* 函 数 名         : LCD12864_WriteData
* 函数功能       : 写数据
* 输    入         : dat
* 输    出         : 无
*******************************************************************************/
void LCD12864_WriteData(uchar dat)
{
  uchar i;
  i = 0;
  while( LCD12864_Busy() == 0)
  {
    LCD12864_Delay1ms(1);
    i++;
    if( i>100)
    {
      return;     //超过等待退出
    }  
  }
  LCD12864_RS = 1;     //选择数据
  LCD12864_RW = 0;     //选择写入
  LCD12864_EN = 0;     //初始化使能端
  LCD12864_DATAPORT = dat;   //放置数据
  LCD12864_EN = 1;       //写时序
  LCD12864_Delay1ms(5);
  LCD12864_EN = 0;                    
}

/*******************************************************************************
* 函 数 名         : LCD12864_Init
* 函数功能       : 初始化LCD12864
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void LCD12864_Init()
{
	LCD12864_PSB = 1;           //选择并行输入
	LCD12864_WriteCmd(0x30);    //选择基本指令操作
	LCD12864_WriteCmd(0x06);  	//地址自动加1
	LCD12864_WriteCmd(0x0c);    //显示开，关光标
	LCD12864_WriteCmd(0x01);    //清除LCD12864的显示内容
}

/*******************************************************************************
* 函 数 名     :  LCDSetPos
* 函数功能     : 设置在基本指令模式下设置显示坐标。注意：x是设置行，y是设置列
* 输    入      : x(行), y（列）
* 输    出      : 无
*******************************************************************************/
void LCDSetPos(uchar x, uchar y)  
{
  uchar pos;
  uchar i=y;
  if(x == 0)        //第一行的地址是80H
    x = 0x80;   
  else if(x == 1)   //第二行的地址是90H
    x = 0x90;  
  else if(x == 2)   //第三行的地址是88H
    x = 0x88; 
  else if(x == 3)   //第四行的地址是98H
    x = 0x98;
  
  pos = x + y;
  LCD12864_WriteCmd(pos);
}

void DisplayWord(uchar *word){
	while(*word !='\0'){
	LCD12864_WriteData(*word);
	word++;		
	}		
}