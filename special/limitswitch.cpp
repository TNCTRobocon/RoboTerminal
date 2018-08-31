#include "limitswitch.hpp"
#include <wiringPi.h>
#include <iostream>

namespace Spcl{

LimitSwitch::LimitSwitch(int n){
  pin_num = n;
  pinMode(pin_num,INPUT);
}

inline bool LimitSwitch::GetStates(){
  return digitalRead(pin_num);
}

}//namespace Spcl
