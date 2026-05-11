#pragma once

//clase que representa una ventanilla de las diferentes areas con el nombre de la seccion elegida, 
//el tiquete que se esta atendiendo y la cantidad de tiquetes atendidos
//escrito por Kevin Solano

#include <ostream>
#include <string>
#include "Ticket.h"

using std::string;
using std::ostream;

class counter {

private:

	string name;

	ticket* actualTicket;

	int attendedTickets;

public:

	//constructor
	counter(string name) {

		this->name = name;

		this->actualTicket = nullptr;

		this->attendedTickets = 0;
	}

	//destructor: libera la memoria del tiquete actual si existe
	~counter() {

		delete actualTicket;
	}

	//getters
	string getName() {

		return name;
	}

	ticket* getActualTicket() {

		return actualTicket;
	}

	int getAttendedTickets() {

		return attendedTickets;
	}

	//saber si la ventanilla esta ocupada o no
	bool isBusy() {

		return actualTicket != nullptr;
	}

	//atender un tiquete y aumentar en 1 el contador de tiquetes atendidos
	//si habia un tiquete anterior, lo elimina antes de reemplazarlo
	void attendTicket(ticket* t) {

		delete actualTicket;

		actualTicket = t;

		actualTicket->attend();

		attendedTickets++;
	}

	//libera la ventanilla asignandole nullptr al tiquete actual
	void freeCounter() {

		delete actualTicket;

		actualTicket = nullptr;
	}

	//imprimir el nombre de la ventanilla y el tiquete que se esta atendiendo, si no hay tiquete muestra ---
	friend ostream& operator<<(ostream& os, const counter& c) {

		os << "ventanilla " << c.name << " | Atendiendo: ";

		if (c.actualTicket != nullptr) {

			os << c.actualTicket->getCode();
		}

		else {

			os << "---";
		}

		return os;
	}
};

