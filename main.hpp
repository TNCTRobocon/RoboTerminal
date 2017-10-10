#pragma once
#include "gamepad.hpp"

void button_handler(const GamePad*,ButtonNames,bool);
void motor_init();
void axis_handler(const GamePad*,AxisNames,float);
void mecanum_handler(const GamePad*);
void shootout(const GamePad*);