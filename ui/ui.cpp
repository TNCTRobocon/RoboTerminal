#include "ui.hpp"

#include <stdio.h>
using namespace std;
namespace UI {

Window::Window(int* argc, char*** argv) {
    // gtkの初期化
    gtk_init(argc, argv);
    // windowの初期化
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(window, 300, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(Window::Exit), this);

    gtk_widget_show(window);
    showing = true;
}

Window::~Window() {}

bool Window::Process() {
    while (gtk_events_pending()) {
        gtk_main_iteration();
    }
    return showing;
}

void Window::Exit(void* dummy, Window* win) {
    win->showing = false;
}

}  // namespace UI