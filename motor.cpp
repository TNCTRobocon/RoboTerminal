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



MotorManager::MotorManager(const char* filename,int rate){
   
   fd=serialOpen(filename,rate);
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
return nullptr;
}

void MotorManager::Write(const char *str){
	serialPrintf(fd,str);
}


