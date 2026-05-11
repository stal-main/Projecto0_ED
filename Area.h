#pragma once

//clase que representa un area de servicio, con su codigo, descripcion, ventanillas y una cola de tickets
//escrito por Kevin Solano y Julián Rodríguez

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

	int totalAttendedTickets;

	double totalWaitTime;

public:

	//constructor
	area(string code, string description, int numWindows) {

		this->code = code;

		this->description = description;

		this->totalDispensedTickets = 0;

		this->totalAttendedTickets = 0;

		this->totalWaitTime = 0.0;

		this->numWindows = numWindows;

		this->windows = new counter*[numWindows];

		for (int i = 0; i < numWindows; i++) {

			string windowName = code + to_string(i + 1);

			windows[i] = new counter(windowName); 
		}

	}

	//destructor
	~area() {

		for (int i = 0; i < numWindows; i++) {

			delete windows[i];
		}

		delete[] windows;
	}

	//getters
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

	//registrar el tiempo de espera de un tiquete atendido para calcular el promedio por area
	void addWaitTime(double waitTime) {

		totalWaitTime += waitTime;

		totalAttendedTickets++;
	}

	//obtener el tiempo promedio de espera de los tiquetes atendidos en el area
	double getAvgWaitTime() {

		if (totalAttendedTickets == 0) return 0.0;

		return totalWaitTime / totalAttendedTickets;
	}

	//reiniciar todos los contadores del area y sus ventanillas para limpiar estadisticas
	void resetStats() {

		totalDispensedTickets = 0;

		totalAttendedTickets = 0;

		totalWaitTime = 0.0;

		for (int i = 0; i < numWindows; i++) {

			windows[i]->resetStats();
		}
	}

	//añadir un tiquete a la cola de tickets del area, aumentando en 1 el contador de tiquetes dispensados
	void addTicket(ticket* t) {

		ticketQueue.insert(t, t->getPriority());

		totalDispensedTickets++;
	}

	//obtener el siguiente tiquete a atender, eliminandolo de la cola de tickets del area
	ticket* getNextTicket() {

		if (ticketQueue.isEmpty()) return nullptr;

		return ticketQueue.removeMin();
	}

	//verificar si la cola de tickets del area esta vacia
	bool queueIsEmpty() {

		return ticketQueue.isEmpty();
	}

	//obtener el tiquete en la posicion dada sin eliminarlo de la cola, para mostrar los codigos en pantalla
	ticket* peekTicketAt(int i) const {

		return ticketQueue.getElementAt(i);
	}

	//buscar una ventanilla por su nombre, devolviendo un puntero a la ventanilla si se encuentra o nullptr si no se encuentra
	counter* findWindow(string windowName) {

		for (int i = 0; i < numWindows; i++) {

			if (windows[i]->getName() == windowName) {

				return windows[i];
			}
		}

		return nullptr;
	}

	//eliminar las ventanillas y crear unas nuevas con el nuevo numero de ventanillas, 
	//asignandoles un nombre a cada una a partir del codigo del area y un numero consecutivo
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

	//imprimir el codigo del area, su descripcion, el numero de ventanillas y la cantidad de tiquetes en la cola
	friend ostream& operator<<(ostream& os, const area& a) {

		os << "[" << a.code << "]" << a.description << " | Windows: " << a.numWindows << " | In queue: " << a.ticketQueue.getSize();

		return os;
			
	}

};

