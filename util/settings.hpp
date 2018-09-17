#pragma once
#include <pch.hpp>
namespace Util {
class Settings {
public:
    using table_t = std::unordered_map<std::string, std::string>;
    constexpr static char split = '=';

private:
    const std::string filename;
    table_t table;
    static table_t LoadFile(const std::string& filename);

public:
    Settings(const std::string& filename);
    virtual ~Settings() = default;
    std::optional<std::string> Read(const std::string& key) const;
    void Write(const std::string& key, const std::string& value) {
        table[key] = value;
    }

    void Update();
};

}  // namespace Util