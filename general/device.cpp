#include "device.hpp"

#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

namespace Command {
const string newline = "\r";
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
        cout << "failed to open serialport : FILE NAME ?" << endl;
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

string DeviceManager::ReadSerial() {  //必ず'\r'で終わる文字列を一つ読み込む
    boost::asio::streambuf buf;
    boost::asio::read_until(serial, buf, '\r');

    // TODO
    // 一定時間経っても応答がない(\rで終わる文字列がこない)場合のプログラムを追加

    string result = boost::asio::buffer_cast<const char*>(
        buf.data());  //バッファの中身を文字列として取り出す
    return result;
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
  //for(:)
  /*
    for (auto& dev : devices) {
        if (!(dev->send.empty())) {
            cmd.push([=] { Select(dev->address); });
            do {
                cmd.push(dev->send.front());
                dev->send.pop();
            } while (!(dev->send.empty()));
        }
    }
    for (auto& dev : devices) {
        if (!(dev->receive.empty())) {
            cmd.push([=] {
                if (ReadSerial() == "GOOD RESPONSE") {
                    /// Statements
                } else {
                    /// Statements
                }
            });
            do {
                cmd.push(dev->receive.front());
                dev->receive.pop();
            } while (!(dev->receive.empty()));
        }
    }
  */
}

void DeviceManager::Flush() { // 未完成
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
      [=] (string response){
          if (Feature(response)) {
              cout << "Feature success" << endl;
          } else {
              /// Statements
          }
      }
    );
}

DeviceBase::~DeviceBase() {}

void DeviceBase::PushCommand(string to_send, function<void(string)> response_checker) {
    // cmd.push(str + Command::newline);
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
        // TODO ftによる検索のためのmultimapへ登録
        if (last != str.end()) {
            ++last;
        }
        first = last;
    }
}

bool DeviceBase::Feature(string response) {
    if (!response.empty()) {
        if (response.back() == '\r') {
            response.pop_back();
            ReadCSV(response);
            return true;
        }
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
        [](string response){
            if (response == "GOOD RESPONSE") {
                /// Statements
            } else {
                /// Statements
            }
        }
    );
}
