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


class Motor{
    friend class MotorManager;
private:
    MotorManager* parent;
    address_t address;
public:
    Motor( MotorManager* ptr,address_t);
	void Select();
    void Duty(float value);
	void AsyncRPM(float rpm);
	void RPM(float rpm);
	void Stop();
};
