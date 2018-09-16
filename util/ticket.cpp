#include "ticket.hpp"
#include <iostream>
using namespace std;

namespace Util {

optional<uint32_t> TicketMachine::CreateTicket() {
    for (uint32_t id = Next(last); id != last; id = Next(id)) {
        if (active_codes.count(id)==0) {
            active_codes.emplace(id);
            return last=id;
        }
    }
    return nullopt;
}

void TicketMachine::ReleaseTicket(uint32_t id){
    if (active_codes.count(id)){
        active_codes.erase(id);
    }
}

}