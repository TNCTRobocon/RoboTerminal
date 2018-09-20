#pragma once
#include <pch.hpp>
#include <general/device.hpp>
#include <ui/ui.hpp>

struct Application final {
    // field
    std::shared_ptr<UI::Window> window{nullptr};
    std::shared_ptr<General::DeviceManager> device_manager{nullptr};
    Application(int* argc, char*** argv);
    ~Application();
    bool Process();
};

extern std::optional<Application> app;
