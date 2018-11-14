#pragma once

//用途　このヘッダーファイルには規定となるクラスを定義する。
#include <stdint.h>

namespace General {

class DeviceBase {
public:
    virtual void Reset() = 0;
};
//モーターの最も基本的なクラスに
class MotorBase : public DeviceBase {
public:
    virtual void Stop() = 0;
    virtual void Rate(float) = 0;
};

//回転数を設定できるモーター用
class AdvancedMotorBase:public MotorBase{
public:
    virtual void Spin(float rpm)=0;
    virtual float Speed()=0;
};

//スイッチやLEDを抽象化するときに
class PinBase :public DeviceBase{
public:
    virtual void Write(bool)=0;
    virtual void Set(){
        Write(true);
    }
    virtual void Clear(){
        Write(false);
    }
    virtual bool Read()=0;
    virtual void Toggle(){
        Write(!Read());
    }
};

//アナログ量の検知用
class AnalogBase:public DeviceBase{
public:
    virtual float Sense()=0;
};

}  // namespace General