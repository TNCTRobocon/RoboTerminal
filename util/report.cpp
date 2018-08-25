#include "report.hpp"
#include <chrono>
#include <fstream>
#include <iostream>

using namespace std;
namespace Util {
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
    cout << "\e[" << (int)type << "m" << type_text << "@" << group << ":"
         << message << "\e[m" << endl;

    //ファイルに記録する
    ofstream log(filename, ios::app);
    if (log.fail()) {
        return;
    }
    log << type_text << "," << group << "," << message << endl;
}

}  // namespace Util