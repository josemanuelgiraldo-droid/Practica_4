#include "router.h"
#include <iostream>
#include <iomanip>

Router::Router(const std::string& routerName) : name(routerName) {
    // Agregar ruta a sí mismo con costo 0
    std::vector<std::string> selfPath = {routerName};
    routingTable[routerName] = RoutingEntry(0, selfPath);
}

std::string Router::getName() const {
    return name;
}

const std::map<std::string, RoutingEntry>& Router::getRoutingTable() const {
    return routingTable;
}

void Router::addRoute(const std::string& destination, int cost,
                      const std::vector<std::string>& path) {
    routingTable[destination] = RoutingEntry(cost, path);
}

void Router::removeRoute(const std::string& destination) {
    if (destination != name) { // No se puede eliminar la ruta a sí mismo
        routingTable.erase(destination);
    }
}

void Router::clearRoutingTable() {
    routingTable.clear();
    // Restaurar ruta a sí mismo
    std::vector<std::string> selfPath = {name};
    routingTable[name] = RoutingEntry(0, selfPath);
}

int Router::getCostTo(const std::string& destination) const {
    auto it = routingTable.find(destination);
    if (it != routingTable.end()) {
        return it->second.cost;
    }
    return -1; // Indica que no hay ruta
}

std::vector<std::string> Router::getPathTo(const std::string& destination) const {
    auto it = routingTable.find(destination);
    if (it != routingTable.end()) {
        return it->second.path;
    }
    return std::vector<std::string>(); // Retorna vector vacío si no hay ruta
}

bool Router::hasRouteTo(const std::string& destination) const {
    return routingTable.find(destination) != routingTable.end();
}

void Router::printRoutingTable() const {
    std::cout << "\n=== Tabla de Enrutamiento de " << name << " ===" << std::endl;
    std::cout << std::setw(15) << "Destino"
              << std::setw(10) << "Costo"
              << "  Camino" << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    for (const auto& entry : routingTable) {
        std::cout << std::setw(15) << entry.first
                  << std::setw(10) << entry.second.cost << "  ";

        // Imprimir el camino
        for (size_t i = 0; i < entry.second.path.size(); ++i) {
            std::cout << entry.second.path[i];
            if (i < entry.second.path.size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::string(60, '=') << std::endl;
}
