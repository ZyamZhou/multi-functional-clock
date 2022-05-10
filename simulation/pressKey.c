#include <reg52.h>
#include <intrins.h>
#include <DS1302.h>
#include <LCD.h>
#define uint unsigned int
#define uchar unsigned char 

sbit K1=P3^0;  		     //ѡ��Ҫ�ı��ʱ�����
sbit K2=P3^1;  		     //����ʱ������� �Ӱ���
sbit K5=P1^6;			 //������
sbit SET_AM_PM=P1^7;
sbit beep=P1^5;

uchar idata SetAlarm;               //������������
uchar idata SetState;               //����ѡ��Ҫ���������� ���ʱ��������
uchar idata SetPlace;               //����ѡ��Ҫ����������
uchar idata alarm[3];               //����װ��������
uchar idata alarm_AM_PM;

uchar BCDToDex(uchar num){
	return (num/16)*10+(num%16);	
}

uchar DexToBCD(uchar num){
	return (num/10)*16+(num%10);
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

/******************************************************************************
��������Delay10ms
�������ܣ���ʱ10ms
���룺 ��
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
		if(K1==0){                      //��ⰴ��K1�Ƿ���
			Delay10ms();                    //��������
			if(K1==0){
				SetPlace++;
				if(SetPlace>=7)
				SetPlace=0;
			}
			while((i<50)&&(K1==0)){       //��ⰴ���Ƿ��ɿ�
				Delay10ms();
				i++;
			}
			i=0;
		}
		/*�ӹ���*/
		if(K2==0){                       //��ⰴ��K2�Ƿ���
			Delay10ms();                  //��������
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
					case 5: break;          //���ڵ���ʾ��ʱ��д����ʱ������
							DisplayWord1(2,2,2*8+2*16,TIME[5]+7);
					case 6: DisplayNumber(1,0,2*8,TIME[6]/16);
							DisplayNumber(1,0,3*8,TIME[6]%16);
							break;
				}
				if((TIME[SetPlace]&0x0f)>9){                       //��֤BCD��
					TIME[SetPlace]=TIME[SetPlace]+6;
				}
				if((TIME[SetPlace]>=0x60)&&(SetPlace<2)){          //����ֻ�ܵ�59
					TIME[SetPlace]=0;
				}
				if((TIME[SetPlace]>=0x24)&&(SetPlace==2)){         //Сʱֻ�ܵ�23
					TIME[SetPlace]=0;
				}
				if((TIME[SetPlace]>=0x32)&&(SetPlace==3)){         //��ֻ�ܵ�31
					TIME[SetPlace]=0;
				}
				if((TIME[SetPlace]>=0x13)&&(SetPlace==4)){         //��ֻ�ܵ�12
					TIME[SetPlace]=0;
				}
				if((TIME[SetPlace]>=0x7)&&(SetPlace==5)){          //��ֻ�ܵ�7
					TIME[SetPlace]=1;
				}
				while((i<50)&&(K2==0)){                             //��ⰴ���Ƿ��ɿ�
					Delay10ms();
					i++;}
				i=0;}
		}
/*������*/
		if(K5==0){                       //��ⰴ��K2�Ƿ���
			Delay10ms();                              //��������
			if(K5==0){
				TIME[SetPlace]=BCDToDex(TIME[SetPlace]);         //ת����ʮ����
				TIME[SetPlace]--;				              //���м�������
				TIME[SetPlace]=DexToBCD(TIME[SetPlace]);     //��ת����BCD��
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
					case 5: break;          //���ڵ���ʾ��ʱ��д����ʱ������
							DisplayWord1(2,2,2*8+2*16,TIME[5]+7);
					case 6: DisplayNumber(1,0,2*8,TIME[6]/16);
							DisplayNumber(1,0,3*8,TIME[6]%16);
							break;
				}
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
				while((i<50)&&(K5==0)){                             //��ⰴ���Ƿ��ɿ�
					Delay10ms();
					i++;}
				i=0;}
		}
	
		/*����AM_PM����*/		
		if(SET_AM_PM==0){                       //��ⰴ��K2�Ƿ���
			Delay10ms();                        //��������
			if(SET_AM_PM==0){
				if(AM_PM==1)AM_PM=0;
				else AM_PM=1;
				while((i<50)&&(K5==0)){                             //��ⰴ���Ƿ��ɿ�
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
��������scanButton_alarm()
�������ܣ������������õļ���ɨ�躯��
���룺 
�����
*******************************************************************************/
void scanButton_alarm(){
	uchar i=0;
		if(K1==0){                      //��ⰴ��K1�Ƿ���
			Delay10ms();                    //��������
			if(K1==0){
				SetPlace++;
				if(SetPlace>=3)
				SetPlace=0;
			}
			while((i<50)&&(K1==0)){       //��ⰴ���Ƿ��ɿ�
				Delay10ms();
				i++;
			}
			i=0;
		}
		/*�ӹ���*/
		if(K2==0){                       //��ⰴ��K2�Ƿ���
			Delay10ms();                  //��������
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
				if((alarm[SetPlace]&0x0f)>9){                       //��֤BCD��
					alarm[SetPlace]=alarm[SetPlace]+6;
				}
				if((alarm[SetPlace]>=0x60)&&(SetPlace<2)){          //����ֻ�ܵ�59
					alarm[SetPlace]=0;
				}
				if((alarm[SetPlace]>=0x24)&&(SetPlace==2)){         //Сʱֻ�ܵ�23
					alarm[SetPlace]=0;
				}
				while((i<50)&&(K2==0)){                             //��ⰴ���Ƿ��ɿ�
					Delay10ms();
					i++;}
				i=0;}
		}
				/*������*/
		if(K5==0){                       //��ⰴ��K2�Ƿ���
			Delay10ms();                  //��������
			if(K5==0){	      
				alarm[SetPlace]=BCDToDex(alarm[SetPlace]);         //ת����ʮ����
				alarm[SetPlace]--;				              //���м�������
				alarm[SetPlace]=DexToBCD(alarm[SetPlace]);     //��ת����BCD��
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
				if((alarm[SetPlace]==0)&&(SetPlace<2)){          //����ֻ�ܵ�59
					alarm[SetPlace]=0x60;
				}
				if((alarm[SetPlace]==0)&&(SetPlace==2)){         //Сʱֻ�ܵ�23
					alarm[SetPlace]=0x12;
				}
				while((i<50)&&(K5==0)){                             //��ⰴ���Ƿ��ɿ�
					Delay10ms();
					i++;}
				i=0;}
		}
	
		/*����AM_PM����*/		
		if(SET_AM_PM==0){                       //��ⰴ��K2�Ƿ���
			alarm_AM_PM=AM_PM;
			Delay10ms();                        //��������
			if(SET_AM_PM==0){
				if(alarm_AM_PM==1)alarm_AM_PM=0;
				else alarm_AM_PM=1;
				while((i<50)&&(K5==0)){                             //��ⰴ���Ƿ��ɿ�
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