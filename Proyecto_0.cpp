
#include <string>
#include "UserType.h"
#include "Service.h"
#include "Ticket.h"
#include "Area.h"


using std::string;
using std::to_string;

ticket* ticketCalculation(userType& user, service& serv, int& globalConsecutive) {

    int PU = user.getPriority();
    int PS = serv.getPriority();
    int PT = PU * 10 + PS;

    string code = serv.getAreaCode() + to_string(globalConsecutive);

    globalConsecutive++;

    return new ticket(code, PT);
}

int main() {

    int globalConsecutive = 100;

    userType u("Wiwiwi", 2);

    service s("Wawawa", 3, "C");

    ticket* t = ticketCalculation(u, s, globalConsecutive);
    
    return 0;
}



