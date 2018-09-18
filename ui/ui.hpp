#pragma once

//作者 teruin
//用途 windowの描画
//補足 シグネチャーの関係でbetter Cとして記述する。
//(gtkmmはc++17に非対応のため利用できないため、gtk+を直に叩く)

#include <memory>
#include <gtk/gtk.h>

namespace UI {

struct Window {
    // field定義
    GtkWidget* window;
    bool showing;

    //コンストラクタ
    Window(int* argc, char*** argv);
    Window(const Window&) = delete;
    virtual ~Window();
    //メソッド(外部用)
    bool Process();  //メッセージループ用
    //メソッド(内部用)
    static void Exit(void* dummy, Window* win);
};

}  // namespace UI