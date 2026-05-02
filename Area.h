#pragma once

#include <ostream>
#include <string>
#include <vector>
#include "Counter.h"
#include "Ticket.h"
#include "HeapPriorityQueue.h"

using std::string;
using std::vector;
using std::to_string;
using std::ostream;


class area {

private: 
	
	string code;

	string description;

	vector<counter*> windows;

	heapPriorityQueue<ticket*> ticketQueue;

	int totalDispensedTickets;

public:

	area(string code, string description, int numWindows) {

		this->code = code;

		this->description = description;

		this->totalDispensedTickets = 0;

		for (int i = 1; i <= numWindows; i++) {

			string windowName = code + to_string(i);

			windows.push_back(new counter(windowName));
		}

	}

	~area() {

		for (counter* v : windows) {

			delete v;
		}
	}

	string getCode() {

		return code;
	}

	string getDescription() {

		return description;
	}

	int getQueueSize() {

		return ticketQueue.getSize();
	}

	int getTotalDispensed() {

		return totalDispensedTickets;
	}

	vector<counter*>& getWindows() {

		return windows;
	}

	void addTicket(ticket* t) {

		ticketQueue.insert(t, t->getPriority());

		totalDispensedTickets++;
	}

	ticket* getNextTicket() {

		if (ticketQueue.isEmpty()) return nullptr;

		return ticketQueue.removeMin();
	}

	bool queueIsEmpty() {

		return ticketQueue.isEmpty();
	}

	counter* findWindow(string windowName) {

		for (counter* c : windows) {

			if (c->getName() == windowName) {

				return c;
			}
		}

		return nullptr;
	}

	void setNumWindows(int numWindows) {

		for (counter* c : windows) {

			delete c;
		}

		windows.clear();

		for (int i = 1; i <= numWindows; i++) {

			string windowsName = code + to_string(i);

			windows.push_back(new counter(windowsName));
		}
	}

	friend ostream& operator<<(ostream& os, const area& a) {

		os << "[" << a.code << "]" << a.description << " | Windows: " << a.windows.size() << " | In queue: " << a.ticketQueue.getSize();

		return os;
			
	}

};

