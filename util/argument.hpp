#pragma once
#include <pch.hpp>

namespace Util {
class Argument final {
    static std::optional<Argument> instance;
    std::unordered_set<char> options;
    std::unordered_map<std::string, std::string> parameters;
    std::string location;
    Argument(int argc, char** argv);
public:
    
    Argument(const Argument&) = default;
    ~Argument() = default;
    static void Initialize(int argc, char** argv) {
        instance.emplace(Argument(argc, argv));
    }
    static const std::optional<Argument>& GetInstance() { return instance; }

    bool HasOption(char c) const { return options.find(c) != options.end(); }
    std::optional<std::string> GetValue(const std::string&) const;
    std::string ToString() const;
    std::string GetLocation() const { return location; }
};

}  // namespace Util