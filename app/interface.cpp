
#include "interface.hpp"

//DutyからAsyncRPMに変える。

void move_front::operator()(){
  motor_front_right->Duty(0.6);
  motor_front_left->Duty(0.6);
  motor_back_right->Duty(0.6);
  motor_back_left->Duty(0.6);
}

void move_back::operator()(){
  motor_front_right->Duty(-0.6);
  motor_front_left->Duty(-0.6);
  motor_back_right->Duty(-0.6);
  motor_back_left->Duty(-0.6);
}

void move_right::operator()(){
  motor_front_right->Duty(-0.6);
  motor_front_left->Duty(0.6);
  motor_back_right->Duty(0.6);
  motor_back_left->Duty(-0.6);
}

void move_left::operator()(){
  motor_front_right->Duty(0.6);
  motor_front_left->Duty(-0.6);
  motor_back_right->Duty(-0.6);
  motor_back_left->Duty(0.6);
}

void move_stop::operator()(){
  motor_front_right->Duty(0);
  motor_front_left->Duty(0);
  motor_back_right->Duty(0);
  motor_back_left->Duty(0);
}

void move_delay::operator()(){
  delay(1000);
}
