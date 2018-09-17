#pragma once
#include <stddef.h>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <random>
#include <unordered_set>
//作者　terurin
//用途　一意な値を持つクラスとそれを管理するクラス
namespace Util {

class Ticket;
class TicketMachine {
    friend class Ticket;
    std::unordered_set<uint32_t> active_codes;
    const uint32_t limit;               //最大発券数
    mutable uint32_t last{UINT32_MAX};  //最後に発券した位置
public:
    TicketMachine(uint32_t _limit = INT32_MAX) : limit(_limit) {}
    TicketMachine(const TicketMachine&) = default;
    virtual ~TicketMachine() = default;
    std::optional<uint32_t> CreateTicket();
    void ReleaseTicket(uint32_t id);

private:
    uint32_t Next(uint32_t number) const;
};

class Ticket {
    std::shared_ptr<TicketMachine> manager;
    std::optional<uint32_t> id;

public:
    Ticket(std::shared_ptr<TicketMachine> tm)
        : manager(tm), id(tm ? tm->CreateTicket() : 0) {}
    Ticket(Ticket&& move)noexcept;
    Ticket():manager(nullptr),id(std::nullopt){}
    virtual ~Ticket();
    bool IsActive() const { return id.has_value(); }
    uint32_t GetValue()const{return id.value_or(0);}
    const std::optional<uint32_t>& GetId() const { return id; }

    size_t Hash()const{
        const size_t h_man=(size_t)manager.get();
        const size_t h_id=id.value_or(0)^(id.has_value()<<1);
        return h_id^(h_man<<1);
    }

    bool operator==(const Ticket& cmp) const { return id == cmp.id; }
    bool operator!=(const Ticket& cmp) const { return id != cmp.id; }
    bool operator<(const Ticket& cmp) const { return id < cmp.id; }
    bool operator>(const Ticket& cmp) const { return id > cmp.id; }
    bool operator<=(const Ticket& cmp) const { return id <= cmp.id; }
    bool operator>=(const Ticket& cmp) const { return id >= cmp.id; }
};

}  // namespace Util

namespace std {
template <>
struct hash<Util::Ticket> {
    size_t operator()(const Util::Ticket& ticket) const {
        return ticket.Hash();
    }
};
}  // namespace std