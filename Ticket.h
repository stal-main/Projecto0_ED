#pragma once

#include <string>
#include <ostream>
#include "time.h"

using std::string;
using std::ostream;

class ticket {

private:

	string code;
	int priority;
	time_t hourArrival;
	time_t hourAttended;

public:

	ticket(string code, int priority) {

		this->code = code;

		this->priority = priority;

		this->hourArrival = time(nullptr);

		this->hourAttended = 0;
	}

	string getCode() const {

		return code;
	}

	int getPriority() const {

		return priority;
	}

	time_t getHourArrival() const {

		return hourArrival;
	}

	time_t getHourAttended() const {

		return hourAttended;
	}

	bool wasAttended() const {

		return hourAttended != 0;
	}

	void attend() {

		hourAttended = time(nullptr);
	}

	double getWaitTime() const {

		if (!wasAttended()) {

			return 0;
		}

		return difftime(hourAttended, hourArrival);
	}

	bool operator<(const ticket& another) const {

		return this->priority < another.priority;
	}

	friend ostream& operator<<(ostream& os, const ticket& t) {

		os << "Ticket :" << t.code << ", prioridad: " << t.priority;

		if (t.wasAttended()) {

			os << "Tiempo esperado: " << t.getWaitTime() << "s";
		}

		else {

			os << "Esperando";
		}

		return os;
	}
};

