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

void DeviceManager::Flush() { // ???
    for (auto& device : devices) {
        if (!(device->cmd.empty())) {
            Select(device->address);
            // TODO 応答を待つ
            while (!(device->cmd.empty())) {
                WriteSerial(device->cmd.front());
                // TODO 応答を待つ

                device->cmd.pop();
            }
        }
    }
}

DeviceBase::DeviceBase() {
    cmd.push(Command::feature);
}

DeviceBase::~DeviceBase() {}

void DeviceBase::PushCommand(string str) {
    cmd.push(str + Command::newline);
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

void DeviceMotor::Synchronize() {
    parent->Flush();
    PushCommand(Command::sync);
    parent->Flush();
    // parent->WriteSerial(ss.str());
}

void DeviceMotor::Duty(float value) {
    stringstream ss;
    ss << Command::duty << Command::space << value;
    PushCommand(ss.str());
}
