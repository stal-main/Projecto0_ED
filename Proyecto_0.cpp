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

void attendTicket();
void userTypeMenu();
void areasMenu();
void servicesMenu();

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
        if (a->getCode() == code) {
            return a;
        }
        areas.next();
    }
    return nullptr;
}

//genera un nuevo tiquete para un usuario y servicio segun lo seleccionado, 
//asignandole un codigo que depende del codigo del area y un numero consecutivo,
ticket* generateTicket(userType& user, service& serv) {
    int PT = user.getPriority() * 10 + serv.getPriority();
    string code = serv.getAreaCode() + to_string(globalConsecutive++);
    user.addRequest();
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

        for (int i = 0; i < a->getNumWindows(); i++) {
            cout << "    " << *windows[i] << endl;
        }
        cout << endl << "  Tiquetes en cola: " << a->getQueueSize() << endl;

        if (a->queueIsEmpty()) {

            cout << "ninguno";
        }
        else {

            for (int i = 0; i < a->getQueueSize(); i++) {
                cout << a->peekTicketAt(i)->getCode() << " ";
            }
        }

        cout << endl;

        areas.next();
    }

    waitForKey();
}

//  2. Tickets menu
void ticketMenu() {
    int opt;
    do {
        printSeparator();
        cout << "           MENU DE TIQUETES" << endl;
        printSeparator();
        cout << "1. Seleccionar tipo de cliente y servicio" << endl;
        cout << "2. Regresar" << endl;

        opt = readOption(1, 2);

        if (opt == 1) {
            try {
                if (userTypes.isEmpty())
                    throw runtime_error("No hay tipos de usuario registrados.");
                if (services.getSize() == 0)
                    throw runtime_error("No hay servicios registrados.");
                if (areas.getSize() == 0)
                    throw runtime_error("No hay areas registradas.");

                LinkedList<userType*> temp;
                int count = 0;
                cout << "\nTipos de usuario disponibles:" << endl;
                while (!userTypes.isEmpty()) {
                    userType* u = userTypes.removeMin();
                    temp.append(u);
                    cout << "  " << (count + 1) << ". " << u->getDescription() << endl;
                    count++;
                }
                temp.goToStart();
                while (!temp.atEnd()) {
                    userType* u = temp.getElement();
                    userTypes.insert(u, u->getPriority());
                    temp.next();
                }

                int userT = readInt("Seleccione tipo de usuario: ");
                if (userT < 1 || userT > count)
                    throw out_of_range("Tipo de usuario invalido.");

                temp.goToPos(userT - 1);
                userType* selectedUser = temp.getElement();
                cout << "\nServicios disponibles:" << endl;
                services.goToStart();
                int serviceI = 0;
                while (!services.atEnd()) {
                    service* s = services.getElement();
                    cout << "  " << (serviceI + 1) << ". " << s->getDescription()
                        << " [Area: " << s->getAreaCode() << "]" << endl;
                    services.next();
                    serviceI++;
                }

                int serviceT = readInt("Seleccione servicio: ");
                if (serviceT < 1 || serviceT > services.getSize())
                    throw out_of_range("Servicio invalido.");

                services.goToPos(serviceT - 1);
                service* selectedService = services.getElement();

                area* a = findAreaByCode(selectedService->getAreaCode());
                if (!a) throw runtime_error("Area del servicio no encontrada.");

                ticket* t = generateTicket(*selectedUser, *selectedService);
                a->addTicket(t);

                printSeparator();
                cout << "  TIQUETE GENERADO" << endl;
                printSeparator();
                cout << "  Codigo: " << t->getCode() << endl;
                cout << "  Prioridad: " << t->getPriority() << endl;
                cout << "  Servicio: " << selectedService->getDescription() << endl;
                cout << "  Area: " << a->getCode() << " - " << a->getDescription() << endl;
                cout << "  En cola: " << a->getQueueSize() << " tiquete(s)" << endl;
                printSeparator();

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

    } while (opt != 2);
}

//  3. Atender ticket
void attendTicket() {
    try {
        if (areas.getSize() == 0)
            throw runtime_error("No hay areas registradas.");

        cout << "\nAreas disponibles:" << endl;
        areas.goToStart();
        int areasI = 0;
        while (!areas.atEnd()) {
            area* a = areas.getElement();
            cout << "  " << (areasI + 1) << ". " << a->getCode()
                << " - " << a->getDescription() << endl;
            areas.next();
            areasI++;
        }

        int areaT = readInt("Seleccione area: ");
        if (areaT < 1 || areaT > areas.getSize())
            throw out_of_range("Area invalida.");

        areas.goToPos(areaT - 1);
        area* a = areas.getElement();

        cout << "Ventanillas del area " << a->getCode() << ":" << endl;
        counter** windows = a->getWindows();
        for (int i = 0; i < a->getNumWindows(); i++)
            cout << "  " << (i + 1) << ". " << windows[i]->getName() << endl;

        int windowS = readInt("Seleccione ventanilla: ");
        if (windowS < 1 || windowS > a->getNumWindows())
            throw out_of_range("Ventanilla invalida.");

        counter* w = windows[windowS - 1];

        if (w->isBusy()) {
            cout << "Advertencia: la ventanilla " << w->getName()
                << " ya esta atendiendo el tiquete " << w->getActualTicket()->getCode()
                << ". Se reemplazara con el siguiente en cola." << endl;
        }

        if (a->queueIsEmpty())
            throw runtime_error("No hay usuarios en espera en el area " + a->getCode() + ".");

        ticket* t = a->getNextTicket();
        w->attendTicket(t);

        double waitTime = t->getWaitTime();
        totalAttended++;
        totalWaitTime += waitTime;
        a->addWaitTime(waitTime);

        cout << "\nAtendiendo tiquete: " << t->getCode()
            << " en ventanilla " << w->getName() << endl;

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


//menu para agregar o eliminar tipos de usuario, validando que la descripcion no este vacia 
//y que la prioridad sea un numero entero mayor a 0
void userTypeMenu() {
    int opt;
    do {
        printSeparator();
        cout << "       MENU - TIPOS DE USUARIO" << endl;
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
                if (userTypes.isEmpty())
                    throw runtime_error("No hay tipos de usuario registrados.");
                LinkedList<userType*> temp;
                int count = 0;
                while (!userTypes.isEmpty()) {
                    userType* u = userTypes.removeMin();
                    temp.append(u);
                    cout << "  " << (count + 1) << ". " << u->getDescription()
                        << " (Prioridad: " << u->getPriority() << ")" << endl;
                    count++;
                }

                int tipo = -1;

                try {
                    tipo = readInt("Seleccione tipo a eliminar: ");
                }
                catch (invalid_argument&) {
                    
                    temp.goToStart();

                    while (!temp.atEnd()) {

                        userType* u = temp.getElement();

                        userTypes.insert(u, u->getPriority());

                        temp.next();
                    }

                    cout << "Error: ingrese un numero valido." << endl;
                    continue;
                }

                
                if (tipo < 1 || tipo > count) {

                    temp.goToStart();

                    while (!temp.atEnd()) {

                        userType* u = temp.getElement();
                               
                        userTypes.insert(u, u->getPriority());

                        temp.next();
                    }

                    throw out_of_range("Opcion invalida.");
                }

                cout << "Advertencia: se eliminaran todos los tiquetes de todas las colas." << endl;

                string resp = readString("Confirmar eliminacion? (s/n): ");

                if (resp != "s" && resp != "S") {

                    temp.goToStart();

                    while (!temp.atEnd()) {

                        userType* u = temp.getElement();

                        userTypes.insert(u, u->getPriority());

                        temp.next();
                    }

                    cout << "Operacion cancelada." << endl;

                    continue;
                }

                temp.goToStart();

                int i = 1;

                while (!temp.atEnd()) {

                    userType* u = temp.getElement();

                    if (i == tipo) {

                        delete u;
                    }
                    else {

                        userTypes.insert(u, u->getPriority());
                    }

                    temp.next();

                    i++;
                }
            
                clearAllQueues();
                cout << "Tipo eliminado y colas limpiadas." << endl;

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
        cout << "           MENU - AREAS" << endl;
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
                if (areas.getSize() == 0)
                    throw runtime_error("No hay areas registradas.");

                areas.goToStart();
                int areaI = 0;
                while (!areas.atEnd()) {
                    area* a = areas.getElement();
                    cout << "  " << (areaI + 1) << ". " << a->getCode()
                        << " - " << a->getDescription() << endl;
                    areas.next();
                    areaI++;
                }

                int areaS = readInt("Seleccione area: ");
                if (areaS < 1 || areaS > areas.getSize())
                    throw out_of_range("Area invalida.");

                areas.goToPos(areaS - 1);
                area* a = areas.getElement();
                cout << "Ventanillas actuales: " << a->getNumWindows() << endl;

                int windnum = readInt("Nuevo numero de ventanillas: ");
                if (windnum < 1)
                    throw out_of_range("Debe haber al menos 1 ventanilla.");

                a->setNumWindows(windnum);
                cout << "Ventanillas actualizadas." << endl;

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
                if (areas.getSize() == 0)
                    throw runtime_error("No hay areas registradas.");

                areas.goToStart();
                int areaI = 0;
                while (!areas.atEnd()) {
                    area* a = areas.getElement();
                    cout << "  " << (areaI + 1) << ". " << a->getCode()
                        << " - " << a->getDescription() << endl;
                    areas.next();
                    areaI++;
                }

                int areaS = readInt("Seleccione area a eliminar: ");
                if (areaS < 1 || areaS > areas.getSize())
                    throw out_of_range("Area invalida.");

                areas.goToPos(areaS - 1);
                area* a = areas.getElement();
                string aCode = a->getCode();
                LinkedList<int> toDelete;
                services.goToStart();
                int serviceI = 0;
                bool Servicios = false;
                while (!services.atEnd()) {
                    service* s = services.getElement();
                    if (s->getAreaCode() == aCode) {
                        if (!Servicios) {
                            cout << "Los siguientes servicios seran eliminados:" << endl;
                            Servicios = true;
                        }
                        cout << "  - " << s->getDescription() << endl;
                        toDelete.append(serviceI);
                    }
                    services.next();
                    serviceI++;
                }

                string resp = readString("Confirmar eliminacion? (s/n): ");
                if (resp != "s" && resp != "S")
                    throw runtime_error("Operacion cancelada.");
                for (int i = toDelete.getSize() - 1; i >= 0; i--) {
                    toDelete.goToPos(i);
                    int pos = toDelete.getElement();
                    services.goToPos(pos);
                    service* s = services.getElement();
                    services.remove();
                    delete s;
                }

                areas.goToPos(areaS - 1);
                areas.remove();
                delete a;
                cout << "Area eliminada." << endl;

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

                cout << "Advertencia: se eliminaran todos los tiquetes de todas las colas." << endl;
                
                string resp = readString("Confimar eliminacion? (s/n): ");

                if (resp != "s" && resp != "S") {

                    throw runtime_error("Operacion cancelada.");
                }

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
        cout << "        MENU DE ADMINISTRACION" << endl;
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

            areas.goToStart();

            while (!areas.atEnd()) {

                areas.getElement()->resetStats();
                areas.next();
            }

            services.goToStart();

            while (!services.atEnd()) {

                services.getElement()->resetStats();

                services.next();
            }

            LinkedList<userType*> temp;

            while (!userTypes.isEmpty()) {

                userType* u = userTypes.removeMin();

                u->resetStats();

                temp.append(u);
            }

            temp.goToStart();

            while (!temp.atEnd()) {

                userType* u = temp.getElement();

                userTypes.insert(u, u->getPriority());

                temp.next();
            }

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
            cout << "    Tiempo promedio de espera: " << a->getAvgWaitTime() << " s" << endl;
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
        cout << "   SISTEMA DE COLAS - MENU PRINCIPAL" << endl;
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

    // Cleanup
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