#pragma once
#ifndef MECANUM_UI_HPP
#define MECANUM_UI_HPP

#include "motor.hpp"
#include "mecanum_control.hpp"
#include "gamepad.hpp"
#include "memory"
#include <utility>
#include <vector>
class MecanumUI;
using mecanum_ui_uptr=std::unique_ptr<MecanumUI> ;
using motor_pair= std::pair<motor_sptr,MecanumWheel>;
using motor_pair_list = std::vector<motor_pair>;

class MecanumUI {
    static const AxisNames sense_axises[];//応答するスティック
    static const ButtonNames sense_buttons[];//応答するボタン
    motor_pair_list motors;//モーターとその設定
    double gain_walk,gain_run,gain_spin;
public:
    MecanumUI(motor_pair_list m,double gw=1.0,double gr=2.0,double gs=1.0);
    MecanumUI(const MecanumUI& orig);
    virtual ~MecanumUI();
    bool Sense(AxisNames)const;//再計算が必要がどうか
    bool Sense(ButtonNames)const;//再計算が必要がどうか
    void Action(const GamePad*);
    void operator ()(const GamePad* g){
        Action(g);
    }
    
private:

};

#endif /* MECANUM_UI_HPP */

