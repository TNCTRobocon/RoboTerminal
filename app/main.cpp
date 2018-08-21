#include "main.hpp"
#include "argument.hpp"
#include <iostream>
#include <memory>
#include <general/gamepad.hpp>
#include "report.hpp"
#include "settings.hpp"
using namespace std;
//開放を自動化するためにスマートポインタで実装する。
static unique_ptr<Argument> argument{nullptr};
static unique_ptr<Settings> setting{nullptr};
static unique_ptr<Report> report{nullptr};

int main(int argc, char** argv) {
    argument.reset(new Argument(argc,argv));    
    setting.reset(new Settings("setting.txt"));
    report.reset(new Report("report.log"));
    cout <<setting->Read("game").value_or("c")<<endl;
    cout <<setting->Read("serial").value_or("c")<<endl;

    report->Info("System","Wake Up");
    setting->Update();
    
    return 0;
}