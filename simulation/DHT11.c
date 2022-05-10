#include<reg51.h>
#include<intrins.h>
#include <LCD.h>

#define ERROR 0
#define OK 1
#define uint unsigned int
#define uchar unsigned char 

sbit Bus=P1^3;//��������

uchar  idata value[5];    
uint idata check;			 //������ֵ 
uchar idata temprature,hum;
//***************************************
// ģ�鹦�ܣ���ȡDHT11������
// ��ģ��Ϊ��ʪ�ȴ����� �������������źź�
// ��ģ�����������5���ֽڵ����� ����ʱԼ4MS
// ��һ�����ֽ�Ϊʪ�����ݵ�������С������
// �����ĸ��ֽ�Ϊ�¶����ݵ�������С������
// ������ֽ�ΪУ���� ����ǰ�ĸ�������Ӻ�ĺ�8λ����ֵ
// �����δ������δ���Ӽ�� �������ݳ�ʱ����

//***************************************
 
void Delay10us(void);
void Delay20ms(void);
void Delay1s(void);
uchar Read_Temp();

uchar Read_Temp()	            
{
	uchar i,j=0,mask;
	Bus=0;                    //�����������Ͳ�����20ms������18ms��
	Delay20ms();
	Bus=1;                    //��������������40us��20-40us��
	Delay10us();
	Delay10us();
	Delay10us();
	Delay10us();
	if(Bus==1)//�������û����Ӧ
	{
		return ERROR;
	}
	else //�����������Ӧ
	{
		while(!Bus);  //�ȴ����ߵĵ���Ӧ(80us)
		while(Bus);   //�ȴ����ߵĸ���Ӧ��80us��
		while(j<5)
		{
			mask=0x80;	  //1000 0000 �����λ��ʼ
			for(i=0;i<8;i++)            //����һ���ֽ�
			{
				while(!Bus);            //�ȴ���һ���͵�ƽ��϶
				Delay10us();            //0����26-28us�ĸߵ�ƽ��1����70us�ĸߵ�ƽ��ȡ40us�Ĵ��ĵ�ƽ���
				Delay10us();
				Delay10us();
				Delay10us();
				if(Bus==0)	            //˵��������0
				{
					value[j]&=(~mask);	//��λ��0
				}
				else
				{
					value[j]|=mask;		//��λ��1
				}
				mask>>=1;				//mask����һλ
				while(Bus);             //�ȴ���λ���ͽ���
			}
			j++;//����������һ���ֽ�
		}
			
	}
	check=(value[0]+value[2])&0x00ff;  //����У��� С��λ��0
	if(check==value[4])//���У���û��
	{
		return OK;
	}
	else
	{
		return ERROR;
	}
 
 
}


/*10us��ʱ�Ӻ���*/
void Delay10us()
{
	uchar i;
	i=2;
	while(--i);
}
 
 
/*20ms��ʱ�Ӻ���*/
void Delay20ms(void)   //��� -0.000000000005us
{
    uchar a,b,c;
    for(c=1;c>0;c--)
        for(b=222;b>0;b--)
            for(a=40;a>0;a--);
}  
 
/*1s��ʱ�Ӻ���*/
void Delay1s(void)   //��� -0.000000000227us
{
    uchar a,b,c;
    for(c=13;c>0;c--)
        for(b=247;b>0;b--)
            for(a=142;a>0;a--);
    _nop_(); 
}


void DisplayTemp()
{
	DisplayNumber(2,6,1*16+2*8,temprature/10);
	DisplayNumber(2,6,1*16+3*8,temprature%10);
	DisplayNumber(2,6,1*16+4*8,hum/10);
	DisplayNumber(2,6,1*16+5*8,hum%10);

}

void DHT11_Init(){
	Delay1s();//Խ��1s���ȶ���	
}
void DHT11(){
	uchar result;
	result=Read_Temp();
	if(result==OK)
	{
		temprature=value[2];   //�¶�
		hum=value[0];    //ʪ��
	}
}