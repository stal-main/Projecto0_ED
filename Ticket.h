#pragma once

//clase que crea un tiquete con su codigo, prioridad, hora de llegada y hora de atencion.
//escrito por Kevin Solano y Julián Rodríguez

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
	//constructor
	ticket(string code, int priority) {

		this->code = code;

		this->priority = priority;

		this->hourArrival = time(nullptr);

		this->hourAttended = 0;
	}

	//getters
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

	//verificar si el tiquete ya fue atendido
	bool wasAttended() const {

		return hourAttended != 0;
	}

	//marcar el tiquete como atendido y guardar la hora de atencion
	void attend() {

		hourAttended = time(nullptr);
	}

	//obtener el tiempo que se tardó en atender el tiquete
	double getWaitTime() const {

		if (!wasAttended()) {

			return 0;
		}

		return difftime(hourAttended, hourArrival);
	}

	//comparar la prioridad de dos tiquetes para ordenar la cola
	bool operator<(const ticket& another) const {

		return this->priority < another.priority;
	}

	//imprimir el tiquete, su prioridad y si ya fue atendido o no
	//en caso de ser atendido muestra el tiempo que taró en ser atendido
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