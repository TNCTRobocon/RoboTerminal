#pragma once

//#include "app/app.hpp"

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
#include <optional>
#include <utility>
//#include <iostream>
//#include <sstream>
//#include <algorithm>

#include <boost/asio.hpp>
#include <boost/type.hpp>

class DeviceManager;
class DeviceBase;
class DeviceMotor;
class DeviceSolenoid;

using address_t = uint32_t;
using factor_t = std::string;

#define SERIAL_TIME_LIMIT (chrono::milliseconds(50))

// future<void> prpr; //TODO right name

class DeviceManager {
private:
    boost::asio::io_service io;
    boost::asio::serial_port serial;  //まだserialportは開かれていない
    // queue commands ?? in DeviceBase
    // vector<shared_ptr<DeviceBase>> devices;
    std::map<address_t, std::weak_ptr<DeviceBase>> devices_address;
    std::multimap<factor_t, std::weak_ptr<DeviceBase>> devices_feature;
    std::queue<std::function<void()>> command;

    std::optional<std::string> ReadSerial();
    void WriteSerial(const std::string&);

public:
  /* Singleton version
    static inline unique_ptr<DeviceManager> GenerateDeviceManager(
        string filename,
        int rate) {
        return unique_ptr<DeviceManager>(new DeviceManager(filename, rate));
    }
  */
    DeviceManager(std::string filename, int rate);
    ~DeviceManager();

    //bool CreateMotor(address_t);
    //bool CreateSolenoid(address_t);

    std::map<address_t, std::weak_ptr<DeviceBase>> GetAddressMap()
    {return devices_address;}
    std::multimap<factor_t, std::weak_ptr<DeviceBase>> GetFeatureMap()
    {return devices_feature;}

    void CacheAddress(address_t adr, std::weak_ptr<DeviceBase> wptr);
    void CacheFeature(address_t adr, factor_t fac);
    void CacheFeature(factor_t fac, std::weak_ptr<DeviceBase> wptr);
    void Select(address_t address);
    //void PushCommandDirectly(function<void()> no_sel);
    void Fetch();
    void Flush(std::future<void>& task);

    std::vector<std::shared_ptr<DeviceBase>> SearchFeature(factor_t target);
};

class DeviceBase {
    friend class DeviceManager;

private:
    std::unordered_set<factor_t> ft;
    // queue<function<void()>> send;
    // queue<function<void()>> receive;
    std::queue<std::tuple<std::string, std::function<void(std::optional<std::string>)>>> async_task;

protected:
    std::shared_ptr<DeviceManager> parent;
    address_t address;
    DeviceBase();
    ~DeviceBase();
    void PushCommand(std::string to_send, std::function<void(std::optional<std::string>)> response_checker);
    void ReadCSV(std::string str);
    bool Feature(std::optional<std::string> response);

public:
    void Echo(std::string str);
    void Reset();

    //virtual void Duty(float value) = 0;
};

class DeviceMotor : public DeviceBase {
private:
    DeviceMotor(std::shared_ptr<DeviceManager> p, address_t a);
    struct CreateHelper;
public:
    static inline std::shared_ptr<DeviceMotor>
    CreateMotor(std::shared_ptr<DeviceManager> p, address_t a);
    ~DeviceMotor();
    void Synchronize();
    void Duty(float value);
    void AsyncRPM(float rpm);
    void RPM(float rpm);
    void Stop();
};

class DeviceSolenoid : public DeviceBase {
private:
    DeviceSolenoid(std::shared_ptr<DeviceManager> p, address_t a);
    struct CreateHelper;
public:
    static inline std::shared_ptr<DeviceSolenoid>
    CreateSolenoid(std::shared_ptr<DeviceManager> p, address_t a);
    ~DeviceSolenoid();
    void Open(int id);
    void Open();
    void Close(int id);
    void Close();
    //void Duty(float value);
};

struct DeviceMotor::CreateHelper{
  DeviceMotor x;
  template<class... Args>
  explicit CreateHelper(Args&&... args) : x(std::forward<Args>(args)...) {}
};

struct DeviceSolenoid::CreateHelper{
  DeviceSolenoid x;
  template<class... Args>
  explicit CreateHelper(Args&&... args) : x(std::forward<Args>(args)...) {}
};
