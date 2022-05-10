#include <reg52.h>
#include <intrins.h>
#include <DS1302.h>
#include <math.h>
#include <LCDNew.h>

#define uchar unsigned char 
#define uint unsigned int

uchar idata code1;
uchar idata code2;                          
uchar idata code3;//�����24����������
uchar idata code4;
uchar idata SolarTerm[24];
uchar idata lunar_month;
uchar idata lunar_date;
uint code temp[12]={0x0800,0x0400,0x0200,0x0100,0x0080,0x0040,0x0020,0x0010,0x0008,0x0004,0x0002,0x0001};
uint idata symbol;
uchar idata sign[13]={0};
/*����ƽ�������*/
uchar code Mon1[2][13]={0,31,28,31,30,31,30,31,31,30,31,30,31,
                        0,31,29,31,30,31,30,31,31,30,31,30,31};

///�����·����ݱ�
code uchar day_code1[9]={0x0,0x1f,0x3b,0x5a,0x78,0x97,0xb5,0xd4,0xf3};
code uint day_code2[3]={0x111,0x130,0x14e};
/*******************************************************************
�������Ӧ��ũ������,ÿ�����ֽ�,
��ʽ��һ�ֽ�BIT7-4 λ��ʾ�����·�,ֵΪ0 Ϊ������,BIT3-0 ��Ӧũ����1-4 �µĴ�С
�ڶ��ֽ�BIT7-0 ��Ӧũ����5-12 �´�С,�����ֽ�BIT7 ��ʾũ����13 ���´�С
�·ݶ�Ӧ��λΪ1 ��ʾ��ũ���´�(30 ��),Ϊ0 ��ʾС(29 ��).
�����ֽ�BIT6-5 ��ʾ���ڵĹ����·�,BIT4-0 ��ʾ���ڵĹ�������
ÿ������ݶ�Ӧ�������е�λ�ÿ� (200-(2100-year)-1)*3+Offset1
*******************************************************************/
uchar code yearData[]={         
0x0B,0x4A,0x43, //2011
0x4B,0x55,0x37, //2012
0x0A,0xd5,0x4A, //2013
0x95,0x5A,0xBf, //2014
0x04,0xBA,0x53, //2015
0x0A,0x5B,0x48, //2016
0x65,0x2B,0xBC, //2017
0x05,0x2B,0x50, //2018
0x0A,0x93,0x45, //2019
0x47,0x4A,0xB9, //2020
0x06,0xAA,0x4C, //2021
0x0A,0xd5,0x41, //2022
0x24,0xdA,0xB6, //2023
0x04,0xB6,0x4A, //2024
0x69,0x57,0x3d, //2025
0x0A,0x4e,0x51, //2026
0x0d,0x26,0x46, //2027
0x5e,0x93,0x3A, //2028
0x0d,0x53,0x4d, //2029
0x05,0xAA,0x43, //2030
0x36,0xB5,0x37, //2031
0x09,0x6d,0x4B, //2032
0xB4,0xAe,0xBf, //2033
0x04,0xAd,0x53, //2034
0x0A,0x4d,0x48, //2035
0x6d,0x25,0xBC, //2036
0x0d,0x25,0x4f, //2037
0x0d,0x52,0x44, //2038
0x5d,0xAA,0x38, //2039
0x0B,0x5A,0x4C, //2040
0x05,0x6d,0x41, //2041
0x24,0xAd,0xB6, //2042
0x04,0x9B,0x4A, //2043
0x7A,0x4B,0xBe, //2044
0x0A,0x4B,0x51, //2045
0x0A,0xA5,0x46, //2046
0x5B,0x52,0xBA, //2047
0x06,0xd2,0x4e, //2048
0x0A,0xdA,0x42, //2049
0x35,0x5B,0x37, //2050
};

/*�ṹ�����ͣ���Ŵ��ڵ���������*/
typedef struct spdate	         
{
        uchar cYear;
        uchar cMon;
        uchar cDay;
}SPDATE;

SPDATE idata spDate;       //ʵ����

uchar convertBCDtoHex(uchar BCD){
	uchar temp1,temp2;
	temp1=BCD/16;
	temp2=BCD%16;
	return temp1*10+temp2;	
}

uchar HexDoDex(uchar hex){						      //��λ16������ת10����
	uchar temp1,temp2;
	temp1=hex/16;
	temp2=hex%16;
	return temp1*16+temp2;
}


/*******************************************************************
�������ƣ�calcSolarTerm()
�������ܣ����뵱���������ݺ���λ������24�������ڵ�����
���������year ������ݵĺ���λ��10������ݣ� 
����ֵ�� �ޣ��������ȫ�ֱ���
*******************************************************************/
void calcSolarTermInit(uchar year){
	uint i;
	uint l;                             //l����2000�꿪ʼ��������
	//uint d=0.2422;
	uint c;
	l=(int)year/4;
	if((year%100!=0 && year%4==0)||(year%100==0 && year%400==0)){l=l-1;}                                           
	for(i=0;i<24;i++){
		 switch(i){
		 	case 0:c=5.4055;break;	    //1�� С��
			case 1:c=20.12;break;	    //1�� ��
		 	case 2:c=3.87;break;        //2�� ����
			case 3:c=18.73;break;		//2�� ��ˮ
			case 4:c=5.63;break;	    //3�� ����
			case 5:c=20.646;break;
			case 6:c=4.81;break;
			case 7:c=20.1;break;
			case 8:c=5.52;break;
			case 9:c=21.04;break;
			case 10:c=5.678;break;
			case 11:c=21.37;break;
			case 12:c=7.108;break;
			case 13:c=22.83;break;
			case 14:c=7.5;break;
			case 15:c=23.13;break;
			case 16:c=7.646;break;
			case 17:c=23.042;break;
			case 18:c=8.318;break;
			case 19:c=23.438;break;
			case 20:c=7.438;break;
			case 21:c=22.36;break;
			case 22:c=7.18;break;
			case 23:c=21.94;break;
		 }	            
		SolarTerm[i]=(int)(year*0.2422+c-l);	//һ��Ҫ��0.2422����d������û���ã�
		}
		/*�Լ�������������*/
		if(year==26){SolarTerm[1]=SolarTerm[1]-1;}
		if(year==84){SolarTerm[3]=SolarTerm[3]+1;}
		if(year==8){SolarTerm[7]=SolarTerm[7]+1;}
		if(year==16){SolarTerm[10]=SolarTerm[10]+1;}
		if(year==2){SolarTerm[12]=SolarTerm[12]+1;}
		if(year==89){SolarTerm[17]=SolarTerm[17]+1;SolarTerm[18]=SolarTerm[18]+1;}
		if(year==21){SolarTerm[21]=SolarTerm[21]-1;}
		if(year==19){SolarTerm[22]=SolarTerm[22]-1;}
		if(year==82){SolarTerm[23]=SolarTerm[23]+1;}
}
/*******************************************************************
�������ƣ�convert()
�������ܣ���������BCD�룬�������BCD�롣
���������year �����·ݵ��� month �����·ݵ�BCD�� date �������ڵ�BCD�� 
����ֵ�� �ޣ��������ȫ�ֱ���
*******************************************************************/


void convert(uchar year,uchar month,uchar date){
	uchar dex_year;
	uchar temp1;
	uchar month_p;
	uchar interval_1;
	uint interval_2;
	uchar flag_y;
	uchar flag2;
	uchar leap_month;
	uchar flag=0;
	bit extraBit;
	uchar i;
	uint dex_wholeYear;	       //2000�� DEX              
	//��BCD��ת����16����
	year=convertBCDtoHex(year);
	month=convertBCDtoHex(month);
	date=convertBCDtoHex(date);

	dex_wholeYear=2000+HexDoDex(year);	       //10����4λ�����
	dex_year=HexDoDex(year);



	//����24����                         
	calcSolarTermInit(year);	       //����������24����
	for(i=0;i<24;i++){			       
		if((month==(i/2+1))&&(date==SolarTerm[i])){
			code3=i;
			break;						 //�����ǵ�code3������                         	
		}
	}
	if(i==23)code3=0xff;

	
	/*����������ת��*/
	//��ô��ڵ���������
	spDate.cYear=dex_year;
    spDate.cMon=(yearData[dex_year*3+2]&0x60)>>5; //���㴺�ڹ����·ݣ���ȷ��
    spDate.cDay=(yearData[dex_year*3+2])&0x1f;    //���㴺�ڹ�������

	//�ж�ƽ������
	if((dex_year%100!=0 && dex_year%4==0) || (dex_year%100==0 && dex_year%400==0)){
		flag=1;       //��ʾ������
	}
	else{
		flag=0;       //��ʾ��ƽ��
	}

	//��ý��������
	leap_month=(yearData[dex_year*3]&0xf0)>>4;
	symbol=0;
	symbol=((yearData[dex_year*3]&0x0f)<<8)|yearData[dex_year*3+1];   //(������ȷ)ũ��1-12�´�С����
	extraBit=(yearData[dex_year*3]&0x80)>>7;         //�����ũ����13���µĴ�С����
	//sign[0]=1;
	
	for(i=0;i<12;i++){	                     //(��ȷ)����sign[i]�д����ũ��1-12�´�С������
		if(temp[i]&symbol) {sign[i]=1;}
		else{sign[i]=0;}
	}
	sign[12]=extraBit;

    // ���㵱�괺���뵱��Ԫ��������,����ֻ���ڹ���1�»�2��
    if(spDate.cMon==0x1){  
        interval_1=spDate.cDay-1;  
    }  
    else{
        interval_1=spDate.cDay+0x1f-1;     //interval_1�Ǵ��ھ���Ԫ��������   
    }

	//���㹫�����뵱��Ԫ��������,Ϊ�˼�������,����������
    //day_code1[9],day_code2[3]
    //����������ھ��»�ǰ,����������0xff,�ñ�day_code1[9],
    //�ھ��º�,��������0xff,�ñ�day_code2[3]
    //�����빫����Ϊ8��10��,��������Ԫ������Ϊday_code1[8-1]+10-1
    //�����빫����Ϊ11��10��,��������Ԫ������Ϊday_code2[11-10]+10-1
    if (month<10){ 
        interval_2=day_code1[month-1]+date-1;
    }
    else{
        interval_2=day_code2[month-10]+date-1;
    }
    if ((month>0x2)&&(year%0x4==0)){  //��������´���2�²��Ҹ����2��Ϊ����,������1
        interval_2+=1;				  //interval_2�ǹ������ھ���Ԫ��������
    }
    //���㹫�����뵱��Ԫ�����������
	
	//�жϹ������ڴ���ǰ���Ǵ��ں�
    if (interval_2>=interval_1){ //�������ڴ��ں����Ǵ��ڵ���ʹ����������������
        interval_2-=interval_1;
        month=0x1;
        month_p=0x1;  //month_pΪ�·�ָ��,�������ڴ���ǰ����Ǵ��ڵ���month_pָ������
        flag2=sign[month_p-1]; //����ũ����Ϊ��С����С��,���·���1,С�·���0
        flag_y=0;
        if(flag2==0)temp1=0x1d; //С��29��
        else temp1=0x1e; //����30��
        while(interval_2>=temp1){
            interval_2-=temp1;
            month_p+=1;
            if(month==leap_month){
            	flag_y=~flag_y;
            	if(flag_y==0)month+=1;
            }
            else month+=1;
            flag2=sign[month_p];
            if(flag2==0)temp1=0x1d;
            else temp1=0x1e;
        }
        lunar_date=interval_2+1;
    }
    else{  //�������ڴ���ǰʹ����������������
		//��ʼ����ȥ������
		year=year-1;
		//�������
		leap_month=(yearData[year*3]&0xf0)>>4;
        interval_1-=interval_2;
        month=0xc;   //�·ݴ�12��ʼ
        if (extraBit==0)
			month_p=0xc; 
        else
			month_p=0xd; //
       	 //month_pΪ�·�ָ��,�������������,һ����ʮ������,��ָ��13,������ָ��12
        
        flag_y=0;
        flag2=sign[month_p-1];
        if(flag2==0)temp1=0x1d;	   //temp1����ũ����С�·�
        else temp1=0x1e;
        while(interval_1>temp1){
            interval_1-=temp1;
            month_p-=1;
            if(flag_y==0)month-=1;
            if(month==leap_month)flag_y=~flag_y;
            flag2=sign[month_p-1];
            if(flag2==0)temp1=0x1d;
            else temp1=0x1e;
         }
        lunar_date=temp1-interval_1+1;
	}
	lunar_month=month;
}
