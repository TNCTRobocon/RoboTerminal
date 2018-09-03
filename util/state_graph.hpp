#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
class StateNode {
    const std::string name;
    using action_t = std::function<void(void)>;
    const action_t action;

public:
    //コンストラクタ&デストラクタ
    StateNode(const std::string& _name, const action_t& _action)
        : name(_name), action(_action) {}
    StateNode(const StateNode& origin) = default;
    virtual ~StateNode() = default;

    //メソッド
    const std::string& GetName() const { return name; }

    void Process() const {
        if (action)
            action();
    }
};

class StateArrow {
    const std::shared_ptr<StateNode> before, after;
    const std::function<bool(void)> condition;
    int rank;  //優先順位
public:
    //コンストラクタ&デストラクタ
    StateArrow(std::shared_ptr<StateNode> _before,
               std::shared_ptr<StateNode> _after,
               const std::function<bool(void)>& _condition,
               int _rank = 0)
        : before(_before), after(_after), condition(_condition), rank(_rank) {}
    StateArrow(const StateArrow&) = default;
    virtual ~StateArrow();
    // operator

    //メソッド
    int GetRank() const { return rank; }
    bool Check() const { return condition != nullptr ? condition() : false; }
    std::string ToString() const;
};

class StateGraph {
    std::vector<StateArrow>
        sorted_arrows;  // Node、Rankの順でソートされている
    std::shared_ptr<StateNode> running;
public:
    //コンストラクタ&デストラクタ
    StateGraph()=default;
    StateGraph(const StateGraph&)=default;
    virtual ~StateGraph()=default;
    //メソッド(メソッドチェイン)
    StateGraph& Add(const StateArrow& item);
    
    std::string ToString();
    
    

};