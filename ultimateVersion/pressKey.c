#include <reg52.h>
#include <intrins.h>
#include <DS1302.h>
#include <LCDNew.h>
#include <map.h>
#include <cellPhone.h>
#define uint unsigned int
#define uchar unsigned char 
							 
sbit K1=P1^2;  		     //选择要改变的时间参数	   SW1
sbit K2=P1^3;  		     //设置时间的数据 加按键     SW2
sbit K5=P1^4;			 //减按键	SW4
sbit SET_AM_PM=P1^6;     //SW6
sbit beep=P1^5;          //BEEP口        

uchar idata SetAlarm;               //用于设置闹钟
uchar idata SetState;               //用于选择要调整的数据 秒分时日月周年
uchar idata SetPlace;               //用于选择要调整的数据
uchar idata alarm[3]={0};           //用于装闹钟数据     初始化为0
uchar idata alarm_AM_PM;		    //默认初始化为AM        

uchar BCDToDex(uchar num);
uchar DexToBCD(uchar num);

void DisplayButton(SetPlace){
				switch(SetPlace){
					case 0:	LCDSetPos(1,3);
							LCD12864_WriteData(StrNUMBER[TIME[0]/16]);	    //秒
							if(TIME[0]%16==0)LCD12864_WriteData('0');
							else LCD12864_WriteData(StrNUMBER[TIME[0]%16]);
							break;
					case 1:	LCDSetPos(1,1);
							LCD12864_WriteData(':');
							LCD12864_WriteData(StrNUMBER[TIME[1]/16]);	    //分
							LCD12864_WriteData(StrNUMBER[TIME[1]%16]);
							LCD12864_WriteData(':');
							break;
					case 2: LCDSetPos(1,0);
							LCD12864_WriteData(StrNUMBER[TIME[2]/16]);	    //时
							LCD12864_WriteData(StrNUMBER[TIME[2]%16]);         
							break;
					case 3: LCDSetPos(0,5);                                 //日
							LCD12864_WriteData(StrNUMBER[TIME[3]/16]);
							if(TIME[3]%16==0){
								LCD12864_WriteData('0');	
							}
							else{
								LCD12864_WriteData(StrNUMBER[TIME[3]%16]);
							}
							LCDSetPos(1,7);
							DisplayWord(MAPNumber(TIME[5]%16));
							break;
					case 4:	LCDSetPos(0,3);                                 //月
							LCD12864_WriteData(StrNUMBER[TIME[4]/16]);	    
							LCD12864_WriteData(StrNUMBER[TIME[4]%16]);
							break;
					case 5: LCDSetPos(1,7);
							DisplayWord(MAPNumber(TIME[5]%16));
							break;          //星期的显示暂时不写，到时候来改
					case 6: LCDSetPos(0,1);                                 //年
							LCD12864_WriteData(StrNUMBER[TIME[6]/16]);	    
							LCD12864_WriteData(StrNUMBER[TIME[6]%16]);
							break;
				}
	
}
/******************************************************************************
函数名：Delay10ms
函数功能：延时10ms
输入：无
输出：无
*******************************************************************************/
void Delay10ms(){                                   //误差0us
	uchar a,b,c;
	for(c=1;c>0;c--)
		for(b=38;b>0;b--)
			for(a=130;a>0;a--);}



/******************************************************************************
函数名：scanButton()
函数功能：键盘扫描函数
输入： 
输出：
*******************************************************************************/
void scanButton(){
	uchar i=0;
		if(K1==0 || flag11==5){                      //检测按键K1是否按下
			Delay10ms();                    //消除抖动
			if(K1==0|| flag11==5){
				SetPlace++;
				if(SetPlace>=7)
				SetPlace=0;
			}
			while((i<50)&&(K1==0)){       //检测按键是否松开
				Delay10ms();
				i++;
			}
			i=0;
			if(flag11==5) flag11=0xff;
		}
		/*加功能*/
		if(K2==0|| flag11==7){                         //检测按键K2是否按下
			Delay10ms();                               //消除抖动
			if(K2==0|| flag11==7){
				TIME[SetPlace]++;
				if(SetPlace==3){
					if(TIME[5]>=0x07) TIME[5]=1;
					else TIME[5]++;
				}
				if((TIME[SetPlace]&0x0f)>9){                       //保证BCD码
					TIME[SetPlace]=TIME[SetPlace]+6;
				}
				if((TIME[SetPlace]>=0x60)&&(SetPlace<2)){          //分秒只能到59
					TIME[SetPlace]=0;
				}
				if((TIME[SetPlace]>=0x13)&&(SetPlace==2)){         //小时只能到23
					TIME[SetPlace]=0;
				}
				if((TIME[SetPlace]>=0x32)&&(SetPlace==3)){         //日只能到31
					TIME[SetPlace]=0;
				}
				if((TIME[SetPlace]>=0x13)&&(SetPlace==4)){         //月只能到12
					TIME[SetPlace]=0;
				}
				if((TIME[SetPlace]>=0x8)&&(SetPlace==5)){          //周只能到7
					TIME[SetPlace]=1;
				}
				DisplayButton(SetPlace);
				while((i<50)&&(K2==0)){                             //检测按键是否松开
					Delay10ms();
					i++;}
				i=0;}
				if(flag11==7) flag11=0xff;
		}
		
/*减功能*/
		if(K5==0 || flag11==8){                       //检测按键K2是否按下
			Delay10ms();                              //消除抖动
			if(K5==0 || flag11==8){
				TIME[SetPlace]=BCDToDex(TIME[SetPlace]);         //转换成十进制
				TIME[SetPlace]--;				              //进行减法操作
				TIME[SetPlace]=DexToBCD(TIME[SetPlace]);     //再转换成BCD码
				if(SetPlace==3){
					if(TIME[5]==1) TIME[5]=7;
					else TIME[5]--;
				}
				if((TIME[SetPlace]==0)&&(SetPlace<2)){        //分秒只能到59
					TIME[SetPlace]=0x60;
				}
				if((TIME[SetPlace]==0)&&(SetPlace==2)){       //小时只能到12
					TIME[SetPlace]=0x12;
				}
				if((TIME[SetPlace]==0)&&(SetPlace==3)){       //日只能到31
					TIME[SetPlace]=0x31;
				}
				if((TIME[SetPlace]==0)&&(SetPlace==4)){       //月只能到12
					TIME[SetPlace]=0x12;
				}
				if((TIME[SetPlace]==0)&&(SetPlace==5)){          //周只能到7
					TIME[SetPlace]=0x07;
				}
				DisplayButton(SetPlace);
				while((i<50)&&(K5==0)){                             //检测按键是否松开
					Delay10ms();
					i++;}
				i=0;}
				if(flag11==8) flag11=0xff;
		}
	
		/*设置AM_PM功能*/		
		if(SET_AM_PM==0 || flag11==9){                       //检测按键K2是否按下
			Delay10ms();                        //消除抖动
			if(SET_AM_PM==0|| flag11==9){		            //确认按键确实按下
				AM_PM=1-AM_PM;
			if(AM_PM==0){                     //A
				LCDSetPos(1,4);
				LCD12864_WriteData('A');
				LCD12864_WriteData('M');
			}
			else{
				LCDSetPos(1,4);
				LCD12864_WriteData('P');
				LCD12864_WriteData('M');
			}
				while((i<50)&&(SET_AM_PM==0)){                             //检测按键是否松开
					Delay10ms();
					i++;
				}
				i=0;
			}
			if(flag11==9) flag11=0xff;
		}	

}
/*************************闹钟按键显示函数**************************************/
void DisplayAlarmButton(SetPlace){
				switch(SetPlace){
					case 0:	LCDSetPos(1,5);
							LCD12864_WriteData(StrNUMBER[alarm[0]/16]);	    //秒
							LCD12864_WriteData(StrNUMBER[alarm[0]%16]);
							break;
					case 1:	LCDSetPos(1,3);
							LCD12864_WriteData(':');
							LCD12864_WriteData(StrNUMBER[alarm[1]/16]);	    //分
							LCD12864_WriteData(StrNUMBER[alarm[1]%16]);
							LCD12864_WriteData(':');
							break;
					case 2: LCDSetPos(1,2);
							LCD12864_WriteData(StrNUMBER[alarm[2]/16]);	    //时
							LCD12864_WriteData(StrNUMBER[alarm[2]%16]);         
							break;
				}		
}

/******************************************************************************
函数名：scanButton_alarm()
函数功能：用于闹钟设置的键盘扫描函数
输入： 
输出：
*******************************************************************************/
void scanButton_alarm(){
	uchar i=0;
		if(K1==0 || flag11==5){                      //检测按键K1是否按下
			Delay10ms();                    //消除抖动
			if(K1==0|| flag11==5){
				SetPlace++;
				if(SetPlace>=3)
				SetPlace=0;
			}
			while((i<50)&&(K1==0)){       //检测按键是否松开
				Delay10ms();
				i++;
			}
			i=0;
			if(flag11==5)flag11=0xff;
		}
		/*加功能*/
		if(K2==0 || flag11==7){                       //检测按键K2是否按下
			Delay10ms();                  //消除抖动
			if(K2==0|| flag11==7){
				alarm[SetPlace]++;
				if((alarm[SetPlace]&0x0f)>9){                       //保证BCD码
					alarm[SetPlace]=alarm[SetPlace]+6;
				}
				if((alarm[SetPlace]>=0x60)&&(SetPlace<2)){          //分秒只能到59
					alarm[SetPlace]=0;
				}
				if((alarm[SetPlace]>=0x13)&&(SetPlace==2)){         //小时只能到12
					alarm[SetPlace]=0;
				}
				DisplayAlarmButton(SetPlace);
				while((i<50)&&(K2==0)){                             //检测按键是否松开
					Delay10ms();
					i++;}
				i=0;}
				if(flag11==7) flag11=0xff;
		}
				/*减功能*/
		if(K5==0|| flag11==8){                       //检测按键K2是否按下
			Delay10ms();                  //消除抖动
			if(K5==0 || flag11==8){
				alarm[SetPlace]=BCDToDex(alarm[SetPlace]);         //转换成十进制
				alarm[SetPlace]--;				                   //进行减法操作
				alarm[SetPlace]=DexToBCD(alarm[SetPlace]);         //再转换成BCD码
				if((alarm[SetPlace]==0)&&(SetPlace<2)){          //分秒只能到59
					alarm[SetPlace]=0x60;
				}
				if((alarm[SetPlace]==0)&&(SetPlace==2)){         //小时只能到23
					alarm[SetPlace]=0x12;
				}
				DisplayAlarmButton(SetPlace);
				while((i<50)&&(K5==0)){                             //检测按键是否松开
					Delay10ms();
					i++;}
				i=0;}
				if(flag11==8) flag11=0xff;
		}
	
		/*设置AM_PM功能*/		
		if(SET_AM_PM==0 || flag11==9){                       //检测按键K2是否按下
			Delay10ms();                        //消除抖动
			if(SET_AM_PM==0 || flag11==9){
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
				while((i<50)&&(SET_AM_PM==0)){                             //检测按键是否松开
					Delay10ms();
					i++;
				}
				i=0;
				if(flag11==9) flag11=0xff;
			}

		}	

}

uchar BCDToDex(uchar num){
	return (num/16)*10+(num%16);	
}

uchar DexToBCD(uchar num){
	return (num/10)*16+(num%10);
}


