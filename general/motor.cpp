#include "motor.hpp"
#include "feature.hpp"
#include <stdio.h>

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <boost/asio.hpp>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;
//モーターのコマンド
const static string cmd_newline = "\r";
const static string cmd_space = " ";

const static string cmd_select = "sel";
const static string cmd_duty = "dt";
const static string cmd_motor_control = "mc";
const static string cmd_async_motor_control = "rady";
const static string cmd_sync = "go";
const static string cmd_angle = "sc";
const static string cmd_reset = "rst";
const static string cmd_stop = "stop";
const static string cmd_feature = "ft";

MotorManager::MotorManager(Feature* ptr, const char* filename, int rate) :
master(ptr), serial(io, filename){
  //ここでserialportを開けない場合、boostが自動で例外を投げてプログラムを中断
  printf("serialport opened successfully\n");
  serial.set_option(boost::asio::serial_port_base::baud_rate(rate)); //ボーレート設定
  Command(cmd_reset);
}

MotorManager::~MotorManager() {
    serial.close();
    printf("serialport closed");
}

motor_sptr MotorManager::CreateMotor(address_t addr) {
    //自分のコレクションに存在しているか?
    for (auto &it:motors){
        if (addr==it->address){
            return it;
        }
    }
    //ないので生成する。
    motor_sptr sptr = std::make_shared<Motor>(this, addr);
    motors.push_back(sptr);
    return sptr;
}

void MotorManager::Write(const std::string& text) { //渡された文字をそのまま送信
    boost::asio::write(serial, boost::asio::buffer(text));
}

void MotorManager::Command(const std::string& command) { //各種コマンドから呼ばれる。指定したconst文字列+delimiterを送信
    boost::asio::write(serial, boost::asio::buffer(command+cmd_newline));//overload演算子'+'は文字列の結合
}

string MotorManager::Read() { //必ず'\r'で終わる文字列を一つ読み込む
    boost::asio::streambuf buf;
    boost::asio::read_until(serial, buf, '\r');
    string result = boost::asio::buffer_cast<const char*>(buf.data());//バッファの中身を文字列として取り出す
    return result;
}

void MotorManager::Synchronize() {
    Command(cmd_sync);
}

/*
void MotorManager::Feature() {
    //for ()

}
*/

void Motor::Select() {
    stringstream ss;
    ss << cmd_select << ' ' << address;
    parent->Command(ss.str());
}

void Motor::Duty(float value) {
    Select();
    stringstream ss;
    ss << cmd_duty << ' ' << value;
    parent->Command(ss.str());
}

void Motor::AsyncRPM(float value) {
    Select();
    stringstream ss;
    ss << cmd_async_motor_control << ' ' << value;
    parent->Command(ss.str());
}

void Motor::RPM(float value) {
    Select();
    stringstream ss;
    ss << cmd_motor_control << ' ' << value;
    parent->Command(ss.str());
}

void Motor::Stop() {
    Select();
    parent->Command(cmd_stop);
}

/*
void Motor::Feature() { //TODO MotorManager から実行するよう変更
    Select();
    parent->Command(cmd_feature);
    string response(parent->Read());

}
*/

Motor::Motor(MotorManager* p, address_t adr) {
    parent = p;
    address = adr;
}
