#ifndef ROUTER_H
#define ROUTER_H

#include <string>
#include <map>
#include <vector>
#include <limits>

/**
 * Estructura que representa una entrada en la tabla de enrutamiento
 */
struct RoutingEntry {
    int cost;                      // Costo total para llegar al destino
    std::vector<std::string> path; // Camino completo desde origen hasta destino

    RoutingEntry() : cost(std::numeric_limits<int>::max()) {}
    RoutingEntry(int c, const std::vector<std::string>& p) : cost(c), path(p) {}
};

/**
 * Clase Router - Representa un enrutador en la red
 */
class Router {
private:
    std::string name;
    // Tabla de enrutamiento: destino -> información de ruta
    std::map<std::string, RoutingEntry> routingTable;

public:
    // Constructor
    explicit Router(const std::string& routerName);

    // Getters
    std::string getName() const;
    const std::map<std::string, RoutingEntry>& getRoutingTable() const;

    // Métodos para manipular la tabla de enrutamiento
    void addRoute(const std::string& destination, int cost,
                  const std::vector<std::string>& path);
    void removeRoute(const std::string& destination);
    void clearRoutingTable();

    // Métodos de consulta
    int getCostTo(const std::string& destination) const;
    std::vector<std::string> getPathTo(const std::string& destination) const;
    bool hasRouteTo(const std::string& destination) const;

    // Método para imprimir la tabla de enrutamiento
    void printRoutingTable() const;
};

#endif // ROUTER_H

