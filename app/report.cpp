#include "report.hpp"
#include <fstream>
#include <iostream>
#include <chrono>

using namespace std;

string ToString(ReportType type) {
    switch (type) {
        case ReportType::Error:
            return "Error";
        case ReportType::Warn:
            return "Warn";
        case ReportType::Info:
            return "Info";
        default:
            return "";
    }
}



Report::Report(const std::string& _filename) : filename(_filename) {}

void Report::Write(ReportType type,
                   const std::string& group,
                   const std::string& message) {
    const string type_text = ToString(type);  
    //コンソールに出力する
    cout << "\e[" << (int)type<<"m"<<type_text<<"@"<<group<<":"<<message<<"\e[m"<<endl;

    //ファイルに記録する
    auto now = chrono::system_clock::now();
    
    ofstream log(filename);
    if (log.fail()){
        return ;
    }
    log <<type_text<<","<<group<<","<<message;

}

