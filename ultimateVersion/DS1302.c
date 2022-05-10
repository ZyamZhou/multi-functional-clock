#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char 
#define uint unsigned int

uchar year;
uchar hour;
uchar day;
uchar month;
uchar date;
uchar minute;
uchar second;
uchar AM_PM;
//DS1302д���ַ����,˳��Ϊ���ʱ�������꣬���λ��дλ����ȡ�ĵ�ַ��д��Ļ����ϼ�1
uchar code WRITE_RTC_ADDR[7] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};
uchar year0=0x20;//���ͷ��λ
//DS1302ʱ�ӳ�ʼ��2020��8��4�����ڶ�12��00��00�롣�洢˳�������ʱ��������,�洢��ʽ����BCD��
uchar TIME[7] = {0x30, 0x59, 0x04, 0x21, 0x06, 0x02, 0x21};

sbit RST=P2^7;
sbit SCLK=P2^5;
sbit DSIO=P2^6;
/*******************************************************************
�������ƣ�Ds1302Write()
�������ܣ���DS1302�����ַ+���ݣ���
���������addr��д�����ݵĵ�ַ  dat��д�������  
����ֵ��  ��
*******************************************************************/
void Ds1302Write(uchar addr, uchar dat)
{
	uchar n;
	RST = 0;
	_nop_();
	SCLK = 0;		//�Ƚ�SCLK�õ͵�ƽ��
	_nop_();
	RST = 1; 		//Ȼ��RST(CE)�øߵ�ƽ��
	_nop_();
	for (n=0; n<8; n++)		//��ʼ���Ͱ�λ��ַ����
	{
		DSIO = addr & 0x01;		//���ݴӵ�λ��ʼ����
		addr >>= 1;
		SCLK = 1;		//������������ʱ��DS1302��ȡ����
		_nop_();
		SCLK = 0;
		_nop_();
	}
	for (n=0; n<8; n++)		//д��8λ����
	{
		DSIO = dat & 0x01;
		dat >>= 1;
		SCLK = 1;		//������������ʱ��DS1302��ȡ����  q
		_nop_();
		SCLK = 0;
		_nop_();	
	}			 
	RST = 0;		//�������ݽ���
	_nop_();
} 

/*******************************************************************
�������ƣ�Ds1302Read()
�������ܣ���ȡһ����ַ�����ݡ�
���������addr����Ҫ��ȡ���ݵĵ�ַ   
����ֵ��  dat������������ 
*******************************************************************/
uchar Ds1302Read(uchar addr)
{
	uchar n,dat,dat1;
	RST = 0;
	_nop_();

	SCLK = 0;		//�Ƚ�SCLK�õ͵�ƽ��
	_nop_();

	RST = 1;		//Ȼ��RST(CE)�øߵ�ƽ��
	_nop_();
	for(n=0; n<8; n++)		//��ʼ���Ͱ�λ��ַ����
	{
		DSIO = addr & 0x01;		//���ݴӵ�λ��ʼ����
		addr >>= 1;
		SCLK = 1;		//������������ʱ��DS1302��ȡ����
		_nop_();
		SCLK = 0;		//DS1302�½���ʱ����������
		_nop_();
	}
	_nop_();
	for(n=0; n<8; n++)		//��ȡ8λ����
	{
		dat1 = DSIO;		//�����λ��ʼ����
		dat = (dat>>1) | (dat1<<7);
		SCLK = 1;
		_nop_();
		SCLK = 0;		//DS1302�½���ʱ����������
		_nop_();
	}
	RST = 0;
	_nop_();		//DS1302��λ���ȶ�ʱ�䡣
	SCLK = 1;
	_nop_();
	DSIO = 0;
	_nop_();
	DSIO = 1;
	_nop_();
	return dat;	
}

/*******************************************************************
�������ƣ�Ds1302Init()
�������ܣ���ʼ��DS1302��
�����������   
����ֵ��  �� 
*******************************************************************/
void Ds1302Init()
{
	uchar n;
	Ds1302Write(0x8E,0X00);		 //�ر�д��������
	for (n=0; n<7; n++)		     //д��7���ֽڵ�ʱ���źţ�����ʱ��������
	{
		Ds1302Write(WRITE_RTC_ADDR[n],TIME[n]); 	
	}
	Ds1302Write(WRITE_RTC_ADDR[2],TIME[2]|0x20);           //����AM��PM
	Ds1302Write(0x8E,0x80);		 //��д��������
}

/*******************************************************************
�������ƣ�Ds1302ReadTime()
�������ܣ���ȡʱ�ӵ����ڡ�ʱ�����ݡ�
�����������   
����ֵ��  �� 
*******************************************************************/
void Ds1302ReadTime()
{
	TIME[6]=Ds1302Read(WRITE_RTC_ADDR[6]|0x01);
	TIME[5]=Ds1302Read(WRITE_RTC_ADDR[5]|0x01);
	TIME[5]=TIME[5]&0x07;
	TIME[4]=Ds1302Read(WRITE_RTC_ADDR[4]|0x01);
	TIME[3]=Ds1302Read(WRITE_RTC_ADDR[3]|0x01);
	TIME[2]=Ds1302Read(WRITE_RTC_ADDR[2]|0x01);
	TIME[2] =TIME[2] & 0x1f;      
	TIME[1]=Ds1302Read(WRITE_RTC_ADDR[1]|0x01);
	TIME[0]=Ds1302Read(WRITE_RTC_ADDR[0]|0x01);
	//����TIME[]��������ĳ�ʼ������
	second=TIME[0];
	minute=TIME[1];
	hour=TIME[2];
	date=TIME[3];
	month=TIME[4];
	day=TIME[5];
	year=TIME[6];

}

