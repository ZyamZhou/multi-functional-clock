#include "reg52.h"
#include "intrins.h"
#include "pressKey.h"
#include "LCDNew.h"
#include "ds1302.h"
#include "SelectAlarm.h"
#include "map.h"

#define uchar unsigned char 
#define uint unsigned int

uchar idata flag11=0;

/*****************************用蓝牙设置闹钟的函数***********************************/
/*
void BtSetAlarm(){		
	if(flag11==5){
		SetPlace++;
		if(SetPlace>=3)
			SetPlace=0;
		flag11=0xff;	 //置状态变量到空状态
	}

	if(flag11==7){
		alarm[SetPlace]++;
		DisplayAlarmButton(SetPlace);
		if((alarm[SetPlace]&0x0f)>9){                       //保证BCD码
			alarm[SetPlace]=alarm[SetPlace]+6;
		}
		if((alarm[SetPlace]>=0x60)&&(SetPlace<2)){          //分秒只能到59
			alarm[SetPlace]=0;
		}
		if((alarm[SetPlace]>=0x12)&&(SetPlace==2)){         //小时只能到23
			alarm[SetPlace]=0;
		}
		flag11=0xff;	//置状态变量到空状态
	}

	if(flag11==8){
		if(alarm[SetPlace]&0x0f<=0){
			alarm[SetPlace]=alarm[SetPlace]-6;			    //保证BCD码减法
		}
		if((alarm[SetPlace]==0x00)&&(SetPlace<2)){          //分秒只能到59
			alarm[SetPlace]=0x60;
		}
		if((alarm[SetPlace]==0x00)&&(SetPlace==2)){         //小时只能到12
			alarm[SetPlace]=0x12;
		}
		alarm[SetPlace]--;
		DisplayAlarmButton(SetPlace);
		flag11=0xff;	                                    //置状态变量到空状态	
	}
/*

			/*设置AM_PM功能*/	
/*	
	if(flag11==9){                       //检测按键K2是否按下
		alarm_AM_PM=1-alarm_AM_PM;
		if(alarm_AM_PM==0){
			LCDSetPos(2,3);
			LCD12864_WriteData(' ');
			LCD12864_WriteData('A');
			LCD12864_WriteData('M');				
		}
		if(alarm_AM_PM==1){
			LCDSetPos(2,3);
			LCD12864_WriteData(' ');
			LCD12864_WriteData('P');
			LCD12864_WriteData('M');
		}
		flag11=0xff;	                                    //置状态变量到空状态
	}
}


*/
/******************************设置闹钟音乐函数***********************************/
void BtSetAlarmMusic(){
	if(flag11==7){
		if(NumMusic>=1)	NumMusic=0;
		else NumMusic++;
		LCDSetPos(1,5);
		LCD12864_WriteData(':');
		LCD12864_WriteData(StrNUMBER[NumMusic]);
		flag11=0xff;
	}
	if(flag11==8){
		if(NumMusic<=0)	NumMusic=1;
		else NumMusic--;
		LCDSetPos(1,5);
		LCD12864_WriteData(':');
		LCD12864_WriteData(StrNUMBER[NumMusic]);
		flag11=0xff;	
	}		
}

/************************************蓝牙闹钟菜单**********************************/
void BTChoice(){
	switch(flag11){
		case 1:                             //设置闹钟开启和关闭
			AlarmOnOff=1-AlarmOnOff;
			break;
		case 7:	                           //进入闹钟时间设置模式
			flagSetATime=1;
			flagTIME=1;
			break;
		case 8:	                           //进入闹钟音乐设置模式
			flagSetAMusic=1;
			flagMUSIC=1;
			break;
		case 5:	                           //进入查看闹钟模式
			flagCheckAlarm=1;
			flagState=1;
			break;
	}
	flag11=0xff;
}