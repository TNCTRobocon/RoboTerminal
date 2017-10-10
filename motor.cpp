#include "motor.hpp"
#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringSerial.h>
#include <iostream>
#include <sstream>
#include <iomanip>


using namespace std;
//モーターのコマンド
const static string cmd_newline="\r";
const static string cmd_space=" ";

const static string cmd_select="sel";
const static string cmd_duty="dt";
const static string cmd_motor_control="mc";
const static string cmd_async_motor_control="rady";
const static string cmd_sync="go";
const static string cmd_angle="sc";
const static string cmd_reset="rst";
const static string cmd_stop="stop";



MotorManager::MotorManager(const char* filename,int rate){
   fd=serialOpen(filename,rate);
	if(fd<0){
		printf("can not open serialport\n");
		exit(1);
	}else{
	    printf("success open serialport\n");
	    Command(cmd_reset);
	}
}

MotorManager::~MotorManager(){
    if (fd>=0){
        //swap config
        serialClose(fd);
        printf("serial port closed");
    }
}

void MotorManager::Write(const std::string& text){
	serialPrintf(fd,text.c_str());
}

unique_ptr<Motor>MotorManager::GenerateMotor(address_t addr){
	return unique_ptr<Motor>(new Motor(this,addr));
}


void MotorManager::Command(const std::string& command){
	serialPrintf(fd,command.c_str());
	//serialPrintf(fd,cmd_newline);
}

void MotorManager::Synchronize(){
	Command(cmd_sync);
}

void Motor::Select(){
		stringstream ss;
		ss<<cmd_select<<' '<<address;
		parent->Command(ss.str());
}

void Motor::Duty(float value){
			Select();
			stringstream ss;
			ss<<cmd_duty<<' '<<value;
			parent->Command(ss.str());
}

void Motor::AsyncRPM(float value){
			Select();
			stringstream ss;
			ss<<cmd_async_motor_control<<' '<<value;
			parent->Command(ss.str());
}

void Motor::RPM(float value){
			Select();
			stringstream ss;
			ss<<cmd_motor_control<<' '<<value;
			parent->Command(ss.str());

}

void Motor::Stop(){
		Select();
		parent->Command(cmd_stop);
}

Motor::Motor( MotorManager* p,address_t adr){
	parent = p;
	address = adr;
}
