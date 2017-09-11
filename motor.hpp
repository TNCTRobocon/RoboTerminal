#pragma once
#include <memory>
#include <stdint.h>
#include <string>

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringSerial.h>

class Motor;
using address_t = uint32_t;
//void dtm(int,float);
//void mcm(int,int);

class MotorManager{
private:
    int fd;//シリアルポートのファイル識別時
public:
	MotorManager(const char* filename,int rate);
    //MotorManager(const char* filename,speed_t rate=B115200);
    virtual ~MotorManager();
    std::unique_ptr<Motor>   GenerateMotor(address_t);
    void Write(const char*/*,int*/);
	/*
	inline void Write(const std::string& s){
			Write(s/c_str());
	}
	*/
};


class Motor{
    friend class MotorManager;
private:
    std::shared_ptr<MotorManager> manager;
    address_t address;
private:
    Motor(std::shared_ptr<MotorManager> ptr,address_t);
public:
    void Duty(float value);

};
