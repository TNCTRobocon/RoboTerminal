#include "main.hpp"
#include "gamepad.hpp"
#include "motor.hpp"
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

int button_name;
bool button_value;
int axis_name;
float axis_value;

void ButtonHandler(const GamePad*,ButtonNames,bool);
void AxisHandler(const GamePad*,AxisNames,float);

void finish(int){
    puts("finish");
    exit(0);
}


int main(){
    GamePad pad("/dev/input/js0");
    signal(SIGINT,finish);
	auto mm = MotorManager::GenerateMotorManeger("/dev/tty0",115200);
    auto legfr = mm->GenerateMotor(16);
	legfr->Select();
	legfr->Duty(0.2);
	
	mm->Synchronize();
    pad.Status();
    pad.SetButtonChangedEvent(ButtonHandler);
    pad.SetAxisChangedEvent(AxisHandler);
    //pad.SetButtonChangedEvent(const GamePad* obj,ButtonNames button,bool value);
    
    while (1){
		}
    }


void AxisHandler(const GamePad*obj,AxisNames axis,float value){
	//puts("bbbb");
	//printf("%s(%d),%f\n",GetAxisName(axis),(int)axis,value);
	axis_name = (int)axis;
	axis_value = value;
	
}


void ButtonHandler(const GamePad* obj,ButtonNames button,bool value){
	//printf("%s,%s\n",GetButtonName(button),value?"true":"false");
	button_name = (int)button;
	button_value = value;
}
