#include "network.h"
#include <iostream>
#include <string>
#include <limits>

void showMenu() {
    std::cout << "\n╔════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║   SIMULADOR DE RED DE ENRUTADORES             ║" << std::endl;
    std::cout << "╠════════════════════════════════════════════════╣" << std::endl;
    std::cout << "║  1. Agregar enrutador                          ║" << std::endl;
    std::cout << "║  2. Eliminar enrutador                         ║" << std::endl;
    std::cout << "║  3. Agregar enlace                             ║" << std::endl;
    std::cout << "║  4. Eliminar enlace                            ║" << std::endl;
    std::cout << "║  5. Actualizar costo de enlace                 ║" << std::endl;
    std::cout << "║  6. Mostrar topología de la red                ║" << std::endl;
    std::cout << "║  7. Mostrar tablas de enrutamiento             ║" << std::endl;
    std::cout << "║  8. Consultar costo de envío de paquete        ║" << std::endl;
    std::cout << "║  9. Consultar camino de envío de paquete       ║" << std::endl;
    std::cout << "║ 10. Cargar red desde archivo                   ║" << std::endl;
    std::cout << "║ 11. Guardar red en archivo                     ║" << std::endl;
    std::cout << "║ 12. Generar red aleatoria                      ║" << std::endl;
    std::cout << "║ 13. Cargar ejemplo del PDF                     ║" << std::endl;
    std::cout << "║  0. Salir                                      ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════╝" << std::endl;
    std::cout << "Seleccione una opción: ";
}

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void loadExampleNetwork(Network& network) {
    std::cout << "\nCargando ejemplo del PDF (Figura 1)..." << std::endl;

    // Crear la red del ejemplo: A, B, C, D
    network.addRouter("A");
    network.addRouter("B");
    network.addRouter("C");
    network.addRouter("D");

    // Agregar enlaces con los costos del ejemplo
    network.addLink("A", "B", 4);
    network.addLink("A", "C", 10);
    network.addLink("B", "C", 3);
    network.addLink("B", "D", 1);
    network.addLink("C", "D", 2);

    std::cout << "Red de ejemplo cargada exitosamente." << std::endl;
}

int main() {
    Network network;
    int option;

    std::cout << "==================================================" << std::endl;
    std::cout << "    Bienvenido al Simulador de Red de Enrutadores" << std::endl;
    std::cout << "    Universidad de Antioquia - Práctica 4" << std::endl;
    std::cout << "==================================================" << std::endl;

    do {
        showMenu();
        std::cin >> option;

        if (std::cin.fail()) {
            clearInputBuffer();
            std::cout << "Error: Ingrese un número válido." << std::endl;
            continue;
        }
        clearInputBuffer();

        switch (option) {
        case 1: { // Agregar enrutador
            std::string name;
            std::cout << "Ingrese el nombre del enrutador: ";
            std::getline(std::cin, name);
            network.addRouter(name);
            break;
        }

        case 2: { // Eliminar enrutador
            std::string name;
            std::cout << "Ingrese el nombre del enrutador a eliminar: ";
            std::getline(std::cin, name);
            network.removeRouter(name);
            break;
        }

        case 3: { // Agregar enlace
            std::string r1, r2;
            int cost;
            std::cout << "Ingrese el nombre del primer enrutador: ";
            std::getline(std::cin, r1);
            std::cout << "Ingrese el nombre del segundo enrutador: ";
            std::getline(std::cin, r2);
            std::cout << "Ingrese el costo del enlace: ";
            std::cin >> cost;
            clearInputBuffer();
            network.addLink(r1, r2, cost);
            break;
        }

        case 4: { // Eliminar enlace
            std::string r1, r2;
            std::cout << "Ingrese el nombre del primer enrutador: ";
            std::getline(std::cin, r1);
            std::cout << "Ingrese el nombre del segundo enrutador: ";
            std::getline(std::cin, r2);
            network.removeLink(r1, r2);
            break;
        }

        case 5: { // Actualizar costo de enlace
            std::string r1, r2;
            int newCost;
            std::cout << "Ingrese el nombre del primer enrutador: ";
            std::getline(std::cin, r1);
            std::cout << "Ingrese el nombre del segundo enrutador: ";
            std::getline(std::cin, r2);
            std::cout << "Ingrese el nuevo costo del enlace: ";
            std::cin >> newCost;
            clearInputBuffer();
            network.updateLinkCost(r1, r2, newCost);
            break;
        }

        case 6: { // Mostrar topología
            network.printNetwork();
            break;
        }

        case 7: { // Mostrar tablas de enrutamiento
            network.printAllRoutingTables();
            break;
        }

        case 8: { // Consultar costo
            std::string source, dest;
            std::cout << "Ingrese el enrutador origen: ";
            std::getline(std::cin, source);
            std::cout << "Ingrese el enrutador destino: ";
            std::getline(std::cin, dest);

            int cost = network.getPacketCost(source, dest);
            if (cost >= 0) {
                std::cout << "\nCosto de envío de " << source << " a " << dest
                          << ": " << cost << std::endl;
            } else {
                std::cout << "\nNo hay ruta disponible entre " << source
                          << " y " << dest << std::endl;
            }
            break;
        }

        case 9: { // Consultar camino
            std::string source, dest;
            std::cout << "Ingrese el enrutador origen: ";
            std::getline(std::cin, source);
            std::cout << "Ingrese el enrutador destino: ";
            std::getline(std::cin, dest);

            std::vector<std::string> path = network.getPacketPath(source, dest);
            if (!path.empty()) {
                std::cout << "\nCamino optimo de " << source << " a " << dest << ":\n";
                for (size_t i = 0; i < path.size(); ++i) {
                    std::cout << path[i];
                    if (i < path.size() - 1) {
                        std::cout << " -> ";
                    }
                }
                std::cout << "\nCosto total: " << network.getPacketCost(source, dest) << std::endl;
            } else {
                std::cout << "\nNo hay ruta disponible entre " << source
                          << " y " << dest << std::endl;
            }
            break;
        }

        case 10: { // Cargar desde archivo
            std::string filename;
            std::cout << "Ingrese el nombre del archivo: ";
            std::getline(std::cin, filename);
            network.loadFromFile(filename);
            break;
        }

        case 11: { // Guardar en archivo
            std::string filename;
            std::cout << "Ingrese el nombre del archivo: ";
            std::getline(std::cin, filename);
            network.saveToFile(filename);
            break;
        }

        case 12: { // Generar red aleatoria
            int numRouters;
            double linkProb;
            int minCost, maxCost;

            std::cout << "Ingrese el numero de enrutadores: ";
            std::cin >> numRouters;
            std::cout << "Ingrese la probabilidad de enlace (0.0 - 1.0): ";
            std::cin >> linkProb;
            std::cout << "Ingrese el costo minimo de enlaces: ";
            std::cin >> minCost;
            std::cout << "Ingrese el costo maximo de enlaces: ";
            std::cin >> maxCost;
            clearInputBuffer();

            network.generateRandom(numRouters, linkProb, minCost, maxCost);
            break;
        }

        case 13: { // Cargar ejemplo del PDF
            loadExampleNetwork(network);
            break;
        }

        case 0: { // Salir
            std::cout << "\nGracias por usar el simulador de red de enrutadores." << std::endl;
            std::cout << "Hasta pronto!" << std::endl;
            break;
        }

        default: {
            std::cout << "Opcion no valida. Por favor intente nuevamente." << std::endl;
            break;
        }
        }

    } while (option != 0);

    return 0;
}
