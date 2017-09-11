#include "motor.hpp"
#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
//test

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringSerial.h>



MotorManager::MotorManager(const char* filename,int rate/*speed_t rate*/){
   
   fd=serialOpen("/dev/ttyS0",115200);
	if(fd<0){
		printf("can not open serialport\n");
		exit(1);
	}else{ 
	    printf("success open serialport\n");
	}
	
}


MotorManager::~MotorManager(){
	
    if (fd>=0){
        //swap config
        serialClose(fd);
        printf("serial port closed");
    }
}

std::unique_ptr<Motor> MotorManager::GenerateMotor(address_t addr){
return nullptr;//TODO STUB
}

void MotorManager::Write(const char *send/*,int youso*/){
	/*
	int i;
	for(i = 0;i<youso;i++){
		printf("%c",*send);
		send++;
	}
	printf("\n");*/
	printf("%c",*send);
}
/*
void dtm(int id,float dt){
	std::string word("sel	%d\r",id);
	void MotorManager::Write(&word);
	std::string word("dt	%4.2f\r",dt);
	void MotorManager::Write(&word);
	delay(1);
}


void mcm(int id,int mc){
	std::string word("sel	%d\r",id);
	void MotorManager::Write(&word);
	std::string word("dt	%4.2f\r",mc);
	void MotorManager::Write(&word);
	if(id==tirelb){
		std::string word("go\r");
		void MotorManager::Write(&word);
	}
}
*/
