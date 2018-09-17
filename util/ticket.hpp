#pragma once
#include <stddef.h>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <random>
#include <unordered_set>
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
    Ticket(Ticket&& move);
    Ticket(const Ticket&)=delete;
    virtual ~Ticket();
    bool IsActive() const { return id.has_value(); }
    const std::optional<uint32_t>& GetId() const { return id; }
    bool operator==(const Ticket& cmp) const { return id == cmp.id; }
    bool operator!=(const Ticket& cmp) const { return id != cmp.id; }
    bool operator<(const Ticket& cmp) const { return id < cmp.id; }
    bool operator>(const Ticket& cmp) const { return id > cmp.id; }
    bool operator<=(const Ticket& cmp) const { return id <= cmp.id; }
    bool operator>=(const Ticket& cmp) const { return id >= cmp.id; }
    size_t Hash()const{
        const size_t h_man=(size_t)manager.get();
        const size_t h_id=id.value_or(0)^(id.has_value()<<1);
        return h_id^(h_man<<1);
    }
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