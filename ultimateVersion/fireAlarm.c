#include "reg52.h"
#include "intrins.h"
#include "blueTeeth.h"
#include "DHT11.h"

#define uchar unsigned char 
#define uint unsigned int

void fireAlarm(){
	if(temprature>=70){
		sendDate(0x44);
	}	
}