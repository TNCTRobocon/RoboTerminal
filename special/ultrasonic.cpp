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

std::mutex mtxt;

void Sonic::sonicend(){
	end_flag = false;
}

Sonic::Sonic(int n){
	pin_num = n;
}

void Sonic::operator()(){
	struct timeval begin,end;
	bool time_flag = false;
	int count = 0;
	while(end_flag){
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
			mtxt.lock();
			time = (end.tv_sec-begin.tv_sec)+(end.tv_usec-begin.tv_usec)*1.0E-6;
			mtxt.unlock();
			//printf("%lf\n",gettime());
		}else{
			time_flag = false;
		}
		count = 0;
		delay(70);//19.455ms
	}
}

}//namespace Spcl
//#endif
