#include "transfer.hpp"
#include <sstream>
using namespace std;
namespace General {
constexpr static char space = ' ';
constexpr static char newline = '\n';

CommandElement CommandElement::Decode(const string& line) {
    stringstream ss{line};
    string command;
    vector<string> parameters;
    ss >> command;
    while (ss){
        string tmp;
        ss>>tmp;
        parameters.push_back(tmp);
    }
    return CommandElement(command,parameters);
}

string CommandElement:: ToString()const{
    stringstream ss;
    ss<<command;
    for (const auto &it:parameters){
        ss<<it;
    }
    return ss.str();
}


}  // namespace General