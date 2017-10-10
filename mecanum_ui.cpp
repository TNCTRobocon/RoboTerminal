#include "mecanum_control.hpp"
#include "motor.hpp"
#include "mecanum_ui.hpp"

const AxisNames MecanumUI::sense_axises[] = {AxisNames::LSX, AxisNames::LSY, AxisNames::RT, AxisNames::LT};
const ButtonNames MecanumUI::sense_buttons[] = {ButtonNames::AXISL};

MecanumUI::MecanumUI(motor_pair_list m, double gw,double gr, double gs) :
motors(m), gain_walk(gw), gain_run(gr),gain_spin(gs)
{
}

MecanumUI::MecanumUI(const MecanumUI& orig)
{
}

MecanumUI::~MecanumUI()
{
}

bool MecanumUI::Sense(AxisNames a)const
{
    for (auto it : sense_axises) {
        if (a == it)return true;
    }
    return false;
}//再計算が必要がどうか

bool MecanumUI::Sense(ButtonNames b) const
{
    for (auto it : sense_buttons) {
        if (b == it)return true;
    }
    return false;
}

void MecanumUI::Action(const GamePad* g)
{
    //ゲームパッドから情報を取得
    double x, y, s;
    bool f;//加速
    x = g->GetAxis(AxisNames::LSX);
    y = g->GetAxis(AxisNames::LSY);
    s = g->GetAxis(AxisNames::LT)-g->GetAxis(AxisNames::RT);
    f = g->GetButton(ButtonNames::AXISL);
    //それぞれの値に補正をかける。
    x*=f?gain_walk:gain_run;
    y*=f?gain_walk:gain_run;
    s*=gain_spin;
    //各モータに指示を与える。
    for (auto &it :motors){
        double rpm= it.second.Calculate(x,y,s);
        it.first->RPM((float)rpm);
    }
    
    
}