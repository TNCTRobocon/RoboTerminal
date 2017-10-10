#pragma once

#include <memory>
#include <math.h>
class MecanumWheel;
using mwheel_uptr=std::unique_ptr<MecanumWheel>;


class MecanumWheel {
    double alpha; //モーターが設置された角度[rad]
    double beta; //モーターの進む向き[rad]
    double length; //中心からの距離[mm](必ず正)
    double gamma{1}; //モータの補正値[無次元]
    double size;//タイヤの半径[mm]
public:
    MecanumWheel(double a, double b=M_PI_2, double l=1.0, double g=1.0,double s=1.0);
    MecanumWheel(const MecanumWheel&) ;
    virtual ~MecanumWheel() {}
    double Calculate(double x, double y, double r);

    double operator()(double x, double y, double r)
    {
        return Calculate(x, y, r);
    }

    double GetAlpha()const
    {
        return alpha;
    }

    double GetBeta()const
    {
        return beta;
    }

    double GetLength()const
    {
        return length;
    }

    double GetGamma()const
    {
        return gamma;
    }

    void SetLength(double x);

};

