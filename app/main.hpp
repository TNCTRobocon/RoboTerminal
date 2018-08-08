#pragma once
#include <general/gamepad.hpp>

void button_handler(const GamePad*,ButtonNames,bool);
void motor_init();
void axis_handler(const GamePad*,AxisNames,float);
