#include "reg52.h"
#include "intrins.h"
#include "ds1302.h"
#include "LCD.h"
#include "pressKey.h"
#include "lunarCalender.h"
#include "DHT11.h"

#define uint unsigned int
#define uchar unsigned char 


sbit K3=P3^2;    		 //触发设置时间功能(定义在main函数里面)	 SET功能
sbit K4=P3^3;            //触发设置闹钟	  alarm功能
sbit LED1=P2^5;          //闹钟灯 高电平驱动

uchar i;
void LCDdisplaywhole();
void AlarmOn();

void main(){
	IntConfiguration();
	InitLCD();
	Ds1302Init();
	ClearScreen(0);
	AM_PM=(bit)(TIME[2]&0x20>>5);
	DHT11_Init();                                  //越过DHT11 1s不稳定期		     
	while(1){
		/*设置时间*/
		AlarmOn();
		if(SetState==0 && SetAlarm==0){			   //设置时间的中断和设置闹钟的中断都没有触发
			Ds1302ReadTime();
			convert(year,month,date);
			DHT11();
			LCDdisplaywhole();
	
		}		
		else if(SetState!=0 && SetAlarm==0){	  //触发设置时间的中断
			scanButton();                       
		}
		else{
			scanButton_alarm();
		}								
	}
}

void LCDdisplaywhole(){
	ClearScreen(0);
	for(i=0;i<128;i++){
		Ds1302ReadTime();
		SetStartLine(0);
	if(SetState==0 && SetAlarm==0){
		/*显示年*/
		DisplayNumber(1,0,0*16,2);
		DisplayNumber(1,0,1*8,0);
		DisplayNumber(1,0,2*8,year/16);
		DisplayNumber(1,0,3*8,year%16);
		DisplayWord1(1,0,4*8,0);	                    //屏号, 页号,列号,字号
		/*显示月*/
		DisplayNumber(1,0,4*8+1*16,month/16);
		DisplayNumber(1,0,5*8+1*16,month%16);
		DisplayWord1(2,0,0*16,1);
		/*显示日*/
		DisplayNumber(2,0,1*16,date/16);
		DisplayNumber(2,0,1*8+1*16,date%16);
		DisplayWord1(2,0,2*8+1*16,2);
		/*换行*/
		/*显示时间*/
		DisplayNumber(1,2,0*8,hour/16);
		DisplayNumber(1,2,1*8,hour%16);
		DisplayNumber(1,2,2*8,10);
		DisplayNumber(1,2,3*8,minute/16);
		DisplayNumber(1,2,4*8,minute%16);
		DisplayNumber(1,2,5*8,10);
		DisplayNumber(1,2,6*8,second/16);
		DisplayNumber(1,2,7*8,second%16);
		/*显示AM PM*/
		if(AM_PM==0){
			DisplayNumber(2,2,0*8,11);
		}
		else{
			DisplayNumber(2,2,0*8,12);
		}
		DisplayNumber(2,2,1*8,13);
		DisplayWord1(2,2,2*8,6);
		DisplayWord1(2,2,2*8+1*16,7);
		DisplayWord1(2,2,2*8+2*16,day+7);
		/*显示24节气*/
		if((convertBCDtoHex(month)==(code3/2+1))&&(convertBCDtoHex(date)==SolarTerm[code3])){
			DisplaySolarTerm(1,4,0*16,code3*2);
			DisplaySolarTerm(1,4,1*16,code3*2+1);
		}
		/*显示天干地支纪年法*/
		//Displaytiangan(1,4,2*16,code1);
		//Displaydizhi(1,4,3*16,code2);
			
		/*显示阴历日期*/
		if(lunar_month==1){
			DisplaySunToLunar(1,6,1*16,13);	
		}
		else if(lunar_month/10==0 && lunar_month%10!=0){
			DisplaySunToLunar(1,6,1*16,lunar_month%10);	
		}
		else if(lunar_month/10==1 && lunar_month%10==0){
			DisplaySunToLunar(1,6,1*16,10);		              //十
		}
		else if(lunar_month%10==1 && lunar_month%10!=0){
			DisplaySunToLunar(1,6,0*16,lunar_month/10);
			DisplaySunToLunar(1,6,1*16,lunar_month%10);
		}
		else{
			DisplaySunToLunar(1,6,1*16,12);	                  //腊
		}

		DisplayWord1(1,6,2*16,1);                             //月
				                     
		if((lunar_date/10==0) || (lunar_date==10)){
			DisplaySunToLunar(1,6,3*16,0);	                  //初
		}
		else if(lunar_date/10==1){
			DisplaySunToLunar(1,6,3*16,10);		              //十		  初十是一种特殊情况
		}
		else if(lunar_date/10==2){
			DisplaySunToLunar(1,6,3*16,11);		              //廿
		}
		else{
			DisplaySunToLunar(1,6,3*16,3);	                  //三        
		}
		if(lunar_date%10!=0){
			DisplaySunToLunar(2,6,0,lunar_date%10);
		}
		else{
			DisplaySunToLunar(2,6,0,10);	
		}
		DisplayTemp();
	}		
	}
	
}
void AlarmOn(){                       //闹钟响起函数
	if((second==alarm[0]) && (minute=alarm[1]) && (hour==alarm[2]) && (AM_PM==alarm_AM_PM)){
		LED1=1;                          //开启闹钟灯
		//到时候再加音乐
		}
	else{
		LED1=0;
	}
}

void Int0() interrupt 0{
	Delay10ms();
	if(K3==0){
		SetState=~SetState;
		SetPlace=0;
		Ds1302Init(); 
	}
}

void Int2() interrupt 2{
	Delay10ms();
	if(K4==0){
		if(SetAlarm!=0){	                         //每当闹钟设置按键被触发的时候设置一次初始化
			alarm[2]=TIME[2] & 0x1f; //时
			alarm[1]=TIME[1];		 //分
			alarm[0]=TIME[0];		 //秒
		}
		SetAlarm=~SetAlarm;
		SetPlace=0;
	}
}