#include "app.hpp"
#include <util/argument.hpp>
#include <util/state_graph.hpp>
#include <util/settings.hpp>
#include <util/report.hpp>
using namespace std;
using namespace Util;
using namespace UI;
using namespace General;

std::optional<Application> app{std::nullopt};

Application::Application(int* argc, char*** argv) {
    // System全体で使う変数を初期化する
    Argument::Initialize(*argc,*argv);
    auto report = GetReport();
    report->Info(ReportGroup::System, "Wake Up");
    auto setting = Setting::GetCommon();
    // Windowsを初期化する
    window.reset(new Window(argc, argv));
    report->Info(ReportGroup::GUI, "UI Wake Up");
    //ゲームパッドを初期化する
    auto gamepad_location = setting->Read("gamepad");
    if (gamepad_location) {
        gamepad.reset(new GamePad(*gamepad_location));
    } else {
        report->Warn(ReportGroup::GamePad, "Missing GamePad Location");
    }
    //シリアルポートを初期化する
    auto serial_location = setting->Read("serial");
    if (serial_location) {
        string band_text = setting->Read("serial-band").value_or("115200");
        int band = stoi(band_text);
        device_manager.reset(new DeviceManager(*serial_location, band));
    } else {
        report->Warn(ReportGroup::GamePad, "Missing GamePad Location");
    }
#ifdef RASPBERRY_PI
    // specialを関する記述

#endif
}

Application::~Application() {
    GetReport()->Info(ReportGroup::System, "Shutdown");
}

bool Application::Process() {
    if (window->Process()) {
        if (gamepad) {
            gamepad->Update();
        }
        return true;
    }
    return false;
}

int main(int argc, char** argv) {
    //app.emplace(&argc, &argv);
    //while (app->Process())
        ;
    auto a=StateNode::Create(AlwaysNone(),"a");
    auto b=StateNode::Create(AlwaysNone(),"b");
    auto g=StateGraph::Create();
    g->Insert(StateEdge::Create(nullptr,a,AlwaysTrue(),"-a"));
    g->Insert(StateEdge::Create(a,b,AlwaysTrue(),"ab"));
    g->Insert(StateEdge::Create(b,nullptr,AlwaysTrue(),"b-"));
    g->Move(a);
    cout<<g->ToPlant();
    return 0;
}
