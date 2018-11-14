#include "app.hpp"
#include <util/argument.hpp>
#include <util/state_graph.hpp>
#include <util/settings.hpp>
#include <util/report.hpp>
#include <general/gamepad.hpp>
using namespace std;
using namespace Util;
using namespace UI;
using namespace General;

std::optional<Application> app{std::nullopt};

Application::Application(int* argc, char*** argv) {
    // System全体で使う変数を初期化する
    Argument::Initialize(*argc, *argv);
    auto report = GetReport();
    report->Info(ReportGroup::System, "Wake Up");
    auto setting = Setting::GetCommon();
    // Windowsを初期化する
    window.reset(new Window(argc, argv));
    report->Info(ReportGroup::GUI, "UI Wake Up");
    //ゲームパッドを初期化する
    InitilizeDefaultPad();
    //シリアルポートを初期化する
/*    auto serial_location = setting->Read("serial");
    if (serial_location) {
        string band_text = setting->Read("serial-band").value_or("115200");
        int band = stoi(band_text);
        device_manager.reset(new DeviceManager(*serial_location, band));
    } else {
        report->Warn(ReportGroup::GamePad, "Missing GamePad Location");
    }*/
#ifdef RASPBERRY_PI
// specialを関する記述

#endif
}

Application::~Application() {
    GetReport()->Info(ReportGroup::System, "Shutdown");
}

bool Application::Process() {
    if (window->Process()) {
        if (auto gamepad = GetDefaultPad(); gamepad) {
            gamepad->Update();
        }
        return true;
    }
    return false;
}

int main(int argc, char** argv) {

    printf("a");
    #ifdef UNIT_TEST
    printf("u");
    #endif
    return 0;
}
