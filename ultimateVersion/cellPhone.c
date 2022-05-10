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

/*****************************�������������ӵĺ���***********************************/
/*
void BtSetAlarm(){		
	if(flag11==5){
		SetPlace++;
		if(SetPlace>=3)
			SetPlace=0;
		flag11=0xff;	 //��״̬��������״̬
	}

	if(flag11==7){
		alarm[SetPlace]++;
		DisplayAlarmButton(SetPlace);
		if((alarm[SetPlace]&0x0f)>9){                       //��֤BCD��
			alarm[SetPlace]=alarm[SetPlace]+6;
		}
		if((alarm[SetPlace]>=0x60)&&(SetPlace<2)){          //����ֻ�ܵ�59
			alarm[SetPlace]=0;
		}
		if((alarm[SetPlace]>=0x12)&&(SetPlace==2)){         //Сʱֻ�ܵ�23
			alarm[SetPlace]=0;
		}
		flag11=0xff;	//��״̬��������״̬
	}

	if(flag11==8){
		if(alarm[SetPlace]&0x0f<=0){
			alarm[SetPlace]=alarm[SetPlace]-6;			    //��֤BCD�����
		}
		if((alarm[SetPlace]==0x00)&&(SetPlace<2)){          //����ֻ�ܵ�59
			alarm[SetPlace]=0x60;
		}
		if((alarm[SetPlace]==0x00)&&(SetPlace==2)){         //Сʱֻ�ܵ�12
			alarm[SetPlace]=0x12;
		}
		alarm[SetPlace]--;
		DisplayAlarmButton(SetPlace);
		flag11=0xff;	                                    //��״̬��������״̬	
	}
/*

			/*����AM_PM����*/	
/*	
	if(flag11==9){                       //��ⰴ��K2�Ƿ���
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
		flag11=0xff;	                                    //��״̬��������״̬
	}
}


*/
/******************************�����������ֺ���***********************************/
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

/************************************�������Ӳ˵�**********************************/
void BTChoice(){
	switch(flag11){
		case 1:                             //�������ӿ����͹ر�
			AlarmOnOff=1-AlarmOnOff;
			break;
		case 7:	                           //��������ʱ������ģʽ
			flagSetATime=1;
			flagTIME=1;
			break;
		case 8:	                           //����������������ģʽ
			flagSetAMusic=1;
			flagMUSIC=1;
			break;
		case 5:	                           //����鿴����ģʽ
			flagCheckAlarm=1;
			flagState=1;
			break;
	}
	flag11=0xff;
}