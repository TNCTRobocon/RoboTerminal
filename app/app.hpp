#pragma once
#include <general/gamepad.hpp>
#include <general/device.hpp>
#include <memory>
#include <optional>

#include <util/argument.hpp>
#include <util/report.hpp>
#include <util/settings.hpp>
#include <ui/ui.hpp>

struct Application final {
    // field
    std::shared_ptr<Util::Argument> argument{nullptr};
    std::shared_ptr<Util::Settings> setting{nullptr};
    std::shared_ptr<Util::Report> report{nullptr};
    std::shared_ptr<GamePad> gamepad{nullptr};
    std::shared_ptr<UI::Window> window{nullptr};
    std::shared_ptr<DeviceManager> device_manager{nullptr};
    Application(int* argc, char*** argv);
    ~Application();
    bool Process();
};

extern std::optional<Application> app;
