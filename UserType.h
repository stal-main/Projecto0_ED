#pragma once

#include <ostream>
#include <string>

using std::string;
using std::ostream;

class userType {

private:

	string description;

	int priority;

public:

	userType(string description, int priority) {

		this->description = description;

		this->priority = priority;
	}

	string getDescription() {

		return description;
	}

	int getPriority() {

		return priority;
	}

	bool operator<(const userType& another) const {

		return this->priority < another.priority;
	}

	friend ostream& operator<<(ostream& os, const userType& t) {

		os << t.description << " (Priority: " << t.priority << ")";

		return os;

	}
};

