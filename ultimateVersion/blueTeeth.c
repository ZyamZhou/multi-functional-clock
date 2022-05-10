#include "reg52.h"
#include "intrins.h"

#define uchar unsigned char 
#define uint unsigned int

/*串口初始化配置*/

void Uart0_Init(){
    RCAP2L=0xD9;//9600波特率对应 FFD9，低位为D9
    RCAP2H=0xFF;//高位为FF
    T2CON=0x34;//RCLK、TCLK、TR2置1
    SCON=0x50;//串口工作模式1，接收使能
    ES=1;//打开接收中断
    EA=1;//打开总中断

}
/*发送数据*/
void sendDate(char date)
{
    SBUF=date;		  //接收到的数据放入发送缓存器发送
    while(!TI);       //等待发送数据完成
    TI=0;			  //清除发送完成标志位
}
 