#include "reg52.h"
#include "intrins.h"
#include "string.h"
#include "DS1302.h"
#include "LCDNew.h"
#include "lunarCalendar.h"
#include "DHT11.h"
#include "map.h"
#include "pressKey.h"
#include "SelectAlarm.h"
#include "blueTeeth.h"
#include "cellPhone.h"

#define uchar unsigned char 
#define uint unsigned int

sbit K3=P3^2;    		 //��������ʱ�书��(������main��������)	 SET����
sbit K4=P3^3;            //������������	  alarm����
sbit LED1=P2^4;          //���ӵ� �ߵ�ƽ����
sbit MUSIC0=P1^0;	     //��������
sbit MUSIC1=P1^1;	     //��������
sbit BEEP=P1^5;
sbit DOUT=P1^7;

uchar flagA=0;
uchar flagLED=0;         //LED����״̬����
uchar acc=0;             //��¼LED��˸����
uchar flagAlarmTriggered=0;
uchar flagClear=0;
uchar UartDate=0;
uchar flagWeather=0;

void DisplayTime();
void IntConfiguration();
void DisplayLunar();	
void LCDdisplaywhole();
void DisplayAlarmOn();
void AlarmOn();
void delay_ms(uint xms);
void AlarmMenu();
void AlarmTriggered();
void PostString(uchar *p);
void FireOn();
void Weather(uchar num);
/******************************main����***********************************/
void main(){
	FireOn();
	Uart0_Init();
	LCD12864_Init();
	Ds1302Init();
	AM_PM=(bit)(TIME[2]&0x20>>5);
	DHT11_Init();                             //Խ��DHT11 1s���ȶ���
	IntConfiguration();                       //���ж�
	LED1=0;	                                  //LED��ʼ��
	alarm_AM_PM=0;                            //����AM/PM��ʼ��
	BEEP=0;
	while(1){
		FireOn();
		switch(flag11){
			case 6:		
				if(SetAlarm==0){
					flagA=1;                       //����alarm ��	                                
				}
				if(SetAlarm!=0){
					flagSetATime=0;
					flagSetAMusic=0;
					flagCheckAlarm=0;
					flagClear=1;                   
				}
				SetAlarm=~SetAlarm;
				SetPlace=0;
				break;
			case 4:					              //��������ʱ���
				SetState=~SetState;
				SetPlace=0;
				Ds1302Init();
				break;				
		}
		flag11=0xff;                              //�ÿ�״̬
		if(flagClear==1){
			LCD12864_WriteCmd(0x01);          //���LCD12864����ʾ����
			flagClear=0;                          
		}
		if(AlarmOnOff==1){		              //���ӿ���״̬��ȷ������
			AlarmOn();                   	
		}
		if(flagAlarmTriggered==1){		      //�����ӱ�������ȷ������
			AlarmTriggered();
			flagAlarmTriggered=0;	
		}
		if((SetState==0) && (SetAlarm==0)){	  //������״̬(ȷ������
			LCDdisplaywhole();                //��ʾ������
		}
		if(SetState!=0 && SetAlarm==0 ){	  //����ʱ������״̬
			scanButton();                     //��������ʱ��
		}
		if(SetState==0 && SetAlarm!=0){       //�������Ӳ˵�
			if(flagA==1){
				LCD12864_WriteCmd(0x01);      //���LCD12864����ʾ����
				flagA=0;	
			}
			AlarmMenu();
		}				
	}
}
/********************************��ʾʱ�̵ĺ���*********************************/
void DisplayTime(){
		/*��ʾʱ��*/
		LCDSetPos(1,0);           //ѡλ��
		LCD12864_WriteData(StrNUMBER[hour/16]);
		LCD12864_WriteData(StrNUMBER[hour%16]);
		LCD12864_WriteData(':');
		LCD12864_WriteData(StrNUMBER[minute/16]);
		LCD12864_WriteData(StrNUMBER[minute%16]);
		LCD12864_WriteData(':');
		LCD12864_WriteData(StrNUMBER[second/16]);
		LCD12864_WriteData(StrNUMBER[second%16]);
		/*��ʾAM PM*/
		if(AM_PM==0){
			LCD12864_WriteData('A');
		}
		else{
			LCD12864_WriteData('P');
		}
		LCD12864_WriteData('M');	
}
/***********************************LCD����ʾ����****************************************/
void LCDdisplaywhole(){
	Ds1302ReadTime();
	convert(year,month,date);     //����������ת��
	DHT11();                        //��ȡ��ʪ��
		LCDSetPos(0,0);             //ѡλ��
		LCD12864_WriteData('2');
		LCD12864_WriteData('0');
		LCD12864_WriteData(StrNUMBER[year/16]);
		LCD12864_WriteData(StrNUMBER[year%16]);
		DisplayWord("��");	          
		/*��ʾ��*/
		LCD12864_WriteData(StrNUMBER[month/16]);
		LCD12864_WriteData(StrNUMBER[month%16]);
		DisplayWord("��");
		/*��ʾ��*/
		LCD12864_WriteData(StrNUMBER[date/16]);
		LCD12864_WriteData(StrNUMBER[date%16]);
		DisplayWord("��");
		//����
		LCDSetPos(1,0);           //ѡλ��
		DisplayTime();
		DisplayWord("��");
		DisplayWord("��");
		DisplayWord(MAPNumber(day)); 
		//����
		LCDSetPos(2,0);
		//��ʾ��ʮ�Ľ���
		if((convertBCDtoHex(month)==(code3/2+1))&&(convertBCDtoHex(date)==SolarTerm[code3])){
			DisplayWord(MAPSolar(code3));
		}
		else{
			DisplayWord("    ");
		}
		DisplayLunar();
		//��ʾ��ʪ��
		LCDSetPos(3,0);           //ѡλ��
		DisplayWord("�¶�");
		LCD12864_WriteData(':');
		LCD12864_WriteData(StrNUMBER[temprature/10]);
		LCD12864_WriteData(StrNUMBER[temprature%10]);
		LCD12864_WriteData(' ');
		DisplayWord("ʪ��");
		LCD12864_WriteData(':');
		LCD12864_WriteData(StrNUMBER[hum/10]);
		LCD12864_WriteData(StrNUMBER[hum%10]);	
}

/*********************************��ʾ�������ں���***************************************/
void DisplayLunar(){
		lunar_date=lunar_date-1;	
		//��ʾ��������
		if(lunar_month==1){
			DisplayWord("��");
		}
		else if(lunar_month/10==0 && lunar_month%10!=0){
			DisplayWord(MAPNumber(lunar_month/10));
			DisplayWord(MAPNumber(lunar_month%10));	
		}
		else if(lunar_month/10==1 && lunar_month%10==0){
			DisplayWord("ʮ");
		}
		else if(lunar_month%10==1 && lunar_month%10!=0){
			DisplayWord(MAPNumber(lunar_month/10));
			DisplayWord(MAPNumber(lunar_month%10));    
		}
		else{
			DisplayWord("��");
		}
		DisplayWord("��");
				                     
		if((lunar_date/10==0) || (lunar_date==10)){
			DisplayWord("��");
		}
		else if(lunar_date/10==1){
			DisplayWord("ʮ");	              //ʮ����ʮ��һ���������
		}
		else if(lunar_date/10==2){
			DisplayWord("إ");
		}
		else{
			DisplayWord("��");      
		}
		if(lunar_date%10!=0){
			DisplayWord(MAPNumber(lunar_date%10));
		}
		else{
			DisplayWord("ʮ");	
		}	
}

/******************************************************************************
��������DisplayAlarmOn()
�������ܣ������Ӱ���������ʱ����ʾ����
���룺 ��
�������
*******************************************************************************/
void DisplayAlarmOn(){
	LCD12864_WriteCmd(0x01);    //���LCD12864����ʾ����
	LCDSetPos(0,1);
	DisplayWord("��������ʱ��");
	/*��ʾʱ��*/
	LCDSetPos(1,2);           //ѡλ��
	LCD12864_WriteData(StrNUMBER[alarm[2]/16]);
	LCD12864_WriteData(StrNUMBER[alarm[2]%16]);
	LCD12864_WriteData(':');
	LCD12864_WriteData(StrNUMBER[alarm[1]/16]);
	LCD12864_WriteData(StrNUMBER[alarm[1]%16]);
	LCD12864_WriteData(':');
	LCD12864_WriteData(StrNUMBER[alarm[0]/16]);
	LCD12864_WriteData(StrNUMBER[alarm[0]%16]);
	/*��ʾAM PM*/
	LCDSetPos(2,3);
	LCD12864_WriteData(' ');
	if(alarm_AM_PM==0){
		LCD12864_WriteData('A');
	}
	if(alarm_AM_PM==1){
		LCD12864_WriteData('P');
	}
	LCD12864_WriteData('M');
			
}

/******************************************************************************
��������Int0Configuration()
�������ܣ������ⲿ�ж�0
���룺 ��
�������
*******************************************************************************/
void IntConfiguration(){
	//����INT0
	IT0=1;				//ʹ��INT0Ϊ�½����ж�
	IT1=1;              //ʹ��INT1Ϊ�½����ж�
	EX0=1;              //�ⲿ�ж�0����λ
	EX1=1;              //�ⲿ�ж�1����λ�������ⲿ�жϡ�
	EA=1;               //�ж����� �������жϣ�

}
/***************************�ⲿ�ж�0�����ӳ���******************************/
void Int0() interrupt 0{
	Delay10ms();
	if(K3==0){
		SetState=~SetState;
		SetPlace=0;
		Ds1302Init(); 
	}
}

/***************************�ⲿ�ж�1�����ӳ���******************************/
void Int2() interrupt 2{
	Delay10ms();
	if(K4==0){
		if(SetAlarm==0){
			flagA=1;                       //ÿ���������ð�����������ʱ������һ�γ�ʼ��	                                
		}
		if(SetAlarm!=0){
			flagSetATime=0;
			flagSetAMusic=0;
			flagCheckAlarm=0;
			flagClear=1;                   //�������Ӳ˵��������·����������ʱ�����������ı���
		}
		SetAlarm=~SetAlarm;
		SetPlace=0;
	}
}
/*********************��ʱ�ӳ���*************************/
void delay_ms(uint xms){
	uint i,j;
	for(i=xms;i>0;i--){
		for(j=110;j>0;j--);
	}	
}



/***********************************���Ӵ�������**************************************/

void AlarmOn(){                                   //����������
	uchar i=0;
	if((second==alarm[0])&&(minute==alarm[1]) && (hour==alarm[2]) && (AM_PM==alarm_AM_PM)&&(SetAlarm==0)){
		flagLED=1;
		flagAlarmTriggered=1;
	}

}



/***************************************���Ӳ˵�************************************/
void AlarmMenu(){
	if(flagSetATime==0 && flagSetAMusic==0 && flagCheckAlarm==0){
		DisplayChoice();
		BTChoice();
		scanChoiceButton();
	}
	if(flagSetATime==1){	 //������������ʱ�����
		if(flagTIME==1){
			DisplayAlarmOn();		     //��ʾ��������ʱ�����
		}
		flagTIME=0;
		scanButton_alarm();
		//BtSetAlarm();
	}
	if(flagSetAMusic==1){   //���������������ֽ��� 
		if(flagMUSIC==1){
			DisplaySetMusic();
		}
		flagMUSIC=0;
		scanButtonSetMusic();
		BtSetAlarmMusic();				
	}
	if(flagCheckAlarm==1){	  //��������״̬�鿴����
		if(flagState==1){
			LCD12864_WriteCmd(0x01);      //����һ����
			flagState=0;
		} 
		if(AlarmOnOff==0){                //������û�д�
			LCDSetPos(0,1);
			DisplayWord("����δ������");	
		}
		else{                            //�������Ѿ�����
			LCDSetPos(0,2);
			DisplayWord("����ʱ��");
			/*��ʾʱ��*/
			LCDSetPos(1,2);        
			LCD12864_WriteData(StrNUMBER[alarm[2]/16]);
			LCD12864_WriteData(StrNUMBER[alarm[2]%16]);
			LCD12864_WriteData(':');
			LCD12864_WriteData(StrNUMBER[alarm[1]/16]);
			LCD12864_WriteData(StrNUMBER[alarm[1]%16]);
			LCD12864_WriteData(':');
			LCD12864_WriteData(StrNUMBER[alarm[0]/16]);
			LCD12864_WriteData(StrNUMBER[alarm[0]%16]);
			/*��ʾAM PM*/
			LCDSetPos(2,3);
			LCD12864_WriteData(' ');
			if(alarm_AM_PM==0){
				LCD12864_WriteData('A');
			}
			if(alarm_AM_PM==1){
				LCD12864_WriteData('P');
			}
			LCD12864_WriteData('M');	
		}	
	}	
}

/*************************************��������ʱ*****************************************/
void AlarmTriggered(){
	LCD12864_WriteCmd(0x01);    //���LCD12864����ʾ����
	LCDSetPos(0,0);
	DisplayWord("��˯��������");
	switch(NumMusic){	       //ѡ������
		case 0:
			MUSIC0=0;
			break;
		case 1:
			MUSIC1=0;
			break;	
	}
	if(flagLED==1){	       		            //��������30sLED��˸30��
		//ɨ�谴��
		acc=30;
		while(acc){
		LED1=1;
		BEEP=1;
		delay_ms(500);
		LED1=0;
		BEEP=0;
		delay_ms(500);
		acc--;			
		}
		flagLED=0;
	}
	switch(NumMusic){
		case 0:
			MUSIC0=1;
			break;
		case 1:
			MUSIC1=1;
			break;	
	}
	LCD12864_WriteCmd(0x01);	
}


/*�жϺ���*/
void Uart() interrupt 4	  //���п��ж�
{
    UartDate=SBUF;        //ȡ�����ܵ�������
    RI=0;			      //��������жϱ�־λ
	flag11=UartDate;
}

void delay(){                                   //���0us
	uchar a,b,c;
	for(c=1;c>0;c--)
		for(b=38;b>0;b--)
			for(a=130;a>0;a--);}

void PostString(uchar *p)  
{  
	while(*p)  	   //��ָ��ָ��ĵ�ַΪ�գ�������ѭ��
	{  
		sendDate(*p); //ָ���һ��Ĭ��ָ���׵�ַ
		delay();  //��ʱ������Ϊ��߷���׼ȷ��
		p++;  
	}	  
}

void delayD()//��ʱ����
{
uchar m,n,s;
for(m=20;m>0;m--)
for(n=20;n>0;n--)
for(s=248;s>0;s--);
}

void smoke(){
	if(DOUT==0){
		delayD();
		if(DOUT==0){
			while(!DOUT){
			LCD12864_WriteCmd(0x01);    //���LCD12864����ʾ����
			delay_ms(300);
			LCDSetPos(1,2);
			DisplayWord("FIRE!RUN!");
			delay_ms(300);
			}
			BEEP=1;
			PostString("FIRE!!");	
		}
	}
}

void FireOn(){
	smoke();
}

void Weather(uchar num){
	if(num==13){
		LCDSetPos(1,3);
		DisplayWord("����");
	}
	if(num==14){
		LCDSetPos(1,3);
		DisplayWord("����");		
	}
	if(num==15){
		LCDSetPos(1,3);
		DisplayWord("����");		
	}			
}