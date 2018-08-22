#pragma once
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace Util {
class Argument {
    std::unordered_set<char> options;
    std::unordered_map<std::string, std::string> parameters;
    std::string location;

public:
    Argument(int argc, char** argv);
    Argument(const Argument&) = default;
    virtual ~Argument() = default;
    bool HasOption(char c) const { return options.find(c) != options.end(); }
    std::optional<std::string> GetValue(const std::string&) const;
    std::string ToString() const;
    std::string GetLocation() const { return location; }
};
}  // namespace Util