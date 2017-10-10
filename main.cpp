#include "main.hpp"
#include "gamepad.hpp"
#include "motor.hpp"
#include "mecanum_control.hpp"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringSerial.h>

#include <memory>

#include <unistd.h>
#include <fcntl.h>
#include <string.h>


using namespace std;

int button_number;
bool button_value;
int axis_number;
float axis_value;

static MecanumWheel *wheelrf=nullptr,*wheellf=nullptr,*wheelrb=nullptr,*wheellb=nullptr;
static unique_ptr<Motor>mtwhrf = nullptr,unique_ptr<Motor>mtwhlf = nullptr,unique_ptr<Motor>mtwhrb = nullptr,unique_ptr<Motor>mtwhlbS = nullptr;
void ButtonHandler(const GamePad*,ButtonNames,bool);
void AxisHandler(const GamePad*,AxisNames,float);
void compute(const GamePad*);
void shootout(const GamePad*);
void conf(auto);
void init();
void deinit();

void finish(int){
    puts("finish");
    exit(0);
}


int main(){
    GamePad pad("/dev/input/js0");
    signal(SIGINT,finish);
	auto mm = MotorManager::GenerateMotorManeger("/dev/ttyS0",115200);
    //auto legfr = mm->GenerateMotor(16);
    //legfr->Select();
	//legfr->Duty(0.2);
	
	conf(mm);
	init();
	
	mm->Synchronize();
    pad.Status();
    pad.SetButtonChangedEvent(ButtonHandler);
    pad.SetAxisChangedEvent(AxisHandler);
    //pad.SetButtonChangedEvent(const GamePad* obj,ButtonNames button,bool value);
    
    while (1){
		
    }
    deinit();
}

void conf(auto mm){
	unique_ptr<Motor>mtwhrf = mm->GenerateMotor(16);
	unique_ptr<Motor>mtwhlf = mm->GenerateMotor(17);
	unique_ptr<Motor>mtwhrb = mm->GenerateMotor(18);
	unique_ptr<Motor>mtwhlb = mm->GenerateMotor(19);
}


void init(){
	wheelrf=new MecanumWheel(2.0,1.0,2.0,1.0);
	wheellf=new MecanumWheel(2.0,1.0,2.0,1.0);
	wheelrb=new MecanumWheel(2.0,1.0,2.0,1.0);
	wheellb=new MecanumWheel(2.0,1.0,2.0,1.0);
}

void deinit(){
	delete wheellb;
	delete wheelrb;
	delete wheellf;
	delete wheelrf;
}

void shootout(const GamePad*obj){
	if(obj -> GetButton(ButtonNames::A)==1){
			printf("konnichiha");
	}
	
}

void compute(const GamePad*obj,auto mm){
	const double movegain = 1;
	const double spingain = 1;
	double x = obj -> GetAxis(AxisNames::LSX)*movegain;
	double y = obj -> GetAxis(AxisNames::LSY)*movegain;
	double r = (obj -> GetAxis(AxisNames::LT) - obj -> GetAxis(AxisNames::RT))*spingain;
	mtwhrf -> AsyncRPM((*wheelrf)(x,y,r)/4);
	mtwhlf -> AsyncRPM((*wheellf)(x,y,r)/4);
	mtwhrb -> AsyncRPM((*wheelrb)(x,y,r)/4);
	mtwhlb -> AsyncRPM((*wheellb)(x,y,r)/4);
	mm.Synchronize();
}

void AxisHandler(const GamePad*obj,AxisNames axis,float value,auto mm){
	printf("%s(%d),%f\n",GetAxisName(axis),(int)axis,value);
	auto tire_conditions[] ={AxisNames::DX,AxisNames::DY,AxisNames::RT,AxisNames::LT};
	for(auto it:tire_conditions){
		if(it == axis){
			compute(obj,mm);
			break;
		}
	}
	axis_number = (int)axis;
	axis_value = value;	
}

void ButtonHandler(const GamePad* obj,ButtonNames button,bool value){
	printf("%s,%s\n",GetButtonName(button),value?"true":"false");
	auto conditions[] = {ButtonNames::X,ButtonNames::Y,ButtonNames::START,ButtonNames::BACK};
	for(auto it:conditions){
		if(it == button){
			shootout(obj);
			break;
		}
	}
	
	button_number = (int)button;
	button_value = value;
}
