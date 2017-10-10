#pragma once
#include <vector>

class MecanumWheel{
	double alpha;//モーターが設置された角度[rad]
  	double beta;//モーターの進む向き[rad]
  	double length;//中心からの距離[mm](必ず正)
  	double gamma{1};//モータの補正値[無次元]
public:
	MecanumWheel(double a,double b,double l,double g);
	MecanumWheel(const MecanumWheel&)=default;
	virtual ~MecanumWheel()=default;
	double Calculate(double x,double y,double r);
	double operator ()(double x,double y,double r){
		return Calculate(x,y,r);
	}
	double GetAlpha()const{
		return alpha;
	}
	double GetBeta()const{
		return beta;
	}

	double GetLength()const{
		return length;
	}

	double GetGamma()const{
		return gamma;
	}

	void SetLength(double x);


};
