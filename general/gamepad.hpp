#pragma once
#include <pch.hpp>
namespace General{

class GamePad;

//DefaultPadについて
void InitilizeDefaultPad();
std::shared_ptr<GamePad> GetDefaultPad();//制約 InitilizeDefaultPad()を実行すること

enum class AxisNames : unsigned int {
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

enum class ButtonNames : unsigned int {
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
    static constexpr size_t button_size = 12;
    static constexpr size_t axis_size = 8;
    int fd;  //ゲームパッドのファイル識別子
    std::vector<int> buttons;
    std::vector<int> axises;

public:
    GamePad(const std::string& filename);
    virtual ~GamePad();
    void Update();               //ここで値を取得する
    std::string Status() const;  //標準入出力へ状態を表示
    //状態確認
    inline bool GetButton(unsigned int id) const {
        return (id < buttons.size()) ? buttons[id] : false;
    }
    inline bool GetButton(ButtonNames name) const {
        return GetButton((unsigned int)name);
    }

    inline int GetAxisRaw(unsigned int id) const {
        return (id < axises.size()) ? axises[id] : 0;
    }
    inline int GetAxisRaw(AxisNames name) const {
        return GetAxisRaw((unsigned int)name);
    }

    inline float GetAxis(unsigned int id) const {
        return (float)GetAxisRaw(id) / INT16_MAX;
    }
    inline float GetAxis(AxisNames name) const {
        return GetAxis((unsigned int)name);
    }

    //ボタンの個数
    inline int GetButtonNum() const { return buttons.size(); }

    inline int GetAxisNum() const { return axises.size(); }
};
}