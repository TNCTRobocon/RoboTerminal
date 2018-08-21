#pragma once
#include <stddef.h>
#include <stdint.h>
#include <functional>
#include <memory>
#include <thread>
#include <string>
enum class AxisNames {
    LSX = 0,
    LSY = 1,
    RSX = 2,
    RSY = 3,
    RT = 4,
    LT = 5,
    DX = 6,
    DY = 7
};

static inline const char* GetAxisName(AxisNames name) {
    const char* names[] = {"LSX", "LSY", "RSX", "RSY", "RT", "LT", "DX", "DY"};
    int idx = (int)name;
    const int cnt = sizeof(names) / sizeof(names[0]);
    return idx < cnt ? names[idx] : "Unknown";
}

enum class ButtonNames {
    A = 0,
    B = 1,
    X = 2,
    Y = 3,
    LB = 4,
    RB = 5,
    BACK = 6,
    START = 7,
    CROSS = 8,
    AXISL = 9,
    AXISR = 10
};

static inline const char* GetButtonName(ButtonNames name) {
    const char* names[] = {"A",    "B",     "X",     "Y",     "LB",   "RB",
                           "BACK", "START", "CROSS", "AXISL", "AXISR"};
    int idx = (int)name;
    const int cnt = sizeof(names) / sizeof(names[0]);
    return idx < cnt ? names[idx] : "Unknown";
}

class GamePad {
private:
    int fd;  //ゲームパッドのファイル識別子
    std::unique_ptr<std::thread> task{nullptr};
    int button_num{0}, axis_num{0};
    bool* buttons{nullptr};
    int* axises{nullptr};
    typedef std::function<void(const GamePad*, ButtonNames, bool)>
        button_event_t;
    typedef std::function<void(const GamePad*, AxisNames, float)> axis_event_t;
    button_event_t button_event{nullptr};
    axis_event_t axis_event{nullptr};

    void Update();  //ここで非同期で値を取得する。
public:
    GamePad(const std::string& filename);
    virtual ~GamePad();
    void Status() const;  //標準入出力へ状態を表示
    //イベント関数設定
    inline void SetButtonChangedEvent(const button_event_t& event = nullptr) {
        button_event = event;
    }

    inline void SetAxisChangedEvent(const axis_event_t& event = nullptr) {
        axis_event = event;
    }

    //状態確認
    inline bool GetButton(int id) const {
        // printf("%d%d\n",id,buttons[id]);
        return id < button_num ? buttons[id] : false;
    }
    inline bool GetButton(ButtonNames name) const {
        return GetButton((int)name);
    }

    inline int GetAxisRaw(int id) const {
        return id < axis_num ? axises[id] : 0;
    }
    inline int GetAxisRaw(AxisNames name) const {
        return GetAxisRaw((int)name);
    }

    inline float GetAxis(int id) const {
        return (float)GetAxisRaw(id) / INT16_MAX;
    }
    inline float GetAxis(AxisNames name) const { return GetAxis((int)name); }

    //ボタンの個数
    inline int GetButtonNum() const { return button_num; }

    inline int GetAxisNum() const { return axis_num; }

    //判断
    inline int ButtonDecision() {
        if (button_event != nullptr) {
            // PadButtonHandler(button_number,ButtonNames name,);
        }
        return 0;
    }
    inline int AxisDecision() {
        if (axis_event != nullptr) {
            // PadAxisHandler(axis_number,ButtonNames name);
        }
        return 0;
    }

    inline int PadButtonHandler(int a, ButtonNames name, void (*Btn)(int));

    inline int PadAxisHandler(int b, AxisNames name, void (*Axis)(int));
};
