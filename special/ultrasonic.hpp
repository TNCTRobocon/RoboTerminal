#pragma once
/*
	module : 28015 REV B
*/
#include <optional>
namespace Spcl{

class Sonic{
private:
	int pin_num;
	//double time{0};
public:
	Sonic(int);
	double operator()();
};

}//namespace Spcl
