#include "gamepad.hpp"
#include <fcntl.h>
#include <linux/joystick.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <app/app.hpp>

using namespace std;

GamePad::GamePad(const string& filename) {
    fd = open(filename.c_str(), O_RDONLY);
    if (fd < 0) {
        perror("GamePad Cannot open\n");
        exit(1);
    } else {
        printf("Successed GamePad[%d] Connection\n", fd);
    }
    //ボタン数取得
    char name[80];
    ioctl(fd, JSIOCGBUTTONS, &button_num);
    ioctl(fd, JSIOCGAXES, &axis_num);
    ioctl(fd, JSIOCGNAME(80), &name);
    // fcntl(fd, F_SETFL, O_NONBLOCK);
    //ボタン数に応じて配列を作成
    buttons = new bool[button_num];
    axises = new int[axis_num];
    printf("name=%s button=%d,axis=%d\n", name, button_num, axis_num);
    fcntl(fd, F_SETFL, O_NONBLOCK);   // using non-blocking mode
}

GamePad::~GamePad() {
    if (fd >= 0) {
        printf("GamePad Close\n");
        close(fd);
    }
    delete[] buttons;
    delete[] axises;
}

void GamePad::Update() {
    struct js_event event;
    //データがあるなら読み込む
    if (read(fd, &event, sizeof(event)) == sizeof(event)) {
        switch (event.type & ~JS_EVENT_INIT) {
            case JS_EVENT_BUTTON:
                buttons[event.number] = event.value;
                if (button_event != nullptr) {
                    button_event(this, (ButtonNames)event.number,
                                 (bool)event.value);
                }
                break;
            case JS_EVENT_AXIS:
                axises[event.number] = event.value;
                if (axis_event != nullptr) {
                    axis_event(this, (AxisNames)event.number,
                               (float)event.value / (float)INT16_MAX);
                }
                break;
            default:
                break;
        }
    }
}

void GamePad::Status() const {
    //ボタン情報
    printf("%d", buttons[0]);
    for (int idx = 1; idx < button_num; idx++) {
        printf(",%d", buttons[idx]);
    }
    printf("\n");
    //スティック情報
    printf("(%d,%d)", axises[0], axises[1]);
    for (int idx = 2; idx < axis_num; idx += 2) {
        printf("(%d,%d)", axises[idx], axises[idx + 1]);
    }
    printf("\n");
    fflush(stdout);
}