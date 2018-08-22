#pragma once

#include <general/gamepad.hpp>
#include <memory>
#include <util/argument.hpp>
#include <util/report.hpp>
#include <util/settings.hpp>

extern std::shared_ptr<Util::Argument> argument;
extern std::shared_ptr<Util::Settings> setting;
extern std::shared_ptr<Util::Report> report;
extern std::shared_ptr<GamePad> gamepad;
