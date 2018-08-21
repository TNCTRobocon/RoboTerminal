#include "argument.hpp"
#include <algorithm>
#include <string.h>
#include <sstream>
#include <iostream>
using namespace std;

Argument::Argument(int argc, char** argv) {
    //デコード開始
    location = argv[0];
    for (int idx = 1; idx < argc; idx++) {
        const char* it = argv[idx];
        if (!strncmp(it, "--", 2)) {
            string key, value;
            for (it += 2; *it != '\0' && *it != '='; it++) {
                key += *it;
            }
            if (*it == '=')
                it++;
            for (; *it != '\0'; it++) {
                value += *it;
            }
            parameters[key] = value;
        } else if (it[0] == '-') {
            for (it += 1; *it != '\0'; it++) {
                options.insert(*it);
            }
        } else {
        }
    }
}

std::string Argument::ToString() const {
    stringstream ss;
    ss << "Location:" << location << endl;
    ss << "Options:";
    for (const auto& it : options) {
        ss << it;
    }
    ss << endl;
    ss << "Parameter:" << endl;
    for (const auto& it : parameters) {
        ss << it.first << ":" << it.second << endl;
    }

    return ss.str();
}

optional<string> Argument::GetValue(const std::string& key) const {
    auto it = parameters.find(key);
    if (it != parameters.end()) {
        return it->second;
    } else {
        return nullopt;
    }
}
