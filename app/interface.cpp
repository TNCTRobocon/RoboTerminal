#include <stdio.h>
#include "interface.hpp"

//DutyからAsyncRPMに変える。

void move_front::operator()(){
  printf("front\n");
/*
  motor_front_right->Duty(0.6);
  motor_front_left->Duty(0.6);
  motor_back_right->Duty(0.6);
  motor_back_left->Duty(0.6);
*/
}

void move_back::operator()(){
  printf("back\n");
/*
  motor_front_right->Duty(-0.6);
  motor_front_left->Duty(-0.6);
  motor_back_right->Duty(-0.6);
  motor_back_left->Duty(-0.6);
*/
}

void move_right::operator()(){
  printf("right\n");
/*
  motor_front_right->Duty(-0.6);
  motor_front_left->Duty(0.6);
  motor_back_right->Duty(0.6);
  motor_back_left->Duty(-0.6);
*/
}

void move_left::operator()(){
  printf("left\n");
/*
  motor_front_right->Duty(0.6);
  motor_front_left->Duty(-0.6);
  motor_back_right->Duty(-0.6);
  motor_back_left->Duty(0.6);
*/
}

void move_stop::operator()(){
  printf("stop\n");
/*
  motor_front_right->Duty(0);
  motor_front_left->Duty(0);
  motor_back_right->Duty(0);
  motor_back_left->Duty(0);
*/
}

void move_delay::operator()(){
  printf("delay\n");
  //delay(1000);
}
