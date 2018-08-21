#pragma once
#include <string>

enum class ReportType : int {
    Info = 34,  // Blue
    Warn = 32,  // Yellow
    Error = 31  // Red
};

enum class ReportGroup : int {
    Unknown,
    None,
    System,
    //随時追加していってください
};

std::string ToString(ReportType);
std::string ToString(ReportGroup);

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