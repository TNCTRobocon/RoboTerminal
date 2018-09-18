#include "ticket.hpp"
using namespace std;

namespace Util {

optional<uint32_t> TicketMachine::CreateTicket() {
    for (uint32_t id = Next(last); id != last; id = Next(id)) {
        if (active_codes.count(id) == 0) {
            active_codes.emplace(id);
            return last = id;
        }
    }
    return nullopt;
}

void TicketMachine::ReleaseTicket(uint32_t id) {
    if (active_codes.count(id)) {
        active_codes.erase(id);
    }
}

uint32_t TicketMachine::Next(uint32_t number) const {
    uint32_t x = number + 1;
    return x < limit ? x : 0;
}

Ticket::Ticket(Ticket&& move) noexcept : manager(move.manager), id(move.id) {
    //破棄
    move.manager.reset();
    id.reset();
}

Ticket::~Ticket() {
    if (manager && id) {
        manager->ReleaseTicket(id.value());
    }
}

}  // namespace Util