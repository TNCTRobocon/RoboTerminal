#pragma once

#include <general/gamepad.hpp>
#include <general/motor.hpp>
#include <memory>
#include <util/argument.hpp>
#include <util/report.hpp>
#include <util/settings.hpp>
#include <special/ultrasonic.hpp>
#include <special/servo.hpp>
#include <special/limitswitch.hpp>

extern std::shared_ptr<Util::Argument> argument;
extern std::shared_ptr<Util::Settings> setting;
extern std::shared_ptr<Util::Report> report;
extern std::shared_ptr<GamePad> gamepad;

//extern motor_sptr tire_flont_right,tire_flont_left,tire_back_right,tire_back_left;
