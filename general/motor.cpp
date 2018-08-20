#include "motor.hpp"
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

MotorManager::MotorManager(const char* filename, int rate)
    : serial(io, filename) {
  serial.set_option(boost::asio::serial_port_base::baud_rate(rate));
  printf("serialport opened successfully\n");
  Command(cmd_reset);
}

MotorManager::~MotorManager() {
  serial.close();
  printf("serialport closed");
}

motor_sptr MotorManager::CreateMotor(address_t addr) {
  //自分のコレクションに存在しているか?
  for (auto& it : motors) {
    if (addr == it->GetAddr()) {
      return it;
    }
  }
  //ないので生成する。
  motor_sptr sptr = std::make_shared<Motor>(this, addr);
  motors.push_back(sptr);
  return sptr;
}

void MotorManager::Write(const std::string& text) {
  boost::asio::write(serial, boost::asio::buffer(text));
}

void MotorManager::Command(const std::string& command) {
  boost::asio::write(serial, boost::asio::buffer(command));
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
