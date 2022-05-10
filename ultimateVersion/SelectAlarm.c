#include "reg52.h"
#include "intrins.h"
#include "string.h"
#include "LCDNew.h"
#include "map.h"
#include "pressKey.h"

#define uchar unsigned char 
#define uint unsigned int

uchar idata flagSetATime=0;
uchar idata flagSetAMusic=0;
uchar idata flagTIME=0;
uchar idata flagMUSIC=0;
uchar idata NumMusic=0;
uchar idata flagCheckAlarm=0;
uchar idata flagSetAlarmTriggered=0;            //一旦闹钟中断开启过一次以后永远是1
uchar idata AlarmOnOff=0;						  //闹钟关闭是0 其它情况是1	    初始化为闹钟关闭
uchar idata flagState=0;              
/****************************选择闹钟设置**********************************/
void DisplayChoice(){
	LCDSetPos(0,0);
	DisplayWord("闹钟状态：");		     //按下SW6键可以设置闹钟的关闭和开启   SW6就是SET_AM_PM
	if(AlarmOnOff==0){
		LCDSetPos(0,5);
		DisplayWord("关闭");	
	}
	if(AlarmOnOff==1){
		LCDSetPos(0,5);
		DisplayWord("开启");
	}
	LCDSetPos(1,0);
	DisplayWord("设置闹钟时间按+");
	LCDSetPos(2,0);
	DisplayWord("设置闹钟音乐按-");
	LCDSetPos(3,0);
	DisplayWord("查看闹钟按SW1");        //to就是SW1 就是K1
}


//K2是+键 K5是-键
void scanChoiceButton(){
	uchar i=0;
	if(SET_AM_PM==0){                   
		Delay10ms();
		if(SET_AM_PM==0){	                    
			AlarmOnOff=1-AlarmOnOff;    //闹钟开启或者关闭
		}
		while((i<50)&&(SET_AM_PM==0)){         
			Delay10ms();
			i++;}
		i=0;			
	}
	if(K2==0){			                 //进入设置闹钟时间
		Delay10ms();
		if(K2==0){	                    
			flagSetATime=1;
			flagTIME=1;	
		}
		while((i<50)&&(K2==0)){         
			Delay10ms();
			i++;}
		i=0;
	}
	if(K5==0){                         //进入设置闹钟音乐
		Delay10ms();
		if(K5==0){
			flagSetAMusic=1;
			flagMUSIC=1;	
		}
		while((i<50)&&(K5==0)){        
			Delay10ms();
			i++;}
		i=0;	
	}
	if(K1==0){			                //进入查看闹钟状态
		Delay10ms();
		if(K1==0){
			flagCheckAlarm=1;
			flagState=1;
		}
		while((i<50)&&(K1==0)){         //检测按键是否松开
			Delay10ms();
			i++;}
		i=0;	
	}	
}

void DisplaySetMusic(){
	LCD12864_WriteCmd(0x01);    //清除LCD12864的显示内容
	LCDSetPos(0,1);
	DisplayWord("设置闹钟音乐");
	LCDSetPos(1,1);
	DisplayWord("音乐序号");
	LCD12864_WriteData(':');
	LCD12864_WriteData(StrNUMBER[NumMusic]);
}

//K2是+键 K5是-键
void scanButtonSetMusic(){
	uchar i=0;
	if(K2==0){
		Delay10ms();
		if(K2==0){
			if(NumMusic>=1)	NumMusic=0;
			else NumMusic++;
			LCDSetPos(1,5);
			LCD12864_WriteData(':');
			LCD12864_WriteData(StrNUMBER[NumMusic]);		
		}
		while((i<50)&&(K2==0)){             //检测按键是否松开
			Delay10ms();
			i++;}
		i=0;	
	}
	if(K5==0){
		Delay10ms();
		if(K5==0){
			if(NumMusic<=0)	NumMusic=1;
			else NumMusic--;
			LCDSetPos(1,5);
			LCD12864_WriteData(':');
			LCD12864_WriteData(StrNUMBER[NumMusic]);		
		}
		while((i<50)&&(K5==0)){            //检测按键是否松开
			Delay10ms();
			i++;}
		i=0;	
	}		
}