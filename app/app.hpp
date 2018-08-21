#pragma once

#include <memory>
#include "argument.hpp"
#include "report.hpp"
#include "settings.hpp"
#include <general/gamepad.hpp>

extern std::shared_ptr<Argument> argument;
extern std::shared_ptr<Settings> setting;
extern std::shared_ptr<Report> report;
extern std::shared_ptr<GamePad> gamepad;