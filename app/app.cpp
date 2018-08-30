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

using long_task_result_t = string;
#define CHECK_NOW chrono::milliseconds(0)

//開放を自動化するためにスマートポインタで実装する。
shared_ptr<Argument> argument{nullptr};
shared_ptr<Settings> setting{nullptr};
shared_ptr<Report> report{nullptr};
shared_ptr<GamePad> gamepad{nullptr};
shared_ptr<DeviceManager> device_manager{nullptr};
// shared_ptr<DeviceManager> device_manager2{nullptr};
// vector<future<long_task_result_t> > ;
future<void> long_task;

static bool volatile is_continue{false};
static void singal_receiver(int num) {
    cout << endl;
    is_continue = false;
}

void ShortTask();
void LongTaskBefore(auto func);
long_task_result_t LongTaskAfter();

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
        device_manager = move(DeviceManager::GenerateDeviceManager(
            serial_location->c_str(), stoi(band)));
        // device_manager2 =
        //  move(DeviceManager::GenerateDeviceManager(serial_location->c_str(),stoi(band)));
    } else {
        report->Warn(ReportGroup::GamePad, "Missing Serial Location");
    }

    // MessageLoop

    signal(SIGINT, singal_receiver);

    for (is_continue = true; is_continue;) {
        /*
          // EXAMPLE
          LongTaskBefore([]() -> long_task_result_t {
              device_manager->Select(39);
              device_manager->WriteSerial("echo miku\r");
              return device_manager->ReadSerial();
          });
          // LongTaskBefore(...
          // LongTaskBefore(...
          bool all_task_finished = false;
          while (!all_task_finished) {
              ShortTask();
              all_task_finished = true;
              for (auto& result : results) {
                  if (result.wait_for(CHECK_NOW) == future_status::timeout) {
                      all_task_finished = false;
                      break;
                  }
              }
          }
          cout << LongTaskAfter() << endl;  // "miku" expected
          // cout << LongTaskAfter() << endl;
          // cout << LongTaskAfter() << endl;
          */
    }

    report->Info(ReportGroup::System, "Shutdown");
    return 0;
}

void ShortTask() {
    if (gamepad) {
        gamepad->Update();
        cout << gamepad->Status();  //確認用
    }
}

/*
void LongTaskBefore(auto func) {
    results.push_back(async(launch::async, func));
}

long_task_result_t LongTaskAfter() {
    long_task_result_t temp = results.front().get();
    results.erase(results.begin());
    return temp;
}
*/

#endif
