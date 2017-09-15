#include "motor.hpp"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringSerial.h>



MotorManager::MotorManager(const char* filename,int rate){
   
   fd=serialOpen(filename,rate);
	if(fd<0){
		printf("can not open serialport\n");
		exit(1);
	}else{ 
	    printf("success open serialport\n");
	    serialPrintf(srid,"sel 0\r");
		serialPrintf(srid,"dt 0\r");
		serialPrintf(srid,"mc 0\r");
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
	return nullptr;
}

void MotorManager::Write(const char *str){
	serialPrintf(fd,str);
}

void Motor::Sel(int id){
	Write("sel %d\r",id)
}

void Motor::Duty(float dt_value){
	Write("dt %4.2f\r",dt_value);
}

void Motor::MotorControl_rady(int mc_value){
	Write("rady %d\r",mc_value);
}

void Motor::MotorControl_go(){
	Write("go\r");
}
