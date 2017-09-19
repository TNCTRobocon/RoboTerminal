#pragma once
#include <memory>
#include <stdint.h>
#include <string>



class Motor;
using address_t = uint32_t;
//void dtm(int,float);
//void mcm(int,int);

class MotorManager{
private:
    int fd;//シリアルポートのファイル識別時
	MotorManager(const char* filename,int rate);
public:
	static inline std::unique_ptr<MotorManager> GenerateMotorManegar(const char *filename,int rate){
			return std::unique_ptr<MotorManager>(new MotorManager (filename,rate));
	}
	
    virtual ~MotorManager();
   
    std::unique_ptr<Motor>   GenerateMotor(address_t addr);
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
    Motor(MotorManager* ptr,address_t);
	void Select();
    void Duty(float value);
	void AsyncRPM(float rpm);
	void RPM(float rpm);
	void Stop();
}
