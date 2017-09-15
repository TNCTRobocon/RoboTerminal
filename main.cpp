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


using namespace std;

int button_number;
bool button_value;
int axis_number;
float axis_value;


void ButtonHandler(const GamePad*,ButtonNames,bool);
void AxisHandler(const GamePad*,AxisNames,float);
/*
void Btn_A;
void Btn_B;
void Btn_X;
void Btn_Y;
void Btn_LB;
void Btn_RB;
void Btn_BACK;
void Btn_START;
void Btn_CROSS;
void Btn_AXISL;
void Btn_AXISR;
void Axs_LSX;
void Axs_LSY;
void Axs_RSX;
void Axs_RSY;
void Axs_RT;
void Axs_LT;
void Axs_DX;
void Axs_DY;
*/
void finish(int){
    puts("finish");
    exit(0);
}


int main(){
    GamePad pad("/dev/input/js0");
    signal(SIGINT,finish);
    MotorManager test();
    test.*GenerateMotorManegar("/dev/ttyS0",115200);
    //pad.Status();
    pad.SetButtonChangedEvent(ButtonHandler);
    pad.SetAxisChangedEvent(AxisHandler);
    //vector<int> vt{A,B,X,Y,LB,RB,BACK,START,CROSS,AXISL,AXISR};
    //pad.PadButtonHandler(int,void(int*)())
    //void (*ptr[11])()={Btn_A,Btn_B,Btn_X,Btn_Y,Btn_LB,Btn_RB,Btn_BACK,Btn_START,Btn_CROSS,Btn_AXISL,Btn_AXISR}
    while (1){
	//pad.ButtonDecision(button_number,(*ptr[11])());
	//pad.AxisDecision(axis_number);
	}
}


void AxisHandler(const GamePad*obj,AxisNames axis,float value){
	//puts("bbbb");
	printf("%s(%d),%f\n",GetAxisName(axis),(int)axis,value);
	axis_number = (int)axis;
	axis_value = value;
}


void ButtonHandler(const GamePad* obj,ButtonNames button,bool value){
	printf("%s,%s\n",GetButtonName(button),value?"true":"false");
	button_number = (int)button;
	button_value = value;
}


