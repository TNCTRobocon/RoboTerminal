#include "state_graph.hpp"
#include <boost/format.hpp>
#include <sstream>
#include <tuple>
using namespace std;

namespace Util {

string StateEdge::ToString() const {
    const string start = from?from->GetName():"*";
    const string end = to?to->GetName():"*";
    return (boost::format("[%1%]->[%2%]:%3%") % start %
            end % name)
        .str();
}

void StateGraph::Insert(shared_ptr<StateEdge> edge) {
    graph.emplace(edge->From(), edge);
}

string StateGraph::ToString() const {
    stringstream ss;
    for (const auto& it : graph) {
        const auto& edge = it.second;
        ss << edge->ToString() << endl;
    }
    return ss.str();
}

void StateGraph::Step() {
    if (next!=nullptr)(*next)();
    auto range = graph.equal_range(next);
    for (auto it=range.first;it!=range.second;it++){
        auto edge = it->second;
        if ((*edge)()){
            next=edge->To();
        }
    }
}

void StateGraph::StepAll(){
    do{
    Step();
    }while (next!=nullptr);
}

}  // namespace Util