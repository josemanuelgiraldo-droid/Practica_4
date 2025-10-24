#ifndef NETWORK_H
#define NETWORK_H

#include "router.h"
#include <map>
#include <set>
#include <string>
#include <vector>
#include <limits>

/**
 * Clase Network - Representa una red de enrutadores interconectados
 */
class Network {
private:
    // Mapa de enrutadores: nombre -> objeto Router
    std::map<std::string, Router*> routers;

    // Matriz de adyacencia: representa la topología de la red
    // topology[router1][router2] = costo del enlace directo
    std::map<std::string, std::map<std::string, int>> topology;

    // Métodos auxiliares privados
    void dijkstra(const std::string& source);
    std::vector<std::string> reconstructPath(
        const std::map<std::string, std::string>& previous,
        const std::string& source,
        const std::string& destination) const;

public:
    // Constructor y destructor
    Network();
    ~Network();

    // Gestión de enrutadores
    void addRouter(const std::string& name);
    void removeRouter(const std::string& name);
    bool hasRouter(const std::string& name) const;

    // Gestión de enlaces
    void addLink(const std::string& router1, const std::string& router2, int cost);
    void removeLink(const std::string& router1, const std::string& router2);
    void updateLinkCost(const std::string& router1, const std::string& router2, int newCost);

    // Actualización de tablas de enrutamiento
    void updateAllRoutingTables();

    // Consultas sobre rutas
    int getPacketCost(const std::string& source, const std::string& destination) const;
    std::vector<std::string> getPacketPath(const std::string& source,
                                           const std::string& destination) const;

    // Carga desde archivo
    void loadFromFile(const std::string& filename);

    // Generación aleatoria
    void generateRandom(int numRouters, double linkProbability,
                        int minCost = 1, int maxCost = 10);
    void saveToFile(const std::string& filename) const;

    // Visualización
    void printNetwork() const;
    void printAllRoutingTables() const;

    // Getters
    std::vector<std::string> getRouterNames() const;
};

#endif // NETWORK_H
