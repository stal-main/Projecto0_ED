#pragma once

#include <ostream>
#include <string>
#include "Counter.h"
#include "Ticket.h"
#include "HeapPriorityQueue.h"

using std::string;
using std::to_string;
using std::ostream;


class area {

private:

	string code;

	string description;

	counter** windows;

	int numWindows;

	heapPriorityQueue<ticket*> ticketQueue;

	int totalDispensedTickets;

public:

	area(string code, string description, int numWindows) {

		this->code = code;

		this->description = description;

		this->totalDispensedTickets = 0;

		this->numWindows = numWindows;

		this->windows = new counter*[numWindows];

		for (int i = 0; i <= numWindows; i++) {

			string windowName = code + to_string(i + 1);

			windows[i] = new counter(windowName); 
		}

	}

	~area() {

		for (int i = 0; i < numWindows; i++) {

			delete windows[i];
		}

		delete[] windows;
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

	counter** getWindows() {

		return windows;
	}

	int getNumWindows() {

		return numWindows;
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

		for (int i = 0; i < numWindows; i++) {

			if (windows[i]->getName() == windowName) {

				return windows[i];
			}
		}

		return nullptr;
	}

	void setNumWindows(int newNumWindows) {

		for (int i = 0; i < numWindows; i++) {

			delete windows[i];
		}

		delete[] windows;

		numWindows = newNumWindows;

		windows = new counter*[numWindows];

		for (int i = 0; i < numWindows; i++) {

			string windowName = code + to_string(i + 1);

			windows[i] = new counter(windowName);
		}
	}

	friend ostream& operator<<(ostream& os, const area& a) {

		os << "[" << a.code << "]" << a.description << " | Windows: " << a.numWindows << " | In queue: " << a.ticketQueue.getSize();

		return os;
			
	}

};

