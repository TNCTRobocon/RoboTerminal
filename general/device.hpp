#pragma once

#include <memory>
#include <stdint.h>
#include <string>
#include <unordered_set>
#include <vector>
#include <queue>
#include <functional>
#include <future>
#include <tuple>
#include <map>
//#include <iostream>
//#include <sstream>
//#include <algorithm>

#include <boost/asio.hpp>

class DeviceManager;
class DeviceBase;
class DeviceMotor;

using namespace std;

using address_t = uint32_t;
using factor_t = string;

//future<void> prpr; //TODO right name



class DeviceManager {
    // friend class DeviceBase;
private:
    boost::asio::io_service io;
    boost::asio::serial_port serial;  //まだserialportは開かれていない
    // queue commands ?? in DeviceBase
    // vector<shared_ptr<DeviceBase>> devices;
    map<address_t,weak_ptr<DeviceBase>> devices_adr;
    multimap<factor_t,weak_ptr<DeviceBase>> devices_ft;
    queue<function<void()>> cmd;
    DeviceManager(string filename, int rate);

public:
    static inline unique_ptr<DeviceManager> GenerateDeviceManager(
        string filename,
        int rate) {
        return unique_ptr<DeviceManager>(
            new DeviceManager(filename, rate));
    }
    ~DeviceManager();

    bool CreateMotor(address_t);

    string ReadSerial();
    void WriteSerial(const string&);
    void Select(address_t address);
    void PushCommandDirectly(function<void()> no_sel);
    void Fetch();
    void Flush();
};

class DeviceBase {
    friend class DeviceManager;

private:
    unordered_set<factor_t> ft;
    //queue<function<void()>> send;
    //queue<function<void()>> receive;
    queue<tuple<string,function<void(string)>>> async_task;

protected:
    DeviceManager* parent;
    address_t address;
    DeviceBase();
    ~DeviceBase();
    void PushCommand(string to_send, function<void(string)> response_checker);
    void ReadCSV(string str);
    bool Feature(string response);
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
