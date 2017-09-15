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
	MotorManager(const char* filename,int rate);
public:
	static inline MotorManager* GenerateMotorManegar(const char *filename,int rate){
		return new MotorManager(filename,rate);
	}
	
    virtual ~MotorManager();
   
    std::unique_ptr<Motor>   GenerateMotor(address_t addr){
		return new Motor(this addr);
	}
    void Write(const char* str);
	
};


class Motor{
    friend class MotorManager;
private:
    std::shared_ptr<MotorManager> parent;
    address_t address;
private:
    Motor(MotorManager* ptr,address_t);
public:
	void Sel(int id);
    void Duty(float dt_value);
	void MotorControl_rady(int mc_value);
	void MotorControl_go();
};
