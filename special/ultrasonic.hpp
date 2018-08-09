#pragma once
/*
	module : 28015 REV B
*/
#include <mutex>

using namespace std;
extern mutex mtxt;
extern mutex mtxp;

class Sonic{
private:
	int pin_num;
	bool end_flag{true};
	double time{0};
public:
	Sonic(int);
	void sonicend();
	void operator()();
	inline double gettime(){
		lock_guard<mutex> lock(mtxt);
		return time;
	}
};
