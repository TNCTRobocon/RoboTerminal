#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
#include "predicate.hpp"
//作者 terurin
//用途　グラフ理論をもとにしたステートマシンの記述

namespace Util {

class StateNode {
    const std::function<void()> action;
    const std::string name;

public:
    StateNode(const std::function<void()> _action, const std::string& _name)
        : action(_action), name(_name) {}
    ~StateNode() {}
    void operator()() const { action(); }
    const std::string& GetName() const { return name; }
    static std::shared_ptr<StateNode> Create(
        const std::function<void()> _action,
        const std::string& _name = "") {
        return std::make_shared<StateNode>(_action, _name);
    }
};  // namespace Util

class StateEdge {
    const std::shared_ptr<StateNode> from;
    const std::shared_ptr<StateNode> to;
    const std::function<bool()> check;
    const std::string name;

public:
    StateEdge(std::shared_ptr<StateNode> _from,
              std::shared_ptr<StateNode> _to,
              std::function<bool()> _check,
              const std::string& _name)
        : from(_from), to(_to), check(_check), name(_name) {}
    bool operator()() const { return check(); }
    const std::string& GetName() const { return name; }
    std::shared_ptr<const StateNode> From() const { return from; }
    std::shared_ptr<const StateNode> To() const { return to; }
    std::shared_ptr<StateNode> From() { return from; }
    std::shared_ptr<StateNode> To() { return to; }
    std::string ToString() const;
    static std::shared_ptr<StateEdge> Create(
        std::shared_ptr<StateNode> _from,
        std::shared_ptr<StateNode> _to,
        std::function<bool()> _check = AlwaysTrue(),
        const std::string& _name = "") {
        return std::make_shared<StateEdge>(_from, _to, _check, _name);
    }
};

class StateGraph {
    std::unordered_multimap<std::shared_ptr<StateNode>,
                            std::shared_ptr<StateEdge>>
        graph;
    std::shared_ptr<StateNode> running{nullptr};

public:
    StateGraph() = default;
    StateGraph(const StateGraph&) = default;
    virtual ~StateGraph() = default;

    void Insert(std::shared_ptr<StateEdge> edge);

    std::string ToString() const;
    std::shared_ptr<StateNode> GetRunning(){return running;}
    std::string GetRunningName(){return running?running->GetName():"*";}
    static std::shared_ptr<StateGraph> Create() {
        return std::make_shared<StateGraph>();
    }
    void Step();
    
};

}  // namespace Util
