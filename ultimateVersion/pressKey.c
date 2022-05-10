#include <reg52.h>
#include <intrins.h>
#include <DS1302.h>
#include <LCDNew.h>
#include <map.h>
#include <cellPhone.h>
#define uint unsigned int
#define uchar unsigned char 
							 
sbit K1=P1^2;  		     //ѡ��Ҫ�ı��ʱ�����	   SW1
sbit K2=P1^3;  		     //����ʱ������� �Ӱ���     SW2
sbit K5=P1^4;			 //������	SW4
sbit SET_AM_PM=P1^6;     //SW6
sbit beep=P1^5;          //BEEP��        

uchar idata SetAlarm;               //������������
uchar idata SetState;               //����ѡ��Ҫ���������� ���ʱ��������
uchar idata SetPlace;               //����ѡ��Ҫ����������
uchar idata alarm[3]={0};           //����װ��������     ��ʼ��Ϊ0
uchar idata alarm_AM_PM;		    //Ĭ�ϳ�ʼ��ΪAM        

uchar BCDToDex(uchar num);
uchar DexToBCD(uchar num);

void DisplayButton(SetPlace){
				switch(SetPlace){
					case 0:	LCDSetPos(1,3);
							LCD12864_WriteData(StrNUMBER[TIME[0]/16]);	    //��
							if(TIME[0]%16==0)LCD12864_WriteData('0');
							else LCD12864_WriteData(StrNUMBER[TIME[0]%16]);
							break;
					case 1:	LCDSetPos(1,1);
							LCD12864_WriteData(':');
							LCD12864_WriteData(StrNUMBER[TIME[1]/16]);	    //��
							LCD12864_WriteData(StrNUMBER[TIME[1]%16]);
							LCD12864_WriteData(':');
							break;
					case 2: LCDSetPos(1,0);
							LCD12864_WriteData(StrNUMBER[TIME[2]/16]);	    //ʱ
							LCD12864_WriteData(StrNUMBER[TIME[2]%16]);         
							break;
					case 3: LCDSetPos(0,5);                                 //��
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
					case 4:	LCDSetPos(0,3);                                 //��
							LCD12864_WriteData(StrNUMBER[TIME[4]/16]);	    
							LCD12864_WriteData(StrNUMBER[TIME[4]%16]);
							break;
					case 5: LCDSetPos(1,7);
							DisplayWord(MAPNumber(TIME[5]%16));
							break;          //���ڵ���ʾ��ʱ��д����ʱ������
					case 6: LCDSetPos(0,1);                                 //��
							LCD12864_WriteData(StrNUMBER[TIME[6]/16]);	    
							LCD12864_WriteData(StrNUMBER[TIME[6]%16]);
							break;
				}
	
}
/******************************************************************************
��������Delay10ms
�������ܣ���ʱ10ms
���룺��
�������
*******************************************************************************/
void Delay10ms(){                                   //���0us
	uchar a,b,c;
	for(c=1;c>0;c--)
		for(b=38;b>0;b--)
			for(a=130;a>0;a--);}



/******************************************************************************
��������scanButton()
�������ܣ�����ɨ�躯��
���룺 
�����
*******************************************************************************/
void scanButton(){
	uchar i=0;
		if(K1==0 || flag11==5){                      //��ⰴ��K1�Ƿ���
			Delay10ms();                    //��������
			if(K1==0|| flag11==5){
				SetPlace++;
				if(SetPlace>=7)
				SetPlace=0;
			}
			while((i<50)&&(K1==0)){       //��ⰴ���Ƿ��ɿ�
				Delay10ms();
				i++;
			}
			i=0;
			if(flag11==5) flag11=0xff;
		}
		/*�ӹ���*/
		if(K2==0|| flag11==7){                         //��ⰴ��K2�Ƿ���
			Delay10ms();                               //��������
			if(K2==0|| flag11==7){
				TIME[SetPlace]++;
				if(SetPlace==3){
					if(TIME[5]>=0x07) TIME[5]=1;
					else TIME[5]++;
				}
				if((TIME[SetPlace]&0x0f)>9){                       //��֤BCD��
					TIME[SetPlace]=TIME[SetPlace]+6;
				}
				if((TIME[SetPlace]>=0x60)&&(SetPlace<2)){          //����ֻ�ܵ�59
					TIME[SetPlace]=0;
				}
				if((TIME[SetPlace]>=0x13)&&(SetPlace==2)){         //Сʱֻ�ܵ�23
					TIME[SetPlace]=0;
				}
				if((TIME[SetPlace]>=0x32)&&(SetPlace==3)){         //��ֻ�ܵ�31
					TIME[SetPlace]=0;
				}
				if((TIME[SetPlace]>=0x13)&&(SetPlace==4)){         //��ֻ�ܵ�12
					TIME[SetPlace]=0;
				}
				if((TIME[SetPlace]>=0x8)&&(SetPlace==5)){          //��ֻ�ܵ�7
					TIME[SetPlace]=1;
				}
				DisplayButton(SetPlace);
				while((i<50)&&(K2==0)){                             //��ⰴ���Ƿ��ɿ�
					Delay10ms();
					i++;}
				i=0;}
				if(flag11==7) flag11=0xff;
		}
		
/*������*/
		if(K5==0 || flag11==8){                       //��ⰴ��K2�Ƿ���
			Delay10ms();                              //��������
			if(K5==0 || flag11==8){
				TIME[SetPlace]=BCDToDex(TIME[SetPlace]);         //ת����ʮ����
				TIME[SetPlace]--;				              //���м�������
				TIME[SetPlace]=DexToBCD(TIME[SetPlace]);     //��ת����BCD��
				if(SetPlace==3){
					if(TIME[5]==1) TIME[5]=7;
					else TIME[5]--;
				}
				if((TIME[SetPlace]==0)&&(SetPlace<2)){        //����ֻ�ܵ�59
					TIME[SetPlace]=0x60;
				}
				if((TIME[SetPlace]==0)&&(SetPlace==2)){       //Сʱֻ�ܵ�12
					TIME[SetPlace]=0x12;
				}
				if((TIME[SetPlace]==0)&&(SetPlace==3)){       //��ֻ�ܵ�31
					TIME[SetPlace]=0x31;
				}
				if((TIME[SetPlace]==0)&&(SetPlace==4)){       //��ֻ�ܵ�12
					TIME[SetPlace]=0x12;
				}
				if((TIME[SetPlace]==0)&&(SetPlace==5)){          //��ֻ�ܵ�7
					TIME[SetPlace]=0x07;
				}
				DisplayButton(SetPlace);
				while((i<50)&&(K5==0)){                             //��ⰴ���Ƿ��ɿ�
					Delay10ms();
					i++;}
				i=0;}
				if(flag11==8) flag11=0xff;
		}
	
		/*����AM_PM����*/		
		if(SET_AM_PM==0 || flag11==9){                       //��ⰴ��K2�Ƿ���
			Delay10ms();                        //��������
			if(SET_AM_PM==0|| flag11==9){		            //ȷ�ϰ���ȷʵ����
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
				while((i<50)&&(SET_AM_PM==0)){                             //��ⰴ���Ƿ��ɿ�
					Delay10ms();
					i++;
				}
				i=0;
			}
			if(flag11==9) flag11=0xff;
		}	

}
/*************************���Ӱ�����ʾ����**************************************/
void DisplayAlarmButton(SetPlace){
				switch(SetPlace){
					case 0:	LCDSetPos(1,5);
							LCD12864_WriteData(StrNUMBER[alarm[0]/16]);	    //��
							LCD12864_WriteData(StrNUMBER[alarm[0]%16]);
							break;
					case 1:	LCDSetPos(1,3);
							LCD12864_WriteData(':');
							LCD12864_WriteData(StrNUMBER[alarm[1]/16]);	    //��
							LCD12864_WriteData(StrNUMBER[alarm[1]%16]);
							LCD12864_WriteData(':');
							break;
					case 2: LCDSetPos(1,2);
							LCD12864_WriteData(StrNUMBER[alarm[2]/16]);	    //ʱ
							LCD12864_WriteData(StrNUMBER[alarm[2]%16]);         
							break;
				}		
}

/******************************************************************************
��������scanButton_alarm()
�������ܣ������������õļ���ɨ�躯��
���룺 
�����
*******************************************************************************/
void scanButton_alarm(){
	uchar i=0;
		if(K1==0 || flag11==5){                      //��ⰴ��K1�Ƿ���
			Delay10ms();                    //��������
			if(K1==0|| flag11==5){
				SetPlace++;
				if(SetPlace>=3)
				SetPlace=0;
			}
			while((i<50)&&(K1==0)){       //��ⰴ���Ƿ��ɿ�
				Delay10ms();
				i++;
			}
			i=0;
			if(flag11==5)flag11=0xff;
		}
		/*�ӹ���*/
		if(K2==0 || flag11==7){                       //��ⰴ��K2�Ƿ���
			Delay10ms();                  //��������
			if(K2==0|| flag11==7){
				alarm[SetPlace]++;
				if((alarm[SetPlace]&0x0f)>9){                       //��֤BCD��
					alarm[SetPlace]=alarm[SetPlace]+6;
				}
				if((alarm[SetPlace]>=0x60)&&(SetPlace<2)){          //����ֻ�ܵ�59
					alarm[SetPlace]=0;
				}
				if((alarm[SetPlace]>=0x13)&&(SetPlace==2)){         //Сʱֻ�ܵ�12
					alarm[SetPlace]=0;
				}
				DisplayAlarmButton(SetPlace);
				while((i<50)&&(K2==0)){                             //��ⰴ���Ƿ��ɿ�
					Delay10ms();
					i++;}
				i=0;}
				if(flag11==7) flag11=0xff;
		}
				/*������*/
		if(K5==0|| flag11==8){                       //��ⰴ��K2�Ƿ���
			Delay10ms();                  //��������
			if(K5==0 || flag11==8){
				alarm[SetPlace]=BCDToDex(alarm[SetPlace]);         //ת����ʮ����
				alarm[SetPlace]--;				                   //���м�������
				alarm[SetPlace]=DexToBCD(alarm[SetPlace]);         //��ת����BCD��
				if((alarm[SetPlace]==0)&&(SetPlace<2)){          //����ֻ�ܵ�59
					alarm[SetPlace]=0x60;
				}
				if((alarm[SetPlace]==0)&&(SetPlace==2)){         //Сʱֻ�ܵ�23
					alarm[SetPlace]=0x12;
				}
				DisplayAlarmButton(SetPlace);
				while((i<50)&&(K5==0)){                             //��ⰴ���Ƿ��ɿ�
					Delay10ms();
					i++;}
				i=0;}
				if(flag11==8) flag11=0xff;
		}
	
		/*����AM_PM����*/		
		if(SET_AM_PM==0 || flag11==9){                       //��ⰴ��K2�Ƿ���
			Delay10ms();                        //��������
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
				while((i<50)&&(SET_AM_PM==0)){                             //��ⰴ���Ƿ��ɿ�
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


