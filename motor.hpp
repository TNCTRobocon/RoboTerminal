#pragma once
#include <memory>
#include <stdint.h>
#include <string>

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringSerial.h>


class Motor;
using address_t = uint32_t;


class MotorManager{
private:
    int fd;//シリアルポートのファイル識別時
    MotorManager(const char* filename,int rate);
public:
    static inline std::unique_ptr<MotorManager> GenerateMotorManeger(const char *filename,int rate){
		return std::unique_ptr<MotorManager>(new MotorManager (filename,rate));
	}
	
    virtual ~MotorManager();
   
    std::unique_ptr<Motor> GenerateMotor(address_t addr);/*{
		return std::unique_ptr<Motor>(new Motor(this,addr));
	}*/
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
	virtual ~IMotorAdvanced();
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
};
