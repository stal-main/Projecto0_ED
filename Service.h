#pragma once

//clase que representa un servicio con su descripción, prioridad, código de área y cantidad de tiquetes solicitados
//escrito por Kevin Solano y Julián Rodríguez

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
	//constructor
	service(string description, int priority, string areaCode) {

		this->description = description;
		
		this->priority = priority;

		this->areaCode = areaCode;

		this->requestedTickets = 0;
	}

	//getters
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

	//incrementar la cantidad de tiquetes solicitados para las estadisticas
	void addRequest() {

		requestedTickets++;
	}

	//comparar la prioridad de dos tiquetes para ordenar la cola
	friend ostream& operator<<(ostream& os, const service& s) {

		os << s.description << " (Priority: " << s.priority << ", Area: " << s.areaCode << ")";

		return os;
	}
};

