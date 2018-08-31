#pragma once

namespace Spcl{

class LimitSwitch{
private:
  int pin_num;
public:
  LimitSwitch(int);
  bool GetStates();

};

}
