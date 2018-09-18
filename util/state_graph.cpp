#include "state_graph.hpp"
#include <boost/format.hpp>
using namespace std;

namespace Util {

string StateEdge::ToString(bool active) const {
    const string start = from ? from->GetName() : "*";
    const string end = to ? to->GetName() : "*";
    return (boost::format((active)?"(*)[%1%]->[%2%]:%3%":"( )[%1%]->[%2%]:%3%") % start % end % name).str();
}

string StateEdge::ToPlant(bool active)const{
    const string start = from ? from->GetName() : "[*]";
    const string end = to ? to->GetName() : "[*]";
    return (boost::format((from&&active)?"%1%#FF0000->%2%:%3%":"%1%->%2%:%3%") % start % end % name).str();
}

void StateNode::operator()(){
    if (action){
        action();
    }
}

bool StateEdge::operator()(){
    if (check){
        return check();
    }else{
        return true;
    }
}

void StateGraph::Insert(shared_ptr<StateEdge> edge) {
    graph.emplace(edge->From(), edge);
}

void StateGraph::Save(const string& filename)const{
//TODO
}

string StateGraph::ToString() const {
    stringstream ss;
    for (const auto& it : graph) {
        const auto& edge = it.second;
        ss << edge->ToString(edge->From()==next) << endl;
    }
    return ss.str();
}

string StateGraph::ToPlant()const{
    stringstream ss;
    ss<<"@startuml"<<endl;
    for (const auto& it : graph) {
        const auto& edge = it.second;
        ss << edge->ToPlant(edge->From()==next) << endl;
    }
    ss<<"@enduml"<<endl;
    return ss.str();
}


bool StateGraph::Step() {
    if (next != nullptr)
        (*next)();
    auto range = graph.equal_range(next);
    for (auto it = range.first; it != range.second; it++) {
        auto edge = it->second;
        if ((*edge)()) {
            next = edge->To();
            break;
        }
    }
    return next!=nullptr;
}

void StateGraph::StepAll() {
    do {
        Step();
    } while (next != nullptr);
}

}  // namespace Util