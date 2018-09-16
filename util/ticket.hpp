#pragma once
#include <stddef.h>
#include <cstdint>
#include <optional>
#include <random>
#include <unordered_set>
namespace Util {

class Ticket;
class TicketMachine {
    friend class Ticket;
    std::unordered_set<uint32_t> active_codes;
    const uint32_t limit;  //最大発券数
    mutable uint32_t last{UINT32_MAX};      //最後に発券した位置
public:
    TicketMachine(uint32_t _limit = INT32_MAX) : limit(_limit) {}
    TicketMachine(const TicketMachine&) = default;
    virtual ~TicketMachine() = default;
    std::optional<uint32_t> CreateTicket();
    void ReleaseTicket(uint32_t id);

private:
    uint32_t Next(uint32_t number)const{
        uint32_t x =number+1;
        return  x<limit?x:0;
    }
};

class Ticket {
    std::optional<uint32_t> code;

public:
    Ticket(TicketMachine&);
    Ticket(Ticket&&);
    virtual ~Ticket();
};

}  // namespace Util
