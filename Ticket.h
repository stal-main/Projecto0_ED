#pragma once

#include <string>
#include <ctime>

using std::string;

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

	double getWaitTime() {

		if (!wasAttended()) {

			return 0;
		}

		return difftime(hourAttended, hourArrival);
	}

	bool operator<(const ticket& another) const {

		return this->priority < another.priority;
	}
};

