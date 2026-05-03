#pragma once

#include <ostream>
#include <string>

using std::string;
using std::ostream;

class service {

private:

	string description;

	int priority;

	string areaCode;

	int requestedTickets;

public:

	service(string description, int priority, string areaCode) {

		this->description = description;
		
		this->priority = priority;

		this->areaCode = areaCode;

		this->requestedTickets = 0;
	}

	string getDescription() {

		return description;
	}

	int getPriority() {

		return priority;
	}

	string getAreaCode() {

		return areaCode;
	}

	int getRequestedTickets() {

		return requestedTickets;
	}

	void addRequest() {

		requestedTickets++;
	}

	friend ostream& operator<<(ostream& os, const service& s) {

		os << s.description << " (Priority: " << s.priority << ", Area: " << s.areaCode << ")";

		return os;
	}
};

