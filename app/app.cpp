#include "app.hpp"
#include "state.hpp"
#include <signal.h>
#include <iostream>
using namespace std;
using namespace Util;
//開放を自動化するためにスマートポインタで実装する。
shared_ptr<Argument> argument{nullptr};
shared_ptr<Settings> setting{nullptr};
shared_ptr<Report> report{nullptr};
shared_ptr<GamePad> gamepad{nullptr};

motor_sptr tire_flont_right{nullptr},tire_flont_left{nullptr},tire_back_right{nullptr},tire_back_left{nullptr};
motor_manager_sptr motor_manager{nullptr};

static bool volatile is_continue{false};
static void singal_receiver(int num) {
    cout << endl;
    is_continue = false;
}

int main(int argc, char** argv) {
    // System全体で使う変数を初期化する
    argument.reset(new Argument(argc, argv));
    report.reset(new Report("report.log"));
    report->Info(ReportGroup::System, "Wake Up");
    setting.reset(new Settings("setting.config"));
    #if 0
    //ゲームパッドを初期化する
    auto gamepad_location = setting->Read("gamepad");
    if (gamepad_location) {
        gamepad.reset(new GamePad(*gamepad_location));
    } else {
        report->Warn(ReportGroup::GamePad, "Missing GamePad Location");
    }
    #endif
    //シリアルポートを初期化する
    auto serial_location =setting->Read("serial");
    auto band=setting->Read("serial-band").value_or("115200");
    if (serial_location){
      //motor_manager = move(MotorManager::GenerateMotorManeger(serial_location->c_str(),stoi(band)));
    }else{
      report->Warn(ReportGroup::GamePad, "Missing GamePad Location");
    }
    //モーターのアドレス設定
#if 0
    auto motor_flontleft_address = setting->Read("flontleft").value_or("16");
    auto motor_flontright_address = setting->Read("flontright").value_or("17");
    auto motor_backleft_address = setting->Read("backleft").value_or("18");
    auto motor_backright_address = setting->Read("backright").value_or("19");
    //モーターの生成
    tire_flont_left = motor_manager->CreateMotor(stoi(motor_flontleft_address));
    tire_flont_right = motor_manager->CreateMotor(stoi(motor_flontright_address));
    tire_back_left = motor_manager->CreateMotor(stoi(motor_backleft_address));
    tire_back_right = motor_manager->CreateMotor(stoi(motor_backright_address));
#endif
    // MessageLoop
    signal(SIGINT, singal_receiver);

    for (is_continue = true; is_continue;) {
      for(automatic* it=new FirstMove();it!=nullptr;){
        (*it)();
        automatic* next=it->next();
        if(next!=it){
          delete it;
          it = next;
        }
      }

      /*
        if (gamepad) {
            gamepad->Update();
            //cout << gamepad->Status();//確認用
        }
        for(manual* it=new MoveStop();it!=nullptr;){
          manual* next=it->next();
          if(next!=it){
            delete it;
            it = next;
          }
          if (gamepad) {
              gamepad->Update();
          }
          cout << gamepad->GetAxis(AxisNames::LSY) << endl;
        }*/

    }

    report->Info(ReportGroup::System, "Shutdown");
    return 0;
}
