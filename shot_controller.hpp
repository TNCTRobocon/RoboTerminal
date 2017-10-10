#pragma once
#include "motor.hpp"

class ShotControl{
	private:
		ShotControl();
	public:
		static inline std::unique_ptr<ShotControl> GenerateShotControl();
		void Shot(auto *shotf,float value);
};


