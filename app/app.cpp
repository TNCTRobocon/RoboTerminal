#if 1

#include "app.hpp"
#include "general/device.hpp"
#include <signal.h>
#include <iostream>
#include <future>
#include <vector>
#include <utility>
//#include <general/motor.hpp>

using namespace std;
using namespace Util;

#define CHECK_NOW chrono::milliseconds(0)

void DeviceInit();  // example
void ShortTask();   // example
void LongTask();    // example

//開放を自動化するためにスマートポインタで実装する。
shared_ptr<Argument> argument{nullptr};
shared_ptr<Settings> setting{nullptr};
shared_ptr<Report> report{nullptr};
shared_ptr<GamePad> gamepad{nullptr};
shared_ptr<DeviceManager> device_manager{nullptr};
future<void> device_control;

shared_ptr<DeviceMotor> device_motor0{nullptr};
shared_ptr<DeviceMotor> device_motor1{nullptr};

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
    auto serial_location = setting->Read("serial");
    auto band = setting->Read("serial-band").value_or("115200");
    if (serial_location) {
        // device_manager = move(DeviceManager::GenerateDeviceManager(
        // serial_location->c_str(), stoi(band)));
        // device_manager2 =
        //  move(DeviceManager::GenerateDeviceManager(serial_location->c_str(),stoi(band)));
    } else {
        report->Warn(ReportGroup::GamePad, "Missing Serial Location");
    }

    auto vec = device_manager->SearchFeature("example");

    for (auto& dev : vec) {
        dev->Echo("HI");
    }

    // MessageLoop

    signal(SIGINT, singal_receiver);

    // for(auto& it : device_manager->SearchFeature("motor")){
    //  it -> Echo("yahho-");
    //}

    for (is_continue = true; is_continue;) {  //例
        LongTask();   //２つのモーターそれぞれに草をシリアルで送信　それが終わるまでの間
        ShortTask();  //ゲームパッドと通信をし続ける
    }//またやり直す

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
