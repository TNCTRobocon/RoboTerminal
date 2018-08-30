#include "device.hpp"
#include "util/report.hpp"
#include "app/app.hpp"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <future>

using namespace std;

namespace Command {
const char newline = '\r';
const string space = " ";
const string select = "sel";
const string feature = "ft";
const string duty = "dt";
const string motor_control = "mc";
const string async_motor_control = "rady";
const string sync = "go";
const string angle = "sc";
const string reset = "rst";
const string stop = "stop";
}

DeviceManager::DeviceManager(string filename, int rate) : serial(io) {
    try {
        serial.open(filename);
    } catch (...) {
        //report -> Error(ReportGroup::SerialPort, "Failed to Open SerialPort");
        // strange error "ReportGroup has not been declared"
    }
    cout << "serialport opened successfully" << endl;
    serial.set_option(
        boost::asio::serial_port_base::baud_rate(rate));  //ボーレート設
}

DeviceManager::~DeviceManager() {
    serial.close();
    cout << "serialport closed" << endl;
}

/// デバイスのインスタンスを生成

bool DeviceManager::CreateMotor(address_t new_address) {
    //アドレスが本当に新しいか確認
    if(devices_adr.count(new_address) == 0){
      //新しいアドレスでMotorへのweakポインタを生成する
      shared_ptr<DeviceMotor> new_sptr =
          make_shared<DeviceMotor>(this, new_address);
      devices_adr[new_address] = new_sptr;
      return true;
      //return ;
    }
    else{//そのアドレスは既に埋まっているならば
      //TODO エラーレポート　address XX is already taken
      //return devices_adr[new_address];//devices_adrの中のそのアドレスの要素を返す
      return false;
    }

}

///　デバイスのインスタンスを生成（終）

void DeviceManager::WriteSerial(
    const string& text) {  //渡された文字をそのまま送信
    boost::asio::write(serial, boost::asio::buffer(text));
}

optional<string> DeviceManager::ReadSerial() {  //必ず'\r'で終わる文字列を一つ読み込む
    boost::asio::streambuf buf;
    future<void> timed_task = async(launch::async,
      [&] {boost::asio::read_until(serial, buf, Command::newline);}
    );
    future_status timer = timed_task.wait_for(SERIAL_TIME_LIMIT);
    if(timer != future_status::timeout){
      string result = boost::asio::buffer_cast<const char*>(
          buf.data());  //バッファの中身を文字列として取り出す
      return result;
    }
    else{
      // error serial read timeout
      return nullopt;
    }


}

void DeviceManager::SetFeature(address_t adr, factor_t fac){
  devices_ft.emplace(fac, devices_adr[adr]);
}

void DeviceManager::Select(address_t address) {
    stringstream ss;
    ss << Command::select << Command::space << address
       << Command::newline;  //例:ss="sel 39\r"
    WriteSerial(ss.str());
}

void DeviceManager::PushCommandDirectly(function<void()> no_sel){
  //cmd.push(no_sel);
}

void DeviceManager::Fetch() {
  queue<function<void()>> send;
  queue<function<void()>> receive;
  for(auto& dev : devices_adr){
    if(shared_ptr<DeviceBase> sptr = dev.second.lock()){
      if(!(sptr -> async_task.empty())){
        send.push( [=] { Select(dev.first); } );
        receive.push(
          [=] {
            if (ReadSerial() == "GOOD RESPONSE"){
              ////
            }
            else{
              ////
            }
          }
        );
        do{
          send.push(
            [=] {
              WriteSerial(get<string>(sptr -> async_task.front()));
            }
          );
          receive.push(
            [=] {
              get<function<void(optional<string>)>>(sptr -> async_task.front())(ReadSerial());
            }
          );
          sptr -> async_task.pop();
        }while(!(sptr -> async_task.empty()));
      }
    }
  }
  while(!send.empty()){
    cmd.push(send.front());
    send.pop();
  }
  while(!receive.empty()){
    cmd.push(receive.front());
    receive.pop();
  }
}

void DeviceManager::Flush(future<void>& task) {
  task = async(launch::async,
    [=]{
      while(!cmd.empty()){
        cmd.front()();
        cmd.pop();
      }
    }
  );
  while(!cmd.empty()){
    cmd.pop();
  }
  /*
   = async(launch::async,
    [=]{
      while(!cmd.empty()){

      }
    }
  );
  */
}

DeviceBase::DeviceBase() { //デバイスのインスタンスを生成時、sel XX と ft を送る
  PushCommand(Command::feature + Command::newline,
      [=] (optional<string> response){
          if (Feature(response)) {
              cout << "Feature success" << endl;
          } else {
              /// Statements
          }
      }
    );
}

DeviceBase::~DeviceBase() {
  //DeviceManagerが持つこのDeviceBaseへのweakポインタはここで無効になる
}

void DeviceBase::PushCommand(string to_send, function<void(optional<string>)> response_checker) {
    async_task.push(make_tuple(to_send, response_checker));
}

void DeviceBase::ReadCSV(string str) {
    ft.clear();
    auto first = str.begin();
    while (first != str.end()) {
        auto last = first;
        while (last != str.end() && *last != ',') {
            ++last;
        }
        ft.emplace(string(first, last));
        parent -> SetFeature(address, string(first, last));
        if (last != str.end()) {
            ++last;
        }
        first = last;
    }
}

bool DeviceBase::Feature(optional<string> response) {
  if(response){
    if (!response->empty()) {
        if (response->back() == Command::newline) {
            response->pop_back();
            ReadCSV(*response);
            return true;
        }
    }
    cout << "Failed to Get Feature from Device" << address << endl;
    cout << "(Response NOT Ending With Delimiter"<<endl;
  }
  else{
    cout << "Failed to Get Feature from Device" << address << endl;
    cout << "(No Response)" << endl;
  }
  return false;
}

DeviceMotor::DeviceMotor(DeviceManager* p, address_t a) {
    parent = p;
    address = a;
}

DeviceMotor::~DeviceMotor() {}

void DeviceMotor::Synchronize() { //selを伴わない、 同時に実行させたい範囲の開始を示す特殊な命令
  /*
    parent->Fetch();
    parent->PushCommandDirectly(
      [=]{
        parent->WriteSerial(Command::sync + Command::newline);
      }
    );
    parent->PushCommandDirectly(
      [=]{
        if (parent->ReadSerial() == "GOOD RESPONSE") {
            /// Statements
        } else {
            /// Statements
        }
      }
    );
    // parent->WriteSerial(ss.str());
  */
}

void DeviceMotor::Duty(float value) {
  stringstream ss;
  ss << Command::duty << Command::space << value << Command::newline;
    PushCommand(ss.str(),
        [=](optional<string> response){
            if (response == "GOOD RESPONSE") {
                /// Statements
            } else {
                /// Statements
            }
        }
    );
}
