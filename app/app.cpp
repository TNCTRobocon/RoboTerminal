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

#define CHECK_NOW chrono::milliseconds(0)

void DeviceInit();  // example
void ShortTask();   // example
void LongTask();    // example

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
        // device_manager = move(DeviceManager::GenerateDeviceManager(
        // serial_location->c_str(), stoi(band)));
        // device_manager2 =
        //  move(DeviceManager::GenerateDeviceManager(serial_location->c_str(),stoi(band)));
    } else {
        report->Warn(ReportGroup::GamePad, "Missing GamePad Location");
    }
#ifdef RASPBERRY_PI
    //specialを関する記述

#endif
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

void DeviceInit() {
    device_motor0 = DeviceMotor::CreateMotor(device_manager, 16);
    device_motor1 = DeviceMotor::CreateMotor(device_manager, 17);
    for (auto& dev : device_manager->AllDevices()) {
        dev->Reset();
    }
}

void ShortTask() {
    do {
        // this_thread::sleep_for(chrono::milliseconds(50));
        if (gamepad) {
            gamepad->Update();
            cout << gamepad->Status();  //確認用
        }
    } while (device_control.wait_for(chrono::milliseconds(CHECK_NOW)) ==
             future_status::timeout);
}

void LongTask() {
    device_motor0->Echo("www");
    device_motor1->Echo("wwwwww");
    device_manager->Fetch();
    device_manager->Flush(device_control);
}

#endif
