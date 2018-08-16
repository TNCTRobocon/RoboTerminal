
#include "interface.hpp"

//DutyからAsyncRPMに変える。

void move_front::operator()(float value){
  motor_front_right->Duty(value);
  motor_front_left->Duty(value);
  motor_back_right->Duty(value);
  motor_back_left->Duty(value);
}

void move_back::operator()(float value){
  motor_front_right->Duty(-value);
  motor_front_left->Duty(-value);
  motor_back_right->Duty(-value);
  motor_back_left->Duty(-value);
}

void move_right::operator()(float value){
  motor_front_right->Duty(-value);
  motor_front_left->Duty(value);
  motor_back_right->Duty(value);
  motor_back_left->Duty(-value);
}

void move_left::operator()(float value){
  motor_front_right->Duty(value);
  motor_front_left->Duty(-value);
  motor_back_right->Duty(-value);
  motor_back_left->Duty(value);
}

void move_stop::operator()(){
  motor_front_right->Duty(0);
  motor_front_left->Duty(0);
  motor_back_right->Duty(0);
  motor_back_left->Duty(0);
}
