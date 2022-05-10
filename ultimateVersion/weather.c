#include "reg52.h"
#include "intrins.h"
#include "string.h"
#include "DHT11.h"
#include "LCDNew.h"
#include "map.h"

#define uchar unsigned char 
#define uint unsigned int

void Weather(uchar num){
	if(num==13){
		LCDSetPos(1,3);
		DisplayWord("ÇçÌì");
	}
	if(num==14){
		LCDSetPos(1,3);
		DisplayWord("ÒõÌì");		
	}
	if(num==15){
		LCDSetPos(1,3);
		DisplayWord("ÓêÌì");		
	}			
}