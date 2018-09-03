#include "state_graph.hpp"
#include <sstream>

using namespace std;

bool StateArrow::operator<(const StateArrow& cmp) const {
    //実態で分岐
    if (!before&!cmp.before){
        return rank<cmp.rank;
    }else if (!before) {
        return false;
    }else if (!cmp.before){
        return true;
    }else{
        const auto& mine= before->GetName();
        const auto& yours = cmp.before->GetName();
        if (mine!=yours){
            return mine<yours;
        }else{
            return rank<cmp.rank;
        }
    }
}

string StateArrow::ToString() const {
    const string from = before != nullptr ? before->GetName() : "[Start]";
    const string to = after != nullptr ? after->GetName() : "[End]";
    stringstream ss;
    ss << from << "->" << to << ":rank=" << rank;
    return ss.str();
}

StateGraph& StateGraph::Add(const StateArrow& arrow) {
    //TODO
    return *this;
}