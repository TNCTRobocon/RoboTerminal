#pragma once
#include <pch.hpp>

namespace Util {

enum class ReportType : int {
    Info = 36,  // LightBlue
    Warn = 32,  // Yellow
    Error = 31  // Red
};
std::string ToString(ReportType);
enum class ReportGroup : int {
    Unknown,
    None,
    System,
    GamePad,
    SerialPort,
    GUI
    //随時追加していってください
};
static inline const std::string& ToString(ReportGroup group) {
    //随時追加していってください
    const static std::string table[] = {"Unknown", "None",       "System",
                                        "GamePad", "SerialPort", "GUI"};
    return table[(int)group];
}

class Report {
    const std::string filename;

public:
    Report(const std::string& filename);
    Report(const Report&) = default;
    virtual ~Report() = default;
    void Write(ReportType type,
               const std::string& group,
               const std::string& message);

    void Write(ReportType type, ReportGroup group, const std::string& message) {
        Write(type, ToString(group), message);
    }

    void Info(const std::string& group, const std::string& message) {
        Write(ReportType::Info, group, message);
    }

    void Info(ReportGroup group, const std::string& message) {
        Write(ReportType::Info, group, message);
    }

    void Warn(const std::string& group, const std::string& message) {
        Write(ReportType::Warn, group, message);
    }

    void Warn(ReportGroup group, const std::string& message) {
        Write(ReportType::Warn, group, message);
    }

    void Error(const std::string& group, const std::string& message) {
        Write(ReportType::Error, group, message);
    }

    void Error(ReportGroup group, const std::string& message) {
        Write(ReportType::Error, group, message);
    }
};
}  // namespace Util
