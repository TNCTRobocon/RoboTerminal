#include "main.hpp"
#include "argument.hpp"
#include <iostream>
#include <memory>
#include <general/gamepad.hpp>
#include "settings.hpp"
using namespace std;
//開放を自動化するためにスマートポインタで実装する。
static unique_ptr<Argument> argument{nullptr};
static unique_ptr<Settings> setting{nullptr};

int main(int argc, char** argv) {
    argument.reset(new Argument(argc,argv));    
    setting.reset(new Settings("setting.txt"));
    cout <<setting->Read("a").value_or("c")<<endl;
    setting->Write("a","b");
    cout <<setting->Read("a").value_or("c")<<endl;
    setting->Update();
    
    return 0;
}