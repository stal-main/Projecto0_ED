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

public:

	service(string description, int priority, string areaCode) {

		this->description = description;
		
		this->priority = priority;

		this->areaCode = areaCode;
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

	friend ostream& operator<<(ostream& os, const service& s) {

		os << s.description << " (Priority: " << s.priority << ", Area: " << s.areaCode << ")";
		return os;
	}
};

