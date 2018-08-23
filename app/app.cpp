#include "app.hpp"
#include <signal.h>
#include <general/motor.hpp>
#include <iostream>
// TODO あとで消す
#include <gtk/gtk.h>

using namespace std;
using namespace Util;
//開放を自動化するためにスマートポインタで実装する。
shared_ptr<Argument> argument{nullptr};
shared_ptr<Settings> setting{nullptr};
shared_ptr<Report> report{nullptr};
shared_ptr<GamePad> gamepad{nullptr};

static bool volatile is_continue{false};
static void singal_receiver(int num) {
    cout << endl;
    is_continue = false;
}

int main(int argc, char** argv) {
    // System全体で使う変数を初期化する
    argument.reset(new Argument(argc, argv));
    report.reset(new Report("report.log"));
    report->Info(ReportGroup::System, "Wake Up");
    setting.reset(new Settings("setting.config"));
    //ゲームパッドを初期化する
    auto gamepad_location = setting->Read("gamepad");
    if (gamepad_location) {
        gamepad.reset(new GamePad(*gamepad_location));
    } else {
        report->Warn(ReportGroup::GamePad, "Missing GamePad Location");
    }
    //シリアルポートを初期化する
    auto serial_location = setting->Read("serial");
    auto band = setting->Read("serial-band").value_or("115200");
    if (serial_location) {
        MotorManager::GenerateMotorManeger(serial_location->c_str(),
                                           stoi(band));
    } else {
        report->Warn(ReportGroup::GamePad, "Missing GamePad Location");
    }

    // Windowsを初期化する
    gtk_init(&argc, &argv);
    auto window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(window, 300, 200);
    gtk_widget_show_all(window);
    // MessageLoop

    signal(SIGINT, singal_receiver);
    /*
        for (is_continue = true; is_continue;) {
            if (gamepad) {
                gamepad->Update();
                // cout << gamepad->Status();//確認用
            }

        }
    */
    gtk_main();
    report->Info(ReportGroup::System, "Shutdown");
    return 0;
}
