//Micro servo MG90S
#include "servo.hpp"
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

namespace Spcl{

Servo::Servo(int n){
  pin_num = n;
  pinMode(pin_num,OUTPUT);
	softPwmCreate(pin_num,15,pwm_range);
}

void Servo::MoveP90(){
  softPwmWrite(pin_num,6);
}

void Servo::Move0(){
  softPwmWrite(pin_num,15);
}

void Servo::MoveM90(){
  softPwmWrite(pin_num,24);
}

void Servo::Move(int degrees){
  if(degrees>=-90 && degrees<=90){
    softPwmWrite(pin_num,15+degrees/10);
  }else{
    printf("servo degrees error\n");
  }
}

}//namespace Spcl
