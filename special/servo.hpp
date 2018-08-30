#pragma once

//Micro servo MG90S

namespace Spcl{

class Servo{
private:
  int pin_num;
  int pwm_range{200};//20ms
public:
  Servo(int);
  void MoveP90();// →
  void Move0();// ↑
  void MoveM90();// ←
  void Move(int);
};

}//namespace Spcl
