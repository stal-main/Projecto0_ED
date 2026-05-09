#include <iostream>
#include <string>
#include <limits>
#include "UserType.h"
#include "Service.h"
#include "Ticket.h"
#include "Area.h"
#include "Counter.h"
#include "LinkedList.h"
#include "HeapPriorityQueue.h"

//programa principal que muestra los menus y le permite al usuario moverse entre ellos
//para simular un sistema de tiquetes con diferentes tipos de usuario, servicios y areas, 
//atendiendo los tiquetes segun su prioridad y mostrando estadisticas de los servicios
//escrito por Juan Esteban Sanchez

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::to_string;
using std::getline;
using std::stoi;
using std::runtime_error;
using std::out_of_range;
using std::invalid_argument;

//Listas para almacenar los tipos de usuario, servicios y areas
heapPriorityQueue<userType*> userTypes;
LinkedList<service*>         services;
LinkedList<area*>            areas;

//Inicializacion de variables
int    globalConsecutive = 100;
int    totalDispensed = 0;
int    totalAttended = 0;
double totalWaitTime = 0.0;

//linea para separar información
void printSeparator() {
    cout << "----------------------------------" << endl;
}

//espera a que el usuario presione enter para continuar
void waitForKey() {
    cout << "\nPresione Enter para continuar...";
    string resp;
    getline(cin, resp);
}

//lee la entrada del usuario como string, la convierte a int y la retorna
int readInt(const string& prompt) {
    string line;
    cout << prompt;
    getline(cin, line);
    return stoi(line);
}

//lee la entrada del usuario como string y la retorna
string readString(const string& prompt) {
    string line;
    cout << prompt;
    getline(cin, line);
    return line;
}

//lee la entrada del usuario como int, validando que este dentro del rango
int readOption(int min, int max) {
    while (true) {
        try {
            int opt = readInt("Opcion: ");
            if (opt < min || opt > max)
                throw out_of_range("Opcion fuera de rango.");
            return opt;
        }
        catch (...) {
            cout << "Error: ingrese un valor valido." << endl;
        }
    }
}

//busca un area por su codigo, devolviendo un puntero al area si se encuentra o nullptr si no se encuentra
area* findAreaByCode(const string& code) {
    areas.goToStart();
    while (!areas.atEnd()) {
        area* a = areas.getElement();
        if (a->getCode() == code) return a;
        areas.next();
    }
    return nullptr;
}

//genera un nuevo tiquete para un usuario y servicio segun lo seleccionado, 
//asignandole un codigo que depende del codigo del area y un numero consecutivo,
ticket* generateTicket(userType& user, service& serv) {
    int PT = user.getPriority() * 10 + serv.getPriority();
    string code = serv.getAreaCode() + to_string(globalConsecutive++);
    serv.addRequest();
    totalDispensed++;
    return new ticket(code, PT);
}

//libera la memoria de los tiquetes en las colas de cada area y de las ventanillas, dejando todas las colas vacias
void clearAllQueues() {
    areas.goToStart();
    while (!areas.atEnd()) {
        area* a = areas.getElement();
        while (!a->queueIsEmpty()) {
            ticket* t = a->getNextTicket();
            delete t;
        }
        counter** windows = a->getWindows();
        for (int i = 0; i < a->getNumWindows(); i++)
            windows[i]->freeCounter();
        areas.next();
    }
}

//muestra el estado de las colas de cada area, mostrando su codigo y descripcion, 
//el numero de ventanillas, el estado de cada ventanilla y la cantidad de tiquetes en la cola
void showQueueStatus() {
    printSeparator();
    cout << "         ESTADO DE LAS COLAS" << endl;
    printSeparator();

    if (areas.getSize() == 0) {
        cout << "No hay areas registradas." << endl;
        waitForKey();
        return;
    }

    areas.goToStart();
    while (!areas.atEnd()) {
        area* a = areas.getElement();
        cout << "\nArea: " << a->getCode() << " - " << a->getDescription() << endl;
        cout << "  Ventanillas (" << a->getNumWindows() << "):" << endl;
        counter** windows = a->getWindows();
        for (int i = 0; i < a->getNumWindows(); i++)
            cout << "    " << *windows[i] << endl;
        cout << endl << "  Tiquetes en cola: " << a->getQueueSize() << endl;
        areas.next();
    }

    waitForKey();
}


//  2. Tickets menu
void ticketMenu() {
    //TODO
}

//  3. AttendTicket
void attendTicket() {
    //TODO
}


//menu para agregar o eliminar tipos de usuario, validando que la descripcion no este vacia 
//y que la prioridad sea un numero entero mayor a 0
void userTypeMenu() {
    int opt;
    do {
        printSeparator();
        cout << "   MENU - TIPOS DE USUARIO" << endl;
        printSeparator();
        cout << "1. Agregar" << endl;
        cout << "2. Eliminar" << endl;
        cout << "3. Regresar" << endl;

        opt = readOption(1, 3);

        if (opt == 1) {
            try {
                string descp = readString("Descripcion: ");
                if (descp.empty())
                    throw runtime_error("La descripcion no puede estar vacia.");

                int prior = readInt("Prioridad: ");
                if (prior < 1)
                    throw out_of_range("La prioridad debe ser mayor a 0.");

                userTypes.insert(new userType(descp, prior), prior);
                cout << "Tipo de usuario agregado." << endl;

            }
            catch (invalid_argument&) {
                cout << "Error: ingrese un numero valido para la prioridad." << endl;
            }
            catch (out_of_range& e) {
                cout << "Error: " << e.what() << endl;
            }
            catch (runtime_error& e) {
                cout << "Error: " << e.what() << endl;
            }

        }
        else if (opt == 2) {
            try {

            }
            catch (invalid_argument&) {
                cout << "Error: ingrese un numero valido." << endl;
            }
            catch (out_of_range& e) {
                cout << "Error: " << e.what() << endl;
            }
            catch (runtime_error& e) {
                cout << "Error: " << e.what() << endl;
            }
        }

    } while (opt != 3);
}


//menu para las diferentes areas, permitiendo agregar un area con su codigo, descripcion y numero de ventanillas
void areasMenu() {
    int opt;
    do {
        printSeparator();
        cout << "     MENU - AREAS" << endl;
        printSeparator();
        cout << "1. Agregar" << endl;
        cout << "2. Modificar cantidad de ventanillas" << endl;
        cout << "3. Eliminar" << endl;
        cout << "4. Regresar" << endl;

        opt = readOption(1, 4);

        if (opt == 1) {
            try {
                string code = readString("Codigo: ");
                if (code.empty())
                    throw runtime_error("El codigo no puede estar vacio.");
                if (findAreaByCode(code))
                    throw runtime_error("Ya existe un area con ese codigo.");

                string desc = readString("Descripcion: ");
                if (desc.empty())
                    throw runtime_error("La descripcion no puede estar vacia.");

                int windnum = readInt("Ventanillas: ");
                if (windnum < 1)
                    throw out_of_range("Debe haber al menos 1 ventanilla.");

                areas.append(new area(code, desc, windnum));
                cout << "Area agregada." << endl;

            }
            catch (invalid_argument&) {
                cout << "Error: ingrese un numero valido para las ventanillas." << endl;
            }
            catch (out_of_range& e) {
                cout << "Error: " << e.what() << endl;
            }
            catch (runtime_error& e) {
                cout << "Error: " << e.what() << endl;
            }

        }
        else if (opt == 2) {
            try {

            }
            catch (invalid_argument&) {
                cout << "Error: ingrese un numero valido." << endl;
            }
            catch (out_of_range& e) {
                cout << "Error: " << e.what() << endl;
            }
            catch (runtime_error& e) {
                cout << "Error: " << e.what() << endl;
            }

        }
        else if (opt == 3) {
            try {
            }
            catch (invalid_argument&) {
                cout << "Error: ingrese un numero valido." << endl;
            }
            catch (out_of_range& e) {
                cout << "Error: " << e.what() << endl;
            }
            catch (runtime_error& e) {
                cout << "Error: " << e.what() << endl;
            }
        }

    } while (opt != 4);
}


// menu de los servicios disponibles, permitiendo agregar un servicio con su descripcion, prioridad y area asociada
void servicesMenu() {
    int opt;
    do {
        printSeparator();
        cout << "       MENU - SERVICIOS DISPONIBLES" << endl;
        printSeparator();
        cout << "1. Agregar" << endl;
        cout << "2. Eliminar" << endl;
        cout << "3. Reordenar" << endl;
        cout << "4. Regresar" << endl;

        opt = readOption(1, 4);

        if (opt == 1) {
            try {
                if (areas.getSize() == 0)
                    throw runtime_error("No hay areas disponibles.");

                string desc = readString("Descripcion: ");
                if (desc.empty())
                    throw runtime_error("La descripcion no puede estar vacia.");

                cout << "Areas disponibles:" << endl;
                areas.goToStart();
                while (!areas.atEnd()) {
                    area* a = areas.getElement();
                    cout << "  " << a->getCode() << " - " << a->getDescription() << endl;
                    areas.next();
                }

                string areaCode = readString("Codigo de area: ");
                if (!findAreaByCode(areaCode))
                    throw runtime_error("Area no encontrada.");

                int prior = readInt("Prioridad (1-9): ");
                if (prior < 1 || prior > 9)
                    throw out_of_range("La prioridad debe estar entre 1 y 9.");

                services.append(new service(desc, prior, areaCode));
                cout << "Servicio agregado." << endl;

            }
            catch (invalid_argument&) {
                cout << "Error: ingrese un numero valido para la prioridad." << endl;
            }
            catch (out_of_range& e) {
                cout << "Error: " << e.what() << endl;
            }
            catch (runtime_error& e) {
                cout << "Error: " << e.what() << endl;
            }

        }
        else if (opt == 2) {
            try {
                if (services.getSize() == 0)
                    throw runtime_error("No hay servicios registrados.");

                services.goToStart();
                int serviceI = 0;
                while (!services.atEnd()) {
                    service* s = services.getElement();
                    cout << "  " << (serviceI + 1) << ". " << *s << endl;
                    services.next();
                    serviceI++;
                }

                int serviceS = readInt("Seleccione servicio a eliminar: ");
                if (serviceS < 1 || serviceS > services.getSize())
                    throw out_of_range("Servicio invalido.");

                services.goToPos(serviceS - 1);
                service* s = services.getElement();
                services.remove();
                delete s;
                clearAllQueues();
                cout << "Servicio eliminado y colas limpiadas." << endl;

            }
            catch (invalid_argument&) {
                cout << "Error: ingrese un numero valido." << endl;
            }
            catch (out_of_range& e) {
                cout << "Error: " << e.what() << endl;
            }
            catch (runtime_error& e) {
                cout << "Error: " << e.what() << endl;
            }

        }
        else if (opt == 3) {
            try {
                if (services.getSize() < 2)
                    throw runtime_error("Se necesitan al menos 2 servicios para reordenar.");

                services.goToStart();
                int serviceI = 0;
                while (!services.atEnd()) {
                    service* s = services.getElement();
                    cout << "  " << (serviceI + 1) << ". " << s->getDescription() << endl;
                    services.next();
                    serviceI++;
                }

                int ubAnt = readInt("Numero de servicio a reubicar: ");
                if (ubAnt < 1 || ubAnt > services.getSize())
                    throw out_of_range("Posicion origen invalida.");

                int ubDest = readInt("Posicion destino: ");
                if (ubDest < 1 || ubDest > services.getSize())
                    throw out_of_range("Posicion destino invalida.");

                if (ubAnt == ubDest) {
                    cout << "El servicio ya esta en esa posicion." << endl;
                }
                else {
                    services.goToPos(ubAnt - 1);
                    service* s = services.getElement();
                    services.remove();
                    services.goToPos(ubDest - 1);
                    services.insert(s);
                    cout << "Servicio reubicado." << endl;
                }

            }
            catch (invalid_argument&) {
                cout << "Error: ingrese un numero valido." << endl;
            }
            catch (out_of_range& e) {
                cout << "Error: " << e.what() << endl;
            }
            catch (runtime_error& e) {
                cout << "Error: " << e.what() << endl;
            }
        }

    } while (opt != 4);

}

//menu de administracion para modificar todos los aspectos del sistema
void adminMenu() {
    int opt;
    do {
        printSeparator();
        cout << "    MENU DE ADMINISTRACION" << endl;
        printSeparator();
        cout << "1. Tipos de usuario" << endl;
        cout << "2. Areas" << endl;
        cout << "3. Servicios disponibles" << endl;
        cout << "4. Limpiar colas y estadisticas" << endl;
        cout << "5. Regresar" << endl;

        opt = readOption(1, 5);

        if (opt == 1) userTypeMenu();
        else if (opt == 2) areasMenu();
        else if (opt == 3) servicesMenu();
        else if (opt == 4) {
            clearAllQueues();
            totalDispensed = 0;
            totalAttended = 0;
            totalWaitTime = 0.0;
            globalConsecutive = 100;
            cout << "Colas y estadisticas limpiadas." << endl;
        }

    } while (opt != 5);
}


//muestra las estadisticas del sistema, incluyendo el total de tiquetes dispensados, 
//clientes atendidos y el tiempo promedio de espera
void showStatistics() {
    printSeparator();
    cout << "       ESTADISTICAS DEL SISTEMA" << endl;
    printSeparator();

    cout << "\n--- General ---" << endl;
    cout << "  Total tiquetes dispensados : " << totalDispensed << endl;
    cout << "  Total tiquetes atendidos : " << totalAttended << endl;
    double avgWait = 0.0;
    if (totalAttended > 0) {
        avgWait = totalWaitTime / totalAttended;
    }
    cout << "  Tiempo promedio de espera : " << avgWait << " s" << endl;

    cout << "\n--- Por servicio ---" << endl;
    if (services.getSize() == 0) {
        cout << "  Sin servicios registrados." << endl;
    }
    else {
        services.goToStart();
        while (!services.atEnd()) {
            service* s = services.getElement();
            cout << "  " << s->getDescription()
                << " Tiquetes solicitados: " << s->getRequestedTickets() << endl;
            services.next();
        }
    }

    cout << "\n--- Por area / ventanilla ---" << endl;
    if (areas.getSize() == 0) {
        cout << " Sin areas registradas." << endl;
    }
    else {
        areas.goToStart();
        while (!areas.atEnd()) {
            area* a = areas.getElement();
            cout << "  Area: " << a->getCode() << " - " << a->getDescription() << endl;
            cout << "    Total dispensados en area: " << a->getTotalDispensed() << endl;
            counter** windows = a->getWindows();
            for (int i = 0; i < a->getNumWindows(); i++)
                cout << "    Ventanilla " << windows[i]->getName()
                << " | Atendidos: " << windows[i]->getAttendedTickets() << endl;
            areas.next();
        }
    }

    waitForKey();

}

//funcion principal que muestra el menu principal y permite al usuario navegar entre las diferentes opciones del sistema
int main() {
    int opt;
    do {
        printSeparator();
        cout << " SISTEMA DE COLAS - MENU PRINCIPAL" << endl;
        printSeparator();
        cout << "1. Estado de las colas" << endl;
        cout << "2. Tiquetes" << endl;
        cout << "3. Atender" << endl;
        cout << "4. Administracion" << endl;
        cout << "5. Estadisticas del sistema" << endl;
        cout << "6. Salir" << endl;

        opt = readOption(1, 6);

        switch (opt) {
        case 1: showQueueStatus();        break;
        case 2: ticketMenu();             break;
        case 3: attendTicket(); waitForKey(); break;
        case 4: adminMenu();              break;
        case 5: showStatistics();         break;
        case 6: cout << "Saliendo del sistema." << endl; break;
        }

    } while (opt != 6);

    while (!userTypes.isEmpty()) {
        userType* u = userTypes.removeMin();
        delete u;
    }
    services.goToStart();
    while (!services.atEnd()) {
        delete services.getElement();
        services.next();
    }
    services.clear();
    areas.goToStart();
    while (!areas.atEnd()) {
        delete areas.getElement();
        areas.next();
    }
    areas.clear();

    return 0;
}