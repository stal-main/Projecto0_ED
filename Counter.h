#pragma once

#include <ostream>
#include <string>
#include "Ticket.h"

using std::string;
using std::ostream;

class counter {

private:

	string name;

	ticket* actualTicket;

	int attendedTickets;

public:

	counter(string name) {

		this->name = name;

		this->actualTicket = nullptr;

		this->attendedTickets = 0;
	}

	string getName() {

		return name;
	}

	ticket* getActualTicket() {

		return actualTicket;
	}

	int getAttendedTickets() {

		return attendedTickets;
	}

	bool isBusy() {

		return actualTicket != nullptr;
	}

	void attendTicket(ticket* t) {

		actualTicket = t;

		actualTicket->attend();

		attendedTickets++;
	}

	void freeCounter() {

		actualTicket = nullptr;
	}

	friend ostream& operator<<(ostream& os, const counter& c) {

		os << "Counter " << c.name << " | Attending: ";

		if (c.actualTicket != nullptr) {

			os << c.actualTicket->getCode();
		}

		else {

			os << "---";
		}

		return os;
	}
};

