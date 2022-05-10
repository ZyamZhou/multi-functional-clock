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
uchar idata flagSetAlarmTriggered=0;            //һ�������жϿ�����һ���Ժ���Զ��1
uchar idata AlarmOnOff=0;						  //���ӹر���0 ���������1	    ��ʼ��Ϊ���ӹر�
uchar idata flagState=0;              
/****************************ѡ����������**********************************/
void DisplayChoice(){
	LCDSetPos(0,0);
	DisplayWord("����״̬��");		     //����SW6�������������ӵĹرպͿ���   SW6����SET_AM_PM
	if(AlarmOnOff==0){
		LCDSetPos(0,5);
		DisplayWord("�ر�");	
	}
	if(AlarmOnOff==1){
		LCDSetPos(0,5);
		DisplayWord("����");
	}
	LCDSetPos(1,0);
	DisplayWord("��������ʱ�䰴+");
	LCDSetPos(2,0);
	DisplayWord("�����������ְ�-");
	LCDSetPos(3,0);
	DisplayWord("�鿴���Ӱ�SW1");        //to����SW1 ����K1
}


//K2��+�� K5��-��
void scanChoiceButton(){
	uchar i=0;
	if(SET_AM_PM==0){                   
		Delay10ms();
		if(SET_AM_PM==0){	                    
			AlarmOnOff=1-AlarmOnOff;    //���ӿ������߹ر�
		}
		while((i<50)&&(SET_AM_PM==0)){         
			Delay10ms();
			i++;}
		i=0;			
	}
	if(K2==0){			                 //������������ʱ��
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
	if(K5==0){                         //����������������
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
	if(K1==0){			                //����鿴����״̬
		Delay10ms();
		if(K1==0){
			flagCheckAlarm=1;
			flagState=1;
		}
		while((i<50)&&(K1==0)){         //��ⰴ���Ƿ��ɿ�
			Delay10ms();
			i++;}
		i=0;	
	}	
}

void DisplaySetMusic(){
	LCD12864_WriteCmd(0x01);    //���LCD12864����ʾ����
	LCDSetPos(0,1);
	DisplayWord("������������");
	LCDSetPos(1,1);
	DisplayWord("�������");
	LCD12864_WriteData(':');
	LCD12864_WriteData(StrNUMBER[NumMusic]);
}

//K2��+�� K5��-��
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
		while((i<50)&&(K2==0)){             //��ⰴ���Ƿ��ɿ�
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
		while((i<50)&&(K5==0)){            //��ⰴ���Ƿ��ɿ�
			Delay10ms();
			i++;}
		i=0;	
	}		
}