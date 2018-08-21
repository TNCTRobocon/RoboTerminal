#include "app.hpp"
#include <iostream>

#include <general/gamepad.hpp>
using namespace std;
//開放を自動化するためにスマートポインタで実装する。
shared_ptr<Argument> argument{nullptr};
shared_ptr<Settings> setting{nullptr};
shared_ptr<Report> report{nullptr};

int main(int argc, char** argv) {
    //System全体で使う変数を初期化する
    argument.reset(new Argument(argc,argv));    
    setting.reset(new Settings("setting.config"));
    report.reset(new Report("report.log"));



    setting->Update();
    
    return 0;
}