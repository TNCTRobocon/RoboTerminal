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
const string open = "opn";
const string close = "cls";
}

DeviceManager::DeviceManager(string filename, int rate) : serial(io) {
    try {
        serial.open(filename);
    } catch (...) {
        //report -> Error(ReportGroup::SerialPort, "Failed to Open SerialPort");
        // strange error "ReportGroup has not been declared"
    }
    cout << "Serialport opened successfully" << endl;
    serial.set_option(
        boost::asio::serial_port_base::baud_rate(rate));  //ボーレート設
}

DeviceManager::~DeviceManager() {
    serial.close();
    cout << "Serialport closed" << endl;
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
    const string& text) {  //渡された文字に'\r'をつけて送信
    boost::asio::write(serial, boost::asio::buffer(text+Command::newline));
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
      if(!result.empty()) //空ではありえないはずだが、念の為
        result.pop_back(); //末尾'\r'を削除
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
    ss << Command::select << Command::space << address;  //例:ss="sel 39"
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
  PushCommand(Command::feature,
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

bool DeviceBase::Feature(optional<string> response) {//
  if(response){
    ReadCSV(*response);
    cout << "Feature receive successfully from Device" << address << endl;
    return true;
  }
  else{
    cout << "Failed to get Feature from Device" << address << endl;
    cout << "(No Response)" << endl;
    return false;
  }
}

void DeviceBase::Echo(string str){
  PushCommand(str,
    [=](optional<string> response){
      cout << "[ ECHO TEST ]" << endl;
      cout << "<--  Input: " << str << endl;
      if(response){
        //cout << "--> Output: " << response << endl;
        if(response == str){
          //cout << "Device " << address << "is responding properly." << endl;
        }
        else{
          //cout << "Device " << address << "is NOT responding properly." << endl;
        }
      }
      else{
        //cout << "Device " << address << "is NOT responding!" << endl;
      }
    }
  );
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
  ss << Command::duty << Command::space << value;
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

DeviceSolenoid::DeviceSolenoid(DeviceManager* p, address_t a){
    parent = p;
    address = a;
}

void DeviceSolenoid::Open(int id){
  stringstream ss;
  ss << Command::open << Command::space << id;
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

void DeviceSolenoid::Close(int id){
  stringstream ss;
  ss << Command::close << Command::space << id;
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

void DeviceSolenoid::Open(){
  Open(0);
}

void DeviceSolenoid::Close(){
  Close(0);
}
