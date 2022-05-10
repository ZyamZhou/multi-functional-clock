#include <reg52.h>
#include <intrins.h>
#include <DS1302.h>
#include <LCD.h>
#define uint unsigned int
#define uchar unsigned char 

sbit K1=P3^0;  		     //选择要改变的时间参数
sbit K2=P3^1;  		     //设置时间的数据 加按键
sbit K5=P1^6;			 //减按键
sbit SET_AM_PM=P1^7;
sbit beep=P1^5;

uchar idata SetAlarm;               //用于设置闹钟
uchar idata SetState;               //用于选择要调整的数据 秒分时日月周年
uchar idata SetPlace;               //用于选择要调整的数据
uchar idata alarm[3];               //用于装闹钟数据
uchar idata alarm_AM_PM;

uchar BCDToDex(uchar num){
	return (num/16)*10+(num%16);	
}

uchar DexToBCD(uchar num){
	return (num/10)*16+(num%10);
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

/******************************************************************************
函数名：Delay10ms
函数功能：延时10ms
输入： 无
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
		if(K1==0){                      //检测按键K1是否按下
			Delay10ms();                    //消除抖动
			if(K1==0){
				SetPlace++;
				if(SetPlace>=7)
				SetPlace=0;
			}
			while((i<50)&&(K1==0)){       //检测按键是否松开
				Delay10ms();
				i++;
			}
			i=0;
		}
		/*加功能*/
		if(K2==0){                       //检测按键K2是否按下
			Delay10ms();                  //消除抖动
			if(K2==0){
				TIME[SetPlace]++;
				switch(SetPlace){
					case 0:	DisplayNumber(1,2,6*8,TIME[0]/16);
							DisplayNumber(1,2,7*8,TIME[0]%16);
							break;
					case 1:	DisplayNumber(1,2,3*8,TIME[1]/16);
							DisplayNumber(1,2,4*8,TIME[1]%16);
							break;
					case 2: DisplayNumber(1,2,0*8,TIME[2]/16);
							DisplayNumber(1,2,1*8,TIME[2]%16);
							break;
					case 3: DisplayNumber(2,0,1*16,TIME[3]/16);
							DisplayNumber(2,0,1*8+1*16,TIME[3]%16);
							break;
					case 4:	DisplayNumber(1,0,4*8+1*16,TIME[4]/16);
							DisplayNumber(1,0,5*8+1*16,TIME[4]%16);
							break;
					case 5: break;          //星期的显示暂时不写，到时候来改
							DisplayWord1(2,2,2*8+2*16,TIME[5]+7);
					case 6: DisplayNumber(1,0,2*8,TIME[6]/16);
							DisplayNumber(1,0,3*8,TIME[6]%16);
							break;
				}
				if((TIME[SetPlace]&0x0f)>9){                       //保证BCD码
					TIME[SetPlace]=TIME[SetPlace]+6;
				}
				if((TIME[SetPlace]>=0x60)&&(SetPlace<2)){          //分秒只能到59
					TIME[SetPlace]=0;
				}
				if((TIME[SetPlace]>=0x24)&&(SetPlace==2)){         //小时只能到23
					TIME[SetPlace]=0;
				}
				if((TIME[SetPlace]>=0x32)&&(SetPlace==3)){         //日只能到31
					TIME[SetPlace]=0;
				}
				if((TIME[SetPlace]>=0x13)&&(SetPlace==4)){         //月只能到12
					TIME[SetPlace]=0;
				}
				if((TIME[SetPlace]>=0x7)&&(SetPlace==5)){          //周只能到7
					TIME[SetPlace]=1;
				}
				while((i<50)&&(K2==0)){                             //检测按键是否松开
					Delay10ms();
					i++;}
				i=0;}
		}
/*减功能*/
		if(K5==0){                       //检测按键K2是否按下
			Delay10ms();                              //消除抖动
			if(K5==0){
				TIME[SetPlace]=BCDToDex(TIME[SetPlace]);         //转换成十进制
				TIME[SetPlace]--;				              //进行减法操作
				TIME[SetPlace]=DexToBCD(TIME[SetPlace]);     //再转换成BCD码
				switch(SetPlace){
					case 0:	DisplayNumber(1,2,6*8,TIME[0]/16);
							DisplayNumber(1,2,7*8,TIME[0]%16);
							break;
					case 1:	DisplayNumber(1,2,3*8,TIME[1]/16);
							DisplayNumber(1,2,4*8,TIME[1]%16);
							break;
					case 2: DisplayNumber(1,2,0*8,TIME[2]/16);
							DisplayNumber(1,2,1*8,TIME[2]%16);
							break;
					case 3: DisplayNumber(2,0,1*16,TIME[3]/16);
							DisplayNumber(2,0,1*8+1*16,TIME[3]%16);
							break;
					case 4:	DisplayNumber(1,0,4*8+1*16,TIME[4]/16);
							DisplayNumber(1,0,5*8+1*16,TIME[4]%16);
							break;
					case 5: break;          //星期的显示暂时不写，到时候来改
							DisplayWord1(2,2,2*8+2*16,TIME[5]+7);
					case 6: DisplayNumber(1,0,2*8,TIME[6]/16);
							DisplayNumber(1,0,3*8,TIME[6]%16);
							break;
				}
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
				while((i<50)&&(K5==0)){                             //检测按键是否松开
					Delay10ms();
					i++;}
				i=0;}
		}
	
		/*设置AM_PM功能*/		
		if(SET_AM_PM==0){                       //检测按键K2是否按下
			Delay10ms();                        //消除抖动
			if(SET_AM_PM==0){
				if(AM_PM==1)AM_PM=0;
				else AM_PM=1;
				while((i<50)&&(K5==0)){                             //检测按键是否松开
					Delay10ms();
					i++;
				}
				i=0;
			}
		   	if(AM_PM==0){
				DisplayNumber(2,2,0*8,11);
			}
			else{
				DisplayNumber(2,2,0*8,12);
			}
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
		if(K1==0){                      //检测按键K1是否按下
			Delay10ms();                    //消除抖动
			if(K1==0){
				SetPlace++;
				if(SetPlace>=3)
				SetPlace=0;
			}
			while((i<50)&&(K1==0)){       //检测按键是否松开
				Delay10ms();
				i++;
			}
			i=0;
		}
		/*加功能*/
		if(K2==0){                       //检测按键K2是否按下
			Delay10ms();                  //消除抖动
			if(K2==0){
				alarm[SetPlace]++;
				switch(SetPlace){
					case 0:	DisplayNumber(1,2,6*8,alarm[0]/16);
							DisplayNumber(1,2,7*8,alarm[0]%16);
							break;
					case 1:	DisplayNumber(1,2,3*8,alarm[1]/16);
							DisplayNumber(1,2,4*8,alarm[1]%16);
							break;
					case 2: DisplayNumber(1,2,0*8,alarm[2]/16);
							DisplayNumber(1,2,1*8,alarm[2]%16);
							break;
				}
				if((alarm[SetPlace]&0x0f)>9){                       //保证BCD码
					alarm[SetPlace]=alarm[SetPlace]+6;
				}
				if((alarm[SetPlace]>=0x60)&&(SetPlace<2)){          //分秒只能到59
					alarm[SetPlace]=0;
				}
				if((alarm[SetPlace]>=0x24)&&(SetPlace==2)){         //小时只能到23
					alarm[SetPlace]=0;
				}
				while((i<50)&&(K2==0)){                             //检测按键是否松开
					Delay10ms();
					i++;}
				i=0;}
		}
				/*减功能*/
		if(K5==0){                       //检测按键K2是否按下
			Delay10ms();                  //消除抖动
			if(K5==0){	      
				alarm[SetPlace]=BCDToDex(alarm[SetPlace]);         //转换成十进制
				alarm[SetPlace]--;				              //进行减法操作
				alarm[SetPlace]=DexToBCD(alarm[SetPlace]);     //再转换成BCD码
				switch(SetPlace){
					case 0:	DisplayNumber(1,2,6*8,alarm[0]/16);
							DisplayNumber(1,2,7*8,alarm[0]%16);
							break;
					case 1:	DisplayNumber(1,2,3*8,alarm[1]/16);
							DisplayNumber(1,2,4*8,alarm[1]%16);
							break;
					case 2: DisplayNumber(1,2,0*8,alarm[2]/16);
							DisplayNumber(1,2,1*8,alarm[2]%16);
							break;
				}
				if((alarm[SetPlace]==0)&&(SetPlace<2)){          //分秒只能到59
					alarm[SetPlace]=0x60;
				}
				if((alarm[SetPlace]==0)&&(SetPlace==2)){         //小时只能到23
					alarm[SetPlace]=0x12;
				}
				while((i<50)&&(K5==0)){                             //检测按键是否松开
					Delay10ms();
					i++;}
				i=0;}
		}
	
		/*设置AM_PM功能*/		
		if(SET_AM_PM==0){                       //检测按键K2是否按下
			alarm_AM_PM=AM_PM;
			Delay10ms();                        //消除抖动
			if(SET_AM_PM==0){
				if(alarm_AM_PM==1)alarm_AM_PM=0;
				else alarm_AM_PM=1;
				while((i<50)&&(K5==0)){                             //检测按键是否松开
					Delay10ms();
					i++;
				}
				i=0;
			}
		   	if(alarm_AM_PM==0){
				DisplayNumber(2,2,0*8,11);
			}
			else{
				DisplayNumber(2,2,0*8,12);
			}
		}	

}