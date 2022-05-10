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

sbit K3=P3^2;    		 //触发设置时间功能(定义在main函数里面)	 SET功能
sbit K4=P3^3;            //触发设置闹钟	  alarm功能
sbit LED1=P2^4;          //闹钟灯 高电平驱动
sbit MUSIC0=P1^0;	     //闹钟音乐
sbit MUSIC1=P1^1;	     //闹中音乐
sbit BEEP=P1^5;
sbit DOUT=P1^7;

uchar flagA=0;
uchar flagLED=0;         //LED开启状态函数
uchar acc=0;             //记录LED闪烁次数
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
/******************************main函数***********************************/
void main(){
	FireOn();
	Uart0_Init();
	LCD12864_Init();
	Ds1302Init();
	AM_PM=(bit)(TIME[2]&0x20>>5);
	DHT11_Init();                             //越过DHT11 1s不稳定期
	IntConfiguration();                       //开中断
	LED1=0;	                                  //LED初始化
	alarm_AM_PM=0;                            //闹钟AM/PM初始化
	BEEP=0;
	while(1){
		FireOn();
		switch(flag11){
			case 6:		
				if(SetAlarm==0){
					flagA=1;                       //蓝牙alarm 键	                                
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
			case 4:					              //蓝牙设置时间键
				SetState=~SetState;
				SetPlace=0;
				Ds1302Init();
				break;				
		}
		flag11=0xff;                              //置空状态
		if(flagClear==1){
			LCD12864_WriteCmd(0x01);          //清除LCD12864的显示内容
			flagClear=0;                          
		}
		if(AlarmOnOff==1){		              //闹钟开启状态（确认无误）
			AlarmOn();                   	
		}
		if(flagAlarmTriggered==1){		      //当闹钟被触发（确认无误）
			AlarmTriggered();
			flagAlarmTriggered=0;	
		}
		if((SetState==0) && (SetAlarm==0)){	  //主界面状态(确认无误）
			LCDdisplaywhole();                //显示主界面
		}
		if(SetState!=0 && SetAlarm==0 ){	  //进入时间设置状态
			scanButton();                     //按键设置时间
		}
		if(SetState==0 && SetAlarm!=0){       //进入闹钟菜单
			if(flagA==1){
				LCD12864_WriteCmd(0x01);      //清除LCD12864的显示内容
				flagA=0;	
			}
			AlarmMenu();
		}				
	}
}
/********************************显示时刻的函数*********************************/
void DisplayTime(){
		/*显示时间*/
		LCDSetPos(1,0);           //选位置
		LCD12864_WriteData(StrNUMBER[hour/16]);
		LCD12864_WriteData(StrNUMBER[hour%16]);
		LCD12864_WriteData(':');
		LCD12864_WriteData(StrNUMBER[minute/16]);
		LCD12864_WriteData(StrNUMBER[minute%16]);
		LCD12864_WriteData(':');
		LCD12864_WriteData(StrNUMBER[second/16]);
		LCD12864_WriteData(StrNUMBER[second%16]);
		/*显示AM PM*/
		if(AM_PM==0){
			LCD12864_WriteData('A');
		}
		else{
			LCD12864_WriteData('P');
		}
		LCD12864_WriteData('M');	
}
/***********************************LCD总显示函数****************************************/
void LCDdisplaywhole(){
	Ds1302ReadTime();
	convert(year,month,date);     //进行阴阳历转换
	DHT11();                        //读取温湿度
		LCDSetPos(0,0);             //选位置
		LCD12864_WriteData('2');
		LCD12864_WriteData('0');
		LCD12864_WriteData(StrNUMBER[year/16]);
		LCD12864_WriteData(StrNUMBER[year%16]);
		DisplayWord("年");	          
		/*显示月*/
		LCD12864_WriteData(StrNUMBER[month/16]);
		LCD12864_WriteData(StrNUMBER[month%16]);
		DisplayWord("月");
		/*显示日*/
		LCD12864_WriteData(StrNUMBER[date/16]);
		LCD12864_WriteData(StrNUMBER[date%16]);
		DisplayWord("日");
		//换行
		LCDSetPos(1,0);           //选位置
		DisplayTime();
		DisplayWord("星");
		DisplayWord("期");
		DisplayWord(MAPNumber(day)); 
		//换行
		LCDSetPos(2,0);
		//显示二十四节气
		if((convertBCDtoHex(month)==(code3/2+1))&&(convertBCDtoHex(date)==SolarTerm[code3])){
			DisplayWord(MAPSolar(code3));
		}
		else{
			DisplayWord("    ");
		}
		DisplayLunar();
		//显示温湿度
		LCDSetPos(3,0);           //选位置
		DisplayWord("温度");
		LCD12864_WriteData(':');
		LCD12864_WriteData(StrNUMBER[temprature/10]);
		LCD12864_WriteData(StrNUMBER[temprature%10]);
		LCD12864_WriteData(' ');
		DisplayWord("湿度");
		LCD12864_WriteData(':');
		LCD12864_WriteData(StrNUMBER[hum/10]);
		LCD12864_WriteData(StrNUMBER[hum%10]);	
}

/*********************************显示阴历日期函数***************************************/
void DisplayLunar(){
		lunar_date=lunar_date-1;	
		//显示阴历日期
		if(lunar_month==1){
			DisplayWord("正");
		}
		else if(lunar_month/10==0 && lunar_month%10!=0){
			DisplayWord(MAPNumber(lunar_month/10));
			DisplayWord(MAPNumber(lunar_month%10));	
		}
		else if(lunar_month/10==1 && lunar_month%10==0){
			DisplayWord("十");
		}
		else if(lunar_month%10==1 && lunar_month%10!=0){
			DisplayWord(MAPNumber(lunar_month/10));
			DisplayWord(MAPNumber(lunar_month%10));    
		}
		else{
			DisplayWord("腊");
		}
		DisplayWord("月");
				                     
		if((lunar_date/10==0) || (lunar_date==10)){
			DisplayWord("初");
		}
		else if(lunar_date/10==1){
			DisplayWord("十");	              //十，初十是一种特殊情况
		}
		else if(lunar_date/10==2){
			DisplayWord("廿");
		}
		else{
			DisplayWord("三");      
		}
		if(lunar_date%10!=0){
			DisplayWord(MAPNumber(lunar_date%10));
		}
		else{
			DisplayWord("十");	
		}	
}

/******************************************************************************
函数名：DisplayAlarmOn()
函数功能：当闹钟按键被按下时的显示函数
输入： 无
输出：无
*******************************************************************************/
void DisplayAlarmOn(){
	LCD12864_WriteCmd(0x01);    //清除LCD12864的显示内容
	LCDSetPos(0,1);
	DisplayWord("设置闹钟时间");
	/*显示时间*/
	LCDSetPos(1,2);           //选位置
	LCD12864_WriteData(StrNUMBER[alarm[2]/16]);
	LCD12864_WriteData(StrNUMBER[alarm[2]%16]);
	LCD12864_WriteData(':');
	LCD12864_WriteData(StrNUMBER[alarm[1]/16]);
	LCD12864_WriteData(StrNUMBER[alarm[1]%16]);
	LCD12864_WriteData(':');
	LCD12864_WriteData(StrNUMBER[alarm[0]/16]);
	LCD12864_WriteData(StrNUMBER[alarm[0]%16]);
	/*显示AM PM*/
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
函数名：Int0Configuration()
函数功能：配置外部中断0
输入： 无
输出：无
*******************************************************************************/
void IntConfiguration(){
	//设置INT0
	IT0=1;				//使能INT0为下降沿中断
	IT1=1;              //使能INT1为下降沿中断
	EX0=1;              //外部中断0允许位
	EX1=1;              //外部中断1允许位，允许外部中断。
	EA=1;               //中断允许 （打开总中断）

}
/***************************外部中断0服务子程序******************************/
void Int0() interrupt 0{
	Delay10ms();
	if(K3==0){
		SetState=~SetState;
		SetPlace=0;
		Ds1302Init(); 
	}
}

/***************************外部中断1服务子程序******************************/
void Int2() interrupt 2{
	Delay10ms();
	if(K4==0){
		if(SetAlarm==0){
			flagA=1;                       //每当闹钟设置按键被触发的时候设置一次初始化	                                
		}
		if(SetAlarm!=0){
			flagSetATime=0;
			flagSetAMusic=0;
			flagCheckAlarm=0;
			flagClear=1;                   //当从闹钟菜单界面重新返回主界面的时候用于清屏的变量
		}
		SetAlarm=~SetAlarm;
		SetPlace=0;
	}
}
/*********************延时子程序*************************/
void delay_ms(uint xms){
	uint i,j;
	for(i=xms;i>0;i--){
		for(j=110;j>0;j--);
	}	
}



/***********************************闹钟触发函数**************************************/

void AlarmOn(){                                   //闹钟响起函数
	uchar i=0;
	if((second==alarm[0])&&(minute==alarm[1]) && (hour==alarm[2]) && (AM_PM==alarm_AM_PM)&&(SetAlarm==0)){
		flagLED=1;
		flagAlarmTriggered=1;
	}

}



/***************************************闹钟菜单************************************/
void AlarmMenu(){
	if(flagSetATime==0 && flagSetAMusic==0 && flagCheckAlarm==0){
		DisplayChoice();
		BTChoice();
		scanChoiceButton();
	}
	if(flagSetATime==1){	 //进入设置闹钟时间界面
		if(flagTIME==1){
			DisplayAlarmOn();		     //显示设置闹钟时间界面
		}
		flagTIME=0;
		scanButton_alarm();
		//BtSetAlarm();
	}
	if(flagSetAMusic==1){   //进入设置闹钟音乐界面 
		if(flagMUSIC==1){
			DisplaySetMusic();
		}
		flagMUSIC=0;
		scanButtonSetMusic();
		BtSetAlarmMusic();				
	}
	if(flagCheckAlarm==1){	  //进入闹钟状态查看界面
		if(flagState==1){
			LCD12864_WriteCmd(0x01);      //就清一次屏
			flagState=0;
		} 
		if(AlarmOnOff==0){                //当闹钟没有打开
			LCDSetPos(0,1);
			DisplayWord("闹钟未开启！");	
		}
		else{                            //当闹钟已经开启
			LCDSetPos(0,2);
			DisplayWord("闹钟时间");
			/*显示时间*/
			LCDSetPos(1,2);        
			LCD12864_WriteData(StrNUMBER[alarm[2]/16]);
			LCD12864_WriteData(StrNUMBER[alarm[2]%16]);
			LCD12864_WriteData(':');
			LCD12864_WriteData(StrNUMBER[alarm[1]/16]);
			LCD12864_WriteData(StrNUMBER[alarm[1]%16]);
			LCD12864_WriteData(':');
			LCD12864_WriteData(StrNUMBER[alarm[0]/16]);
			LCD12864_WriteData(StrNUMBER[alarm[0]%16]);
			/*显示AM PM*/
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

/*************************************闹钟响起时*****************************************/
void AlarmTriggered(){
	LCD12864_WriteCmd(0x01);    //清除LCD12864的显示内容
	LCDSetPos(0,0);
	DisplayWord("别睡了起床啦！");
	switch(NumMusic){	       //选择音乐
		case 0:
			MUSIC0=0;
			break;
		case 1:
			MUSIC1=0;
			break;	
	}
	if(flagLED==1){	       		            //闹钟响起30sLED闪烁30次
		//扫描按键
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


/*中断函数*/
void Uart() interrupt 4	  //串行口中断
{
    UartDate=SBUF;        //取出接受到的数据
    RI=0;			      //清除接受中断标志位
	flag11=UartDate;
}

void delay(){                                   //误差0us
	uchar a,b,c;
	for(c=1;c>0;c--)
		for(b=38;b>0;b--)
			for(a=130;a>0;a--);}

void PostString(uchar *p)  
{  
	while(*p)  	   //若指针指向的地址为空，则跳出循环
	{  
		sendDate(*p); //指针第一次默认指向首地址
		delay();  //延时，作用为提高发送准确度
		p++;  
	}	  
}

void delayD()//延时程序
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
			LCD12864_WriteCmd(0x01);    //清除LCD12864的显示内容
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
		DisplayWord("晴天");
	}
	if(num==14){
		LCDSetPos(1,3);
		DisplayWord("阴天");		
	}
	if(num==15){
		LCDSetPos(1,3);
		DisplayWord("雨天");		
	}			
}