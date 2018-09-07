#if 1

#include "app.hpp"
#include "general/device.hpp"
#include <signal.h>
#include <general/motor.hpp>
#include <iostream>
#include <ui/ui.hpp>
// TODO あとで消す
#include <gtk/gtk.h>

using namespace std;
using namespace Util;
using namespace UI;

std::optional<Application> app{std::nullopt};

Application::Application(int* argc, char*** argv) {
    // System全体で使う変数を初期化する
    argument.reset(new Argument(*argc, *argv));
    report.reset(new Report("report.log"));
    report->Info(ReportGroup::System, "Wake Up");
    setting.reset(new Settings("setting.config"));
    // Windowsを初期化する
    window.reset(new Window(argc, argv));
    report->Info(ReportGroup::GUI, "UI Wake Up");
    //ゲームパッドを初期化する
    auto gamepad_location = setting->Read("gamepad");
    if (gamepad_location) {
        gamepad.reset(new GamePad(*gamepad_location));
    } else {
        report->Warn(ReportGroup::GamePad, "Missing GamePad Location");
    }
    //シリアルポートを初期化する
    auto serial_location = setting->Read("serial");
    auto band = setting->Read("serial-band").value_or("115200");
    if (serial_location) {
        MotorManager::GenerateMotorManeger(serial_location->c_str(),
                                           stoi(band));
    } else {
        report->Warn(ReportGroup::GamePad, "Missing GamePad Location");
    }
#ifdef RASPBERRY_PI
    // specialを関する記述

#endif
}

Application::~Application() {
    report->Info(ReportGroup::System, "Shutdown");
}

bool Application::Process() {
    if (window->Process()) {
        if (gamepad) {
            gamepad->Update();
        }
        return true;
    }
    return false;
}

int main(int argc, char** argv) {
    app.emplace(&argc,&argv);
    while (app->Process());
    return 0;
}

void ShortTask() {
  this_thread::sleep_for(chrono::milliseconds(100));
    if (gamepad) {
        gamepad->Update();
        cout << gamepad->Status();  //確認用
    }

}



#endif
