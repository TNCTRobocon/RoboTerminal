/*
	module : 28015 REV B
*/
#if 0
#include "ultrasonic.hpp"
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringSerial.h>
#include <mutex>
#include <time.h>
#include <sys/time.h>

std::mutex mtxt;
std::mutex mtxp;

void Sonic::sonicend(){
	end_flag = false;
}

Sonic::Sonic(int n){
	pin_num = n;
}

void Sonic::operator()(){
	struct timeval begin,end;

	while(end_flag){
		pinMode(pin_num,OUTPUT);
		digitalWrite(pin_num,1);
		pinMode(pin_num,INPUT);
		while(digitalRead(pin_num)!=1);
		gettimeofday(&begin,nullptr);
		while(digitalRead(pin_num)!=0);
		gettimeofday(&end,nullptr);
		mtxt.lock();
		time = (end.tv_sec-begin.tv_sec)+(end.tv_usec-begin.tv_usec)*1.0E-6;
		mtxt.unlock();
		auto temp = gettime();
		if(temp){
			printf("%lf\n",gettime());
		}else{
			printf("no\n");
		}
	}
}
#endif