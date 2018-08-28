#include "device.hpp"

#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

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

shared_ptr<DeviceMotor> DeviceManager::CreateMotor(address_t new_address) {
    for (auto& it : devices) {  //アドレスが本当に新しいか確認
        if (new_address ==
            it->address) {  //そのアドレスは既に埋まっているならば
            return nullptr;  // devicesの中のそのアドレスの要素を返す
        }
    }
    //新しいアドレスで生成する
    shared_ptr<DeviceMotor> new_sptr =
        make_shared<DeviceMotor>(this, new_address);
    devices.push_back(new_sptr);
    return new_sptr;
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

void DeviceManager::PushCommandDirectly(std::function<void()> no_sel){
  cmd.push(no_sel);
}

void DeviceManager::Fetch() {
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
}

void DeviceManager::Flush() { // 未完成
  progress = async(launch::async,
    [=]{
      while(!cmd.empty()){

      }
    }
  );
}

DeviceBase::DeviceBase() { //デバイスのインスタンスを生成時、sel XX と ft を送る
  PushCommand(
      [=] {
          parent->WriteSerial(Command::feature + Command::newline);
      },
      [=] {
          if (Feature(parent->ReadSerial())) {
              cout << "Feature success" << endl;
          } else {
              /// Statements
          }
      }
    );
}

DeviceBase::~DeviceBase() {}

void DeviceBase::PushCommand(std::function<void()> before,
                             std::function<void()> after) {
    // cmd.push(str + Command::newline);
    send.push(before);
    receive.push(after);
}

void DeviceBase::Read_csv(string str) {
    ft.clear();
    auto first = str.begin();
    while (first != str.end()) {
        auto last = first;
        while (last != str.end() && *last != ',') {
            ++last;
        }
        ft.emplace(std::string(first, last));
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
            Read_csv(response);
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
}

void DeviceMotor::Duty(float value) {
    PushCommand(
        [=] {
            stringstream ss;
            ss << Command::duty << Command::space << value << Command::newline;
            parent->WriteSerial(ss.str());
        },
        [=] {
            if (parent->ReadSerial() == "GOOD RESPONSE") {
                /// Statements
            } else {
                /// Statements
            }
        });
}
