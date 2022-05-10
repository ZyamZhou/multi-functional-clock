#include "reg52.h"
#include "intrins.h"

#define uint unsigned int
#define uchar unsigned char

uchar code StrNUMBER[]={"0123456789"};
uchar code NUMBER[]={"0123456789"};
uchar code *word0="С��";
uchar code *word1="��";
uchar code *word2="����";
uchar code *word3="��ˮ";
uchar code *word4="����";
uchar code *word5="����";
uchar code *word6="����";
uchar code *word7="����";
uchar code *word8="����";
uchar code *word9="С��";
uchar code *word10="â��";
uchar code *word11="����";
uchar code *word12="С��";
uchar code *word13="����";
uchar code *word14="����";
uchar code *word15="����";
uchar code *word16="��¶";
uchar code *word17="���";
uchar code *word18="��¶";
uchar code *word19="˪��";
uchar code *word20="����";
uchar code *word21="Сѩ";
uchar code *word22="��ѩ";
uchar code *word23="����";

uchar code *Cnumber1="һ";
uchar code *Cnumber2="��";
uchar code *Cnumber3="��\xfd";
uchar code *Cnumber4="��";
uchar code *Cnumber5="��";
uchar code *Cnumber6="��";
uchar code *Cnumber7="��";
uchar code *Cnumber8="��";
uchar code *Cnumber9="��";
uchar code *Cnumber10="ʮ";

uchar *MAPSolar(uchar num){
	uchar *p;
	switch(num){
		case 0:p=word0;break;
		case 1:p=word1;break;
		case 2:p=word2;break;
		case 3:p=word3;break;
		case 4:p=word4;break;
		case 5:p=word5;break;
		case 6:p=word6;break;
		case 7:p=word7;break;
		case 8:p=word8;break;
		case 9:p=word9;break;
		case 10:p=word10;break;
		case 11:p=word11;break;
		case 12:p=word12;break;
		case 13:p=word13;break;
		case 14:p=word14;break;
		case 15:p=word15;break;
		case 16:p=word16;break;
		case 17:p=word17;break;
		case 18:p=word18;break;
		case 19:p=word19;break;
		case 20:p=word20;break;
		case 21:p=word21;break;
		case 22:p=word22;break;
		case 23:p=word23;break;
	}
	return p;
}

uchar *MAPNumber(uchar num){
	uchar *p;
	switch(num){
		case 1:p=Cnumber1;break;
		case 2:p=Cnumber2;break;
		case 3:p=Cnumber3;break;
		case 4:p=Cnumber4;break;
		case 5:p=Cnumber5;break;
		case 6:p=Cnumber6;break;
		case 7:p=Cnumber7;break;
		case 8:p=Cnumber8;break;
		case 9:p=Cnumber9;break;
		case 10:p=Cnumber10;break;
	}
	return p;	
}