#pragma once
#include <pch.hpp>
#include <future>
#include <queue>
#include <utility>
#include <boost/asio.hpp>
#include <boost/type.hpp>

namespace General {
using address_t = uint32_t;
using factor_t = std::string;

#define SERIAL_TIME_LIMIT (chrono::milliseconds(50))

// future<void> prpr; //TODO right name
class DeviceManager;
class DeviceBase;
class DeviceMotor;
class DeviceSolenoid;

class DeviceManager {
private:
    boost::asio::io_service io;
    boost::asio::serial_port serial;  //まだserialportは開かれていない
    std::unordered_map<address_t, std::weak_ptr<DeviceBase>> devices_adr;
    std::unordered_multimap<factor_t, std::weak_ptr<DeviceBase>> devices_ft;
    std::queue<std::function<void()>> cmd;

    std::optional<std::string> ReadSerial();
    void WriteSerial(const std::string&);

public:
    DeviceManager(std::string filename, int rate);
    ~DeviceManager();

    bool CreateMotor(address_t);
    bool CreateSolenoid(address_t);

    void SetFeature(address_t adr, factor_t fac);
    void Select(address_t address);
    // void PushCommandDirectly(function<void()> no_sel);
    void Fetch();
    void Flush(std::future<void>& task);
};

class DeviceBase {
    friend class DeviceManager;

private:
    std::unordered_set<factor_t> ft;
    std::queue<std::tuple<std::string,
                          std::function<void(std::optional<std::string>)>>>
        async_task;

protected:
    DeviceManager* parent;
    address_t address;
    DeviceBase();
    ~DeviceBase();
    void PushCommand(
        std::string to_send,
        std::function<void(std::optional<std::string>)> response_checker);
    void ReadCSV(std::string str);
    bool Feature(std::optional<std::string> response);
    void Echo(std::string str);
    void Reset();
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

class DeviceSolenoid : public DeviceBase {
private:
public:
    DeviceSolenoid(DeviceManager* p, address_t a);
    ~DeviceSolenoid();
    void Open(int id);
    void Open();
    void Close(int id);
    void Close();
};
}