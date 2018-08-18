#pragma once
#include <memory>
#include <stdint.h>
#include <string>

#include <boost/asio.hpp>

#include <vector>

class Motor;
using address_t = uint32_t;
using motor_wptr= std::weak_ptr<Motor>;
using motor_uptr= std::unique_ptr<Motor>;
using motor_sptr= std::shared_ptr<Motor>;
class MotorManager;
using motor_manager_sptr= std::shared_ptr<MotorManager>;

class MotorManager{
private:
    boost::asio::io_service io;//int fd;//シリアルポートのファイル識別時
    boost::asio::serial_port serial;
    MotorManager(const char* filename,int rate);
    std::vector<motor_sptr> motors;
public:
    static inline std::unique_ptr<MotorManager> GenerateMotorManeger(const char *filename,int rate){
		return std::unique_ptr<MotorManager>(new MotorManager (filename,rate));
}

    virtual ~MotorManager();

    motor_sptr CreateMotor(address_t);//モーターを生成する
    //void Remove(address_t);//モーターを消去する。
    //motor_sptr GetMotor(address_t);//生成済みのモーターを取得する

    void Write(const std::string&);
	void Command(const std::string&);
	void Synchronize();
	void Reset();
};

class IMotorCommon{
public:
	IMotorCommon(){}
	virtual ~IMotorCommon(){}
	virtual void Duty(float value)=0;
	virtual void Stop()=0;
};

class IMotorAdvanced{
public:
	IMotorAdvanced(){}
	virtual ~IMotorAdvanced(){}
	virtual void RPM(float rpm)=0;
};

class Motor:public IMotorAdvanced,public IMotorCommon{
    friend class MotorManager;
private:
    MotorManager* parent;
    address_t address;
public:
    Motor( MotorManager* ptr,address_t);
    virtual ~Motor(){}
    Motor(const Motor&)=delete;
    void Select();
    virtual void Duty(float value);
    void AsyncRPM(float rpm);
    virtual void RPM(float rpm);
    virtual void  Stop();
    bool operator <(const Motor& cmp)const{
        return address<cmp.address;
    }
    address_t GetAddr()const{return address;}
};
