#include "app.hpp"
#include <util/state_graph.hpp>
using namespace std;
using namespace Util;
using namespace UI;
using namespace General;

std::optional<Application> app{std::nullopt};
static bool volatile is_continue{false};
static void signal_receiver(int num){
  cout << endl;
  is_continue = false;
}

Application::Application(int* argc, char*** argv) {
    // System全体で使う変数を初期化する
    argument.reset(new Argument(*argc, *argv));
    report.reset(new Report("report.log"));
    report->Info(ReportGroup::System, "Wake Up");
    setting.reset(new Settings("setting.config"));
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
        report->Warn(ReportGroup::SerialPort, "Missing SerialPort Location");
    }
#ifdef RASPBERRY_PI
    // specialを関する記述

#endif
}

Application::~Application() {
    report->Info(ReportGroup::System, "Shutdown");
}

bool Application::Process() {
  /*
    if (window->Process()) {
        if (gamepad) {
            gamepad->Update();
        }
        return true;
    }
    return false;
  */
  int adr;
  cout << "device address? : ";
  cin >> adr;
  auto motor0 = DeviceMotor::CreateMotor(device_manager, adr);
  //auto motor1 = DeviceMotor::CreateMotor(device_manager, 19);
  motor0 -> Echo("miku");
  cout << ( device_manager -> Fetch() ) << endl;
  device_manager -> Flush(another_task);

  return true;
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

    signal(SIGINT, signal_receiver);
    debug("before init");
    auto test_app = make_shared<Application>(&argc, &argv);
    test_app -> Process();
    debug("after process");
    is_continue = true;
    while(is_continue){

    }
    /*
    auto timelimit = test_app->long_task.wait_for(chrono::seconds(3));
    if(timelimit != future_status::timeout)
      cout << test_app->long_task.get() << endl;
    else
      cout << "TIMEOUT" << endl;
    */
    return 0;
}
