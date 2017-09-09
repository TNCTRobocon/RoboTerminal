#include "main.hpp"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "gamepad.hpp"
#include "motor.hpp"

#include <string>
#include <vector>

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringSerial.h>


using namespace std;
/*
int button_name;
bool button_value;
int axis_name;
float axis_value;
*/
//void ButtonHandler(const GamePad*,ButtonNames,bool);
//void AxisHandler(const GamePad*,AxisNames,float);

void finish(int){
    puts("finish");
    exit(0);
}


int main(){
	//int youso;
    GamePad pad("/dev/input/js0");
    signal(SIGINT,finish);
    //char Send[] = "sel 20";
    //youso = sizeof Send/sizeof Send[0];
    MotorManager test("/dev/ttyS0",115200);
    //pad.Status();
    //pad.SetButtonChangedEvent(ButtonHandler);
    //pad.SetAxisChangedEvent(AxisHandler);
    //pad.SetButtonChangedEvent(const GamePad* obj,ButtonNames button,bool value);
    /*
    int& btn=button_name;
    
	vector<int> vec(32);
	for (vector<int>::iterator p=vec.begin();p!=vec.end();p++){
		cout<<*p;
	}
	*/
    while (1){
		//test.Write(Send,youso);
	}
}

/*
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

*/
