#include "settings.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

namespace Util {
constexpr char Settings::split;

static inline void DeleteSpace(string& buf) {
    size_t pos;
    while ((pos = buf.find_first_of(" 　\t")) != string::npos) {
        buf.erase(pos, 1);
    }
}

Settings::table_t Settings::LoadFile(const string& filename) {
    table_t result;
    ifstream stream(filename);
    if (!stream.is_open())
        return result;
    string line;
    while (getline(stream, line)) {
        string cut = line.substr(0, line.find('#'));
        stringstream ss(cut);
        if (string key, value;
            getline(ss, key, split) && getline(ss, value, split)) {
            //空白文字を消去
            DeleteSpace(key);
            DeleteSpace(value);

            result[key] = value;
        }
    }
    return result;
}

Settings::Settings(const string& _filename)
    : filename(_filename), table(LoadFile(_filename)) {}

optional<string> Settings::Read(const string& key) const {
    auto it = table.find(key);
    if (it != table.end()) {
        return it->second;
    } else {
        return std::nullopt;
    }
}

void Settings::Update() {
    table_t table_old = LoadFile(filename);
    table_t diff;

    for (const auto& record : table) {
        auto comp = table_old.find(record.first);
        if (comp == table_old.end()) {
            diff[record.first] = record.second;
        } else if (record.second != comp->second) {
            diff[record.first] = record.second;
        }
    }

    //変更点のみを書き込み
    ofstream stream(filename, ios::app);  //追記
    if (!stream.is_open())
        return;
    for (const auto& record : diff) {
        stream << record.first << split << record.second << endl;
    }
    stream.flush();
}

}  // namespace Util