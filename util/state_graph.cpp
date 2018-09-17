#include "state_graph.hpp"
#include <boost/format.hpp>
#include <sstream>
#include <tuple>
using namespace std;

namespace Util {

string StateEdge::ToString() const {
    return (boost::format("[%1%]->[%2%]:%3%") % from->GetName() %
            to->GetName() % name)
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

void StateGraph::Step() {}

}  // namespace Util