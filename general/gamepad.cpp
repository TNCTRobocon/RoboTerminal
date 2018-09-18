#include "gamepad.hpp"
#include <fcntl.h>
#include <linux/joystick.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <app/app.hpp>

using namespace std;
using namespace Util;
namespace General{
constexpr size_t GamePad::button_size;
constexpr size_t GamePad::axis_size;

GamePad::GamePad(const string& filename)
    : buttons(button_size), axises(axis_size) {
    fd = open(filename.c_str(), O_RDONLY);
    auto& report = app->report;
    if (fd < 0) {
        string message = "Cannot Open GamePad" + filename;

        if (report) {
            report->Error(ReportGroup::GamePad, message);
        } else {
            cerr << message << endl;
        }
    } else {
        // GamePadの設定
        fcntl(fd, F_SETFL, O_NONBLOCK);  // using non-blocking mode
        //表示
        stringstream ss;
        ss << "Open GamePad:" << filename;
        if (report) {
            report->Info(ReportGroup::GamePad, ss.str());
        } else {
            cout << ss.str();
        }
    }
}

GamePad::~GamePad() {
    auto& report = app->report;
    if (fd >= 0) {
        close(fd);
        string message = "Close GamePad";
        if (report) {
            report->Info(ReportGroup::GamePad, message);
        } else {
            cerr << message << endl;
        }
    }
}

void GamePad::Update() {
    struct js_event event;
    //データがあるなら読み込む
    if (read(fd, &event, sizeof(event)) == sizeof(event)) {
        switch (event.type & ~JS_EVENT_INIT) {
            case JS_EVENT_BUTTON:
                if (event.number < buttons.size()) {
                    buttons[event.number] = event.value;
                }
                break;
            case JS_EVENT_AXIS:
                if (event.number < axises.size()) {
                    axises[event.number] = event.value;
                }
                break;
            default:
                break;
        }
    }
}

std::string GamePad::Status() const {
    stringstream ss;
    //ボタン情報
    {
        auto it = buttons.begin();
        ss << "button[" << buttons.size() << "]:" << *it;
        for (; it != buttons.end(); it++) {
            ss << "," << *it;
        }
    }
    ss << endl;
    {
        //スティック情報
        auto it = axises.begin();
        ss << "axis[" << axises.size() << "]:" << *it;
        for (; it != axises.end(); it++) {
            ss << "," << *it;
        }
    }
    ss << endl;
    return ss.str();
}
}