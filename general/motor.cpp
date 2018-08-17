#include "motor.hpp"
#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

///RasPi Only
//#include <wiringPi.h>
//#include <wiringPiI2C.h>
//#include <wiringSerial.h>

#include <boost/asio.hpp>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

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

MotorManager::MotorManager(const char* filename, int rate) : serial(io, filename){
  //boost::asio::io_service io;
  //boost::asio::serial_port serial(io, filename); // TODO シリアルポートを開くためのファイル名を正しく設定
  serial.set_option(boost::asio::serial_port_base::baud_rate(rate));
  printf("serialport opened successfully\n");
  Command(cmd_reset);


  /* RasPi Only Version
    fd = serialOpen(filename, rate);
    if (fd < 0) {
        printf("can not open serialport\n");
        exit(1);
    } else {
        printf("success open serialport\n");
        Command(cmd_reset);
    }
  */
}

MotorManager::~MotorManager() {
  serial.close();
  printf("serialport closed");
  /*
    if (fd >= 0) {
        //swap config
        serialClose(fd);
        printf("serial port closed");
    }
  */
}

motor_sptr MotorManager::CreateMotor(address_t addr){
    //自分のコレクションに存在しているか?
    for (auto &it:motors){
        if (addr==it->GetAddr()){
            return it;
        }
    }
    //ないので生成する。
    motor_sptr sptr = std::make_shared<Motor>(this,addr);
    motors.push_back(sptr);
    return sptr;
}

void MotorManager::Write(const std::string& text) {
    boost::asio::write(serial, boost::asio::buffer(text));
    //serialPrintf(fd, text.c_str());
}

void MotorManager::Command(const std::string& command) {
  boost::asio::write(serial, boost::asio::buffer(command));

  /* RasPi Only
    serialPrintf(fd, command.c_str());
    //serialPrintf(fd,cmd_newline);
  */
}

void MotorManager::Synchronize() {
    Command(cmd_sync);
}

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

Motor::Motor(MotorManager* p, address_t adr) {
    parent = p;
    address = adr;
}
