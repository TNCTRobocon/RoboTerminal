#include "main.hpp"
#include "gamepad.hpp"
#include "motor.hpp"
#include "mecanum_control.hpp"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringSerial.h>
#include "mecanum_ui.hpp"
#include <string.h>
#include <bits/unique_ptr.h>

using namespace std;

static motor_sptr motor_front_right{nullptr}, motor_front_left{nullptr}, motor_back_right{nullptr}, motor_back_left{nullptr};
static motor_manager_sptr motor_manager{nullptr};
static mecanum_ui_uptr mecanum_ui{nullptr};

int main()
{
    GamePad pad("/dev/input/js0");
    motor_init();

    //Trigger Handle
    pad.SetButtonChangedEvent(button_handler);
    pad.SetAxisChangedEvent(axis_handler);

    while (1) {

    }

}

void motor_init()
{
    //モーターの初期化
    motor_manager = move(MotorManager::GenerateMotorManeger("/dev/ttyS0", 115200));
    //モーターにアドレスを割り当てるTODO正しい値を代入すること
    auto afr = motor_manager->CreateMotor(1);
    auto afl = motor_manager->CreateMotor(2);
    auto abr = motor_manager->CreateMotor(3);
    auto abl = motor_manager->CreateMotor(4);
    //各モーターの動作を規定する。TODO 正しい値を代入すること
    MecanumWheel mfr(M_PI / 4 + M_PI / 2 * 0);
    MecanumWheel mfl(M_PI / 4 + M_PI / 2 * 1);
    MecanumWheel mbr(M_PI / 4 + M_PI / 2 * 2);
    MecanumWheel mbl(M_PI / 4 + M_PI / 2 *3 );
    //モーター一覧を作成
    motor_pair_list list;
    list.push_back(motor_pair(afr, mfr));
    list.push_back(motor_pair(afl, mfl));
    list.push_back(motor_pair(abr, mbr));
    list.push_back(motor_pair(abl, mbl));
    //uiを作成
    mecanum_ui.reset(new MecanumUI(list));
}

void shootout(const GamePad*obj)
{
    if (obj -> GetButton(ButtonNames::A)) {
        printf("konnichiha");
    }

}

void axis_handler(const GamePad*obj, AxisNames axis, float value)
{
    if (mecanum_ui != nullptr) {
        if (mecanum_ui->Sense(axis)) {
            mecanum_ui->Action(obj);
        }
    }

}

void button_handler(const GamePad* obj, ButtonNames button, bool value)
{
    if (mecanum_ui != nullptr) {
        if (mecanum_ui->Sense(button)) {
            mecanum_ui->Action(obj);
        }
    }
}
