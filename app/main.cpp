#include "main.hpp"
#include <general/gamepad.hpp>
#include <general/motor.hpp>
#include <special/ultrasonic.hpp>
#include <app/interface.hpp>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

#include <string.h>
#include <bits/unique_ptr.h>

#include <thread>
#include <memory>
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

    pad.Status();

    /*
    if(wiringPiSetup()!=0){
      printf("error wiringPi setup\n");
      return 1;
    }else{
      printf("success wiringPi setup\n");
    }

    //sonic_one();
    //printf("%lf\n",sonic_one.gettime());

    Sonic sonic_one(4);
    thread t1(sonic_one);
    getchar();
    sonic_one.sonicend();
    t1.join();
    return 0;
*/
    
  //while(1){
    if(pad.GetButton(ButtonNames::A)){
      printf("konnichiha\n");
    }
    /*auto it=make_unique<manual>();
      while(it!=nullptr){
        (*it)();
        it = move(it->next());
      }*/
      
      for(manual* it=new move_stop();it!=nullptr;){
        manual* next=it->next();
        if(next!=it){
          delete it;
          it = next;
        }
      }

  //}
}

void motor_init()
{
    //モーターの初期化
    //motor_manager = move(MotorManager::GenerateMotorManeger("/dev/ttyS0", 115200));
    motor_manager = move(MotorManager::GenerateMotorManeger("/dev/ttyUSB0", 115200));//パソコンでの実行環境
    //モーターにアドレスを割り当てるTODO正しい値を代入すること
    motor_front_right = motor_manager->CreateMotor(16);
    motor_front_left = motor_manager->CreateMotor(17);
    motor_back_right = motor_manager->CreateMotor(18);
    motor_back_left = motor_manager->CreateMotor(19);
}


void axis_handler(const GamePad*obj, AxisNames axis, float value)
{

}

void button_handler(const GamePad* obj, ButtonNames button, bool value)
{

}
