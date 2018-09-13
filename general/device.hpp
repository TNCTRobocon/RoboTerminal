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


template <typename... T>
std::vector<factor_t> FactorAND(T... vectors){
  int count = 0;
  std::unordered_set<factor_t> survivers;
  for(const auto& factors : std::initializer_list<std::vector<factor_t>>{vectors...}){
    if(count == 0){
      for(const auto& factor : factors){ //一個目のベクタの要素を全て生存者とする
        survivers.emplace(factor);
      }
    }
    else{//２個目以降は
      std::unordered_set<factor_t> temporary;
      for(const auto& factor : factors){
        temporary.emplace(factor);
      }
      for(auto& factor : survivers){
        if(temporary.find(factor) == temporary.end()){//生存者のうちｎ個目のベクタの要素に含まれないものは
          survivers.erase(factor);//削除される
        }
      }
    }
    count++;
  }
  std::vector<factor_t> AND_result;
  for(const auto& surviver : survivers){//最後まで生き残った要素をすべてベクタに格納し、返す
    AND_result.push_back(surviver);
  }
  return AND_result;
}

class DeviceManager {
private:
    boost::asio::io_service io;
    boost::asio::serial_port serial;  //まだserialportは開かれていない
    std::map<address_t, std::weak_ptr<DeviceBase>> devices_address;
    std::multimap<factor_t, std::weak_ptr<DeviceBase>> devices_feature;
    std::queue<std::function<void()>> command;

    std::optional<std::string> ReadSerial();
    void WriteSerial(const std::string&);

public:
    DeviceManager(std::string filename, int rate);
    ~DeviceManager();


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
    std::unordered_set<factor_t> feature;
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
};

struct DeviceMotor::CreateHelper{
  DeviceMotor x;
  template<class... Args>
  explicit CreateHelper(Args&&... args) : x(std::forward<Args>(args)...) {}
};

std::shared_ptr<DeviceMotor> DeviceMotor::CreateMotor(std::shared_ptr<DeviceManager> p, address_t a){
  //アドレスが本当に新しいか確認
  if(p->GetAddressMap().count(a) == 0){
    //新しいアドレスでMotorへのポインタを生成する
    auto dummy_sptr = std::make_shared<CreateHelper>(p,a);
    auto new_sptr = std::shared_ptr<DeviceMotor>(std::move(dummy_sptr), &dummy_sptr->x);
    p->CacheAddress(a, new_sptr);//キャッシュを残す
    return new_sptr;
  }
  else{//そのアドレスは既に埋まっているならば
      //TODO? エラーレポート　address XX is already taken
    return nullptr;
  }
}

struct DeviceSolenoid::CreateHelper{
  DeviceSolenoid x;
  template<class... Args>
  explicit CreateHelper(Args&&... args) : x(std::forward<Args>(args)...) {}
};

std::shared_ptr<DeviceSolenoid> DeviceSolenoid::CreateSolenoid(std::shared_ptr<DeviceManager> p, address_t a){
  //アドレスが本当に新しいか確認
  if(p->GetAddressMap().count(a) == 0){
    //新しいアドレスでSolenoidへのポインタを生成する
    auto dummy_sptr = std::make_shared<CreateHelper>(p,a);
    auto new_sptr = std::shared_ptr<DeviceSolenoid>(std::move(dummy_sptr), &dummy_sptr->x);
    p->CacheAddress(a, new_sptr);//キャッシュを残す
    return new_sptr;
  }
  else{//そのアドレスは既に埋まっているならば
      //TODO? エラーレポート　address XX is already taken
    return nullptr;
  }
}
