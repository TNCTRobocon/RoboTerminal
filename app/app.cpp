#include "app.hpp"
#include <iostream>
#include <signal.h>
#include <general/gamepad.hpp>
using namespace std;
//開放を自動化するためにスマートポインタで実装する。
shared_ptr<Argument> argument{nullptr};
shared_ptr<Settings> setting{nullptr};
shared_ptr<Report> report{nullptr};

static bool volatile is_continue{false};
static void singal_receiver(int num){
    cout<<endl;
    is_continue=false;
}

int main(int argc, char** argv) {
    //System全体で使う変数を初期化する
    argument.reset(new Argument(argc,argv));    
    setting.reset(new Settings("setting.config"));
    report.reset(new Report("report.log"));
    report->Info(ReportGroup::System,"Wake Up");
    
    //MessageLoop
    signal(SIGINT,singal_receiver);
    for (is_continue=true;is_continue;){
        
    }

    
    report->Info(ReportGroup::System,"Shutdown");
    return 0;
}

