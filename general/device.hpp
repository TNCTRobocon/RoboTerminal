#pragma once

#include <memory>
#include <stdint.h>
#include <string>
#include <unordered_set>
#include <vector>
#include <queue>
//#include <iostream>
//#include <sstream>
//#include <algorithm>

#include <boost/asio.hpp>

class DeviceManager;
class DeviceBase;
class DeviceMotor;

using address_t = uint32_t;
using factor_t = std::string;

namespace Command {
const std::string newline = "\r";
const std::string space = " ";
const std::string select = "sel";
const std::string feature = "ft";
const std::string duty = "dt";
const std::string motor_control = "mc";
const std::string async_motor_control = "rady";
const std::string sync = "go";
const std::string angle = "sc";
const std::string reset = "rst";
const std::string stop = "stop";
}

class DeviceManager {
    // friend class DeviceBase;
private:
    boost::asio::io_service io;
    boost::asio::serial_port serial;  //まだserialportは開かれていない
    // std::queue commands ?? in DeviceBase
    std::vector<std::shared_ptr<DeviceBase>> devices;
    DeviceManager(std::string filename, int rate);

public:
    static inline std::unique_ptr<DeviceManager> GenerateDeviceManager(
        std::string filename,
        int rate) {
        return std::unique_ptr<DeviceManager>(
            new DeviceManager(filename, rate));
    }
    ~DeviceManager();

    std::shared_ptr<DeviceMotor> CreateMotor(address_t);

    std::string ReadSerial();
    void WriteSerial(const std::string&);
    void Select(address_t address);
    void Flush();
};

class DeviceBase {
    friend class DeviceManager;

private:
    std::unordered_set<factor_t> ft;
    std::queue<std::string> cmd;

protected:
    DeviceManager* parent;
    address_t address;
    DeviceBase();
    ~DeviceBase();
    void PushCommand(std::string str);
    void Read_csv(std::string str);
    bool Feature(std::string response);
    //??? void Flush();
    // void Select
};

class DeviceMotor : public DeviceBase {
private:
public:
    DeviceMotor(DeviceManager* p, address_t a);
    ~DeviceMotor();
    void Synchronize();
    void Duty(float value);
    void AsyncRPM(float rpm);
    void RPM(float rpm);
    void Stop();
};
