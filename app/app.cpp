#if 1

#include "app.hpp"
#include "general/device.hpp"
#include <signal.h>
#include <iostream>
#include <future>
#include <vector>
#include <utility>
#include <wiringPi.h>
//#include <general/motor.hpp>

using namespace std;
using namespace Util;
using namespace Spcl;
using long_task_result_t = string;
#define CHECK_NOW chrono::milliseconds(0)

//開放を自動化するためにスマートポインタで実装する。
shared_ptr<Argument> argument{nullptr};
shared_ptr<Settings> setting{nullptr};
shared_ptr<Report> report{nullptr};
shared_ptr<GamePad> gamepad{nullptr};
shared_ptr<DeviceManager> device_manager{nullptr};
//shared_ptr<DeviceManager> device_manager2{nullptr};
vector< future<long_task_result_t> >results;

//motor_sptr tire_flont_right{nullptr},tire_flont_left{nullptr},tire_back_right{nullptr},tire_back_left{nullptr};
//motor_manager_sptr motor_manager{nullptr};

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
#if 0
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
      device_manager =
        move(DeviceManager::GenerateDeviceManager(serial_location->c_str(),stoi(band)));
      //device_manager2 =
      //  move(DeviceManager::GenerateDeviceManager(serial_location->c_str(),stoi(band)));
    } else {
        report->Warn(ReportGroup::GamePad, "Missing Serial Location");
    }
#endif
    #if 0
    //モーターのアドレス設定
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
    if(wiringPiSetup()!=0){
      report->Warn(ReportGroup::WiringPi, "error wiringPi setup");
    }else{
      report->Info(ReportGroup::WiringPi, "success WiringPi setup");
    }
    Sonic sonic_one(4);
    thread t1(sonic_one);
    // MessageLoop
    signal(SIGINT, singal_receiver);

    for (is_continue = true; is_continue;) {

      /*
      for(automatic* it=new FirstMove();it!=nullptr;){
        (*it)();
        automatic* next=it->next();
        if(next!=it){
          delete it;
        }
      }*/
      while(getchar() != '\n');
      cout << sonic_one.gettime() << endl;
      delay(1000);
    }
    //t1.join();
    report->Info(ReportGroup::System, "Shutdown");
    return 0;
}

void ShortTask(){
  if (gamepad) {
      gamepad->Update();
      cout << gamepad->Status();  //確認用
  }
}

void LongTaskBefore(auto func){
  results.push_back(async(launch::async, func));
}

long_task_result_t LongTaskAfter(){
  long_task_result_t temp = results.front().get();
  results.erase(results.begin());
  return temp;
}

#endif
