#include "state_graph.hpp"
#include <sstream>

using namespace std;
string StateArrow::ToString()const{
    const string from = before!=nullptr?before->GetName():"[Start]";
    const string to = after!=nullptr?after->GetName():"[End]";
    stringstream ss;
    ss<<from<<"->"<<to<<":rank="<<rank;
    return ss.str();    
}