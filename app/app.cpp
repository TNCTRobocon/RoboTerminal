#include "app.hpp"
#include <signal.h>
#include <iostream>
#include <general/motor.hpp>
using namespace std;
using namespace Util;
//開放を自動化するためにスマートポインタで実装する。
shared_ptr<Argument> argument{nullptr};
shared_ptr<Settings> setting{nullptr};
shared_ptr<Report> report{nullptr};
shared_ptr<GamePad> gamepad{nullptr};

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
    //ゲームパッドを初期化する
    auto gamepad_location = setting->Read("gamepad");
    if (gamepad_location) {
        gamepad.reset(new GamePad(*gamepad_location));
    } else {
        report->Warn(ReportGroup::GamePad, "Missing GamePad Location");
    }
    //シリアルポートを初期化する
    auto serial_location =setting->Read("serial");
    auto band=setting->Read("serial-band").value_or("115200");
    if (serial_location){
        MotorManager::GenerateMotorManeger(serial_location->c_str(),stoi(band));
    }else{
        report->Warn(ReportGroup::GamePad, "Missing GamePad Location");
    }

    // MessageLoop

    signal(SIGINT, singal_receiver);

    for (is_continue = true; is_continue;) {
        if (gamepad) {
            gamepad->Update();
            //cout << gamepad->Status();//確認用
        }
    }

    report->Info(ReportGroup::System, "Shutdown");
    return 0;
}
