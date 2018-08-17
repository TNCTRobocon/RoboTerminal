#include "main.hpp"
#include <general/gamepad.hpp>
#include <general/motor.hpp>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
//#include <wiringPi.h>
//#include <wiringPiI2C.h>
//#include <wiringSerial.h>
#include <string.h>
#include <bits/unique_ptr.h>

using namespace std;

static motor_sptr motor_front_right{nullptr}, motor_front_left{nullptr}, motor_back_right{nullptr}, motor_back_left{nullptr};
static motor_manager_sptr motor_manager{nullptr};

int main()
{
    GamePad pad("/dev/input/js0");
    motor_init();

    //Trigger Handle
    pad.SetButtonChangedEvent(button_handler);
    pad.SetAxisChangedEvent(axis_handler);

    while (1) {

    }

}

void motor_init()
{
    //モーターの初期化
    motor_manager = move(MotorManager::GenerateMotorManeger("/dev/ttyS0", 115200));
    //モーターにアドレスを割り当てるTODO正しい値を代入すること
    auto afr = motor_manager->CreateMotor(1);
    auto afl = motor_manager->CreateMotor(2);
    auto abr = motor_manager->CreateMotor(3);
    auto abl = motor_manager->CreateMotor(4);
}


void axis_handler(const GamePad*obj, AxisNames axis, float value)
{

}

void button_handler(const GamePad* obj, ButtonNames button, bool value)
{

}
