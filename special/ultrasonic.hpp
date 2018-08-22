#pragma once
/*
	module : 28015 REV B
*/
#include <mutex>

extern std::mutex mtxt;
extern std::mutex mtxp;

class Sonic{
private:
	int pin_num;
	bool end_flag{true};
	double time{0};
	//std::optional<double> time{std::nullopt}; /////
public:
	Sonic(int);
	void sonicend();
	void operator()();
	inline double gettime(){
		std::lock_guard<std::mutex> lock(mtxt);
		return time;
	}
};
