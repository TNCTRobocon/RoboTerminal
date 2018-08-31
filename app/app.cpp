#include "app.hpp"
#include <signal.h>
#include <general/motor.hpp>
#include <iostream>
#include <ui/ui.hpp>
// TODO あとで消す
#include <gtk/gtk.h>

using namespace std;
using namespace Util;
using namespace UI;

//開放を自動化するためにスマートポインタで実装する。
shared_ptr<Argument> argument{nullptr};
shared_ptr<Settings> setting{nullptr};
shared_ptr<Report> report{nullptr};
shared_ptr<GamePad> gamepad{nullptr};
shared_ptr<Window> window{nullptr};

//グローバル変数の初期化
static void shared_init(int* argc, char*** argv) {
    // System全体で使う変数を初期化する
    argument.reset(new Argument(*argc, *argv));
    report.reset(new Report("report.log"));
    report->Info(ReportGroup::System, "Wake Up");
    setting.reset(new Settings("setting.config"));
    // Windowsを初期化する
    window.reset(new Window(argc, argv));
    //ゲームパッドを初期化する
    auto gamepad_location = setting->Read("gamepad");
    if (gamepad_location) {
        gamepad.reset(new GamePad(*gamepad_location));
    } else {
        report->Warn(ReportGroup::GamePad, "Missing GamePad Location");
    }
    //シリアルポートを初期化する
    /*auto serial_location = setting->Read("serial");
    auto band = setting->Read("serial-band").value_or("115200");
    if (serial_location) {
        MotorManager::GenerateMotorManeger(serial_location->c_str(),
                                           stoi(band));
    } else {
        report->Warn(ReportGroup::GamePad, "Missing GamePad Location");
    }*/
}

int main(int argc, char** argv) {
    shared_init(&argc, &argv);

    while (window->Process()) {
        if (gamepad) {
            gamepad->Update();
        }
    }

    report->Info(ReportGroup::System, "Shutdown");
    return 0;
}
