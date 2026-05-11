#pragma once

//clase que representa un tipo de usuario con su descripción y prioridad
//escrito por Kevin Solano y Julián Rodríguez

#include <ostream>
#include <string>

using std::string;
using std::ostream;

class userType {

private:

	string description;

	int priority;

	int requestedTickets;

public:
	//constructor
	userType(string description, int priority) {

		this->description = description;

		this->priority = priority;

		this->requestedTickets = 0;
	}
	//getters
	string getDescription() {

		return description;
	}

	int getPriority() {

		return priority;
	}

	int getRequestedTickets() {

		return requestedTickets;
	}

	//incrementar la cantidad de tiquetes solicitados para las estadisticas
	void addRequest() {

		requestedTickets++;
	}

	//reiniciar el contador de tiquetes para limpiar estadisticas
	void resetStats() {

		requestedTickets = 0;
	}

	//comparar la prioridad de dos tiquetes para ordenar la cola
	bool operator<(const userType& another) const {

		return this->priority < another.priority;
	}

	//imprimir el tipo de usuario y su prioridad
	friend ostream& operator<<(ostream& os, const userType& t) {

		os << t.description << " (Priority: " << t.priority << ")";

		return os;

	}
};