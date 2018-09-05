/*
	module : 28015 REV B
*/
//#if 0
#include "ultrasonic.hpp"
#include <stdio.h>
#include <wiringPi.h>
#include <time.h>
#include <sys/time.h>

namespace Spcl{

Sonic::Sonic(int n){
	pin_num = n;
}

double Sonic::operator()(){
	struct timeval begin,end;
	bool time_flag = false;
	int count = 0;
	pinMode(pin_num,OUTPUT);
	digitalWrite(pin_num,1);
	pinMode(pin_num,INPUT);
	delayMicroseconds(750);
	while(digitalRead(pin_num)!=1){
		delayMicroseconds(1);
		count++;
		if(count>20000){
			time_flag = true;
			break;
		}
	}
	gettimeofday(&begin,nullptr);
	while(digitalRead(pin_num)!=0){};
	gettimeofday(&end,nullptr);
	if(!time_flag){
		return (end.tv_sec-begin.tv_sec)+(end.tv_usec-begin.tv_usec)*1.0E-6;
	}else{
		return 0;
	}
}

}//namespace Spcl
//#endif
