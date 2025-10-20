#include "network.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>
#include <cstdlib>
#include <ctime>

Network::Network() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

Network::~Network() {
    for (auto& pair : routers) {
        delete pair.second;
    }
    routers.clear();
}

void Network::addRouter(const std::string& name) {
    if (routers.find(name) == routers.end()) {
        routers[name] = new Router(name);
        topology[name] = std::map<std::string, int>();
        std::cout << "Enrutador '" << name << "' agregado a la red." << std::endl;
    } else {
        std::cout << "El enrutador '" << name << "' ya existe en la red." << std::endl;
    }
}

void Network::removeRouter(const std::string& name) {
    auto it = routers.find(name);
    if (it != routers.end()) {
        delete it->second;
        routers.erase(it);
        topology.erase(name);

        // Eliminar enlaces que conectan a este enrutador
        for (auto& pair : topology) {
            pair.second.erase(name);
        }

        std::cout << "Enrutador '" << name << "' eliminado de la red." << std::endl;
        updateAllRoutingTables();
    } else {
        std::cout << "El enrutador '" << name << "' no existe en la red." << std::endl;
    }
}

bool Network::hasRouter(const std::string& name) const {
    return routers.find(name) != routers.end();
}

void Network::addLink(const std::string& router1, const std::string& router2, int cost) {
    if (!hasRouter(router1) || !hasRouter(router2)) {
        std::cout << "Error: Uno o ambos enrutadores no existen." << std::endl;
        return;
    }

    if (cost <= 0) {
        std::cout << "Error: El costo debe ser positivo." << std::endl;
        return;
    }

    topology[router1][router2] = cost;
    topology[router2][router1] = cost; // Enlace bidireccional

    std::cout << "Enlace agregado: " << router1 << " <-> " << router2
              << " (costo: " << cost << ")" << std::endl;

    updateAllRoutingTables();
}

void Network::removeLink(const std::string& router1, const std::string& router2) {
    if (topology[router1].erase(router2) > 0) {
        topology[router2].erase(router1);
        std::cout << "Enlace eliminado: " << router1 << " <-> " << router2 << std::endl;
        updateAllRoutingTables();
    } else {
        std::cout << "No existe enlace entre " << router1 << " y " << router2 << std::endl;
    }
}

void Network::updateLinkCost(const std::string& router1, const std::string& router2,
                             int newCost) {
    if (topology[router1].find(router2) != topology[router1].end()) {
        topology[router1][router2] = newCost;
        topology[router2][router1] = newCost;
        std::cout << "Costo del enlace actualizado: " << router1 << " <-> " << router2
                  << " (nuevo costo: " << newCost << ")" << std::endl;
        updateAllRoutingTables();
    } else {
        std::cout << "No existe enlace entre " << router1 << " y " << router2 << std::endl;
    }
}

void Network::updateAllRoutingTables() {
    // Calcular rutas más cortas desde cada enrutador usando Dijkstra
    for (const auto& pair : routers) {
        dijkstra(pair.first);
    }
}

void Network::dijkstra(const std::string& source) {
    Router* sourceRouter = routers[source];
    sourceRouter->clearRoutingTable();

    // Estructuras para el algoritmo de Dijkstra
    std::map<std::string, int> distances;
    std::map<std::string, std::string> previous;
    std::set<std::string> unvisited;

    // Inicialización
    for (const auto& pair : routers) {
        distances[pair.first] = std::numeric_limits<int>::max();
        unvisited.insert(pair.first);
    }
    distances[source] = 0;

    // Algoritmo de Dijkstra
    while (!unvisited.empty()) {
        // Encontrar el nodo no visitado con menor distancia
        std::string current;
        int minDist = std::numeric_limits<int>::max();
        for (const std::string& node : unvisited) {
            if (distances[node] < minDist) {
                minDist = distances[node];
                current = node;
            }
        }

        if (minDist == std::numeric_limits<int>::max()) {
            break; // No hay más nodos alcanzables
        }

        unvisited.erase(current);

        // Actualizar distancias a vecinos
        for (const auto& neighbor : topology[current]) {
            const std::string& neighborName = neighbor.first;
            int edgeCost = neighbor.second;

            if (unvisited.find(neighborName) != unvisited.end()) {
                int newDist = distances[current] + edgeCost;
                if (newDist < distances[neighborName]) {
                    distances[neighborName] = newDist;
                    previous[neighborName] = current;
                }
            }
        }
    }

    // Construir tabla de enrutamiento con los caminos completos
    for (const auto& pair : routers) {
        const std::string& destination = pair.first;
        if (distances[destination] != std::numeric_limits<int>::max()) {
            std::vector<std::string> path = reconstructPath(previous, source, destination);
            sourceRouter->addRoute(destination, distances[destination], path);
        }
    }
}

std::vector<std::string> Network::reconstructPath(
    const std::map<std::string, std::string>& previous,
    const std::string& source,
    const std::string& destination) const {

    std::vector<std::string> path;
    std::string current = destination;

    while (current != source) {
        path.push_back(current);
        auto it = previous.find(current);
        if (it == previous.end()) {
            return std::vector<std::string>(); // No hay camino
        }
        current = it->second;
    }
    path.push_back(source);

    std::reverse(path.begin(), path.end());
    return path;
}

int Network::getPacketCost(const std::string& source, const std::string& destination) const {
    auto it = routers.find(source);
    if (it != routers.end()) {
        return it->second->getCostTo(destination);
    }
    return -1;
}

std::vector<std::string> Network::getPacketPath(const std::string& source,
                                                const std::string& destination) const {
    auto it = routers.find(source);
    if (it != routers.end()) {
        return it->second->getPathTo(destination);
    }
    return std::vector<std::string>();
}

void Network::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: No se pudo abrir el archivo '" << filename << "'." << std::endl;
        return;
    }

    // Limpiar red actual
    for (auto& pair : routers) {
        delete pair.second;
    }
    routers.clear();
    topology.clear();

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; // Ignorar líneas vacías y comentarios

        std::istringstream iss(line);
        std::string router1, router2;
        int cost;

        if (iss >> router1 >> router2 >> cost) {
            if (!hasRouter(router1)) addRouter(router1);
            if (!hasRouter(router2)) addRouter(router2);
            addLink(router1, router2, cost);
        }
    }

    file.close();
    std::cout << "Red cargada desde '" << filename << "'." << std::endl;
}

void Network::generateRandom(int numRouters, double linkProbability,
                             int minCost, int maxCost) {
    // Limpiar red actual
    for (auto& pair : routers) {
        delete pair.second;
    }
    routers.clear();
    topology.clear();

    // Crear enrutadores
    for (int i = 0; i < numRouters; ++i) {
        std::string name = "R" + std::to_string(i);
        addRouter(name);
    }

    // Crear enlaces aleatorios
    std::vector<std::string> routerList = getRouterNames();
    for (size_t i = 0; i < routerList.size(); ++i) {
        for (size_t j = i + 1; j < routerList.size(); ++j) {
            double prob = static_cast<double>(std::rand()) / RAND_MAX;
            if (prob < linkProbability) {
                int cost = minCost + std::rand() % (maxCost - minCost + 1);
                addLink(routerList[i], routerList[j], cost);
            }
        }
    }

    std::cout << "Red aleatoria generada con " << numRouters << " enrutadores." << std::endl;
}

void Network::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: No se pudo crear el archivo '" << filename << "'." << std::endl;
        return;
    }

    file << "# Topología de red\n";
    file << "# Formato: Router1 Router2 Costo\n\n";

    std::set<std::pair<std::string, std::string>> written;
    for (const auto& pair : topology) {
        const std::string& router1 = pair.first;
        for (const auto& link : pair.second) {
            const std::string& router2 = link.first;
            int cost = link.second;

            // Evitar escribir el mismo enlace dos veces
            if (written.find({router1, router2}) == written.end() &&
                written.find({router2, router1}) == written.end()) {
                file << router1 << " " << router2 << " " << cost << "\n";
                written.insert({router1, router2});
            }
        }
    }

    file.close();
    std::cout << "Red guardada en '" << filename << "'." << std::endl;
}

void Network::printNetwork() const {
    std::cout << "\n========== TOPOLOGÍA DE LA RED ==========" << std::endl;
    std::cout << "Enrutadores: " << routers.size() << std::endl;

    int linkCount = 0;
    for (const auto& pair : topology) {
        linkCount += pair.second.size();
    }
    linkCount /= 2; // Cada enlace se cuenta dos veces

    std::cout << "Enlaces: " << linkCount << std::endl;
    std::cout << "\nConexiones:" << std::endl;

    std::set<std::pair<std::string, std::string>> printed;
    for (const auto& pair : topology) {
        const std::string& router1 = pair.first;
        for (const auto& link : pair.second) {
            const std::string& router2 = link.first;
            int cost = link.second;

            if (printed.find({router1, router2}) == printed.end() &&
                printed.find({router2, router1}) == printed.end()) {
                std::cout << "  " << router1 << " <-> " << router2
                          << " (costo: " << cost << ")" << std::endl;
                printed.insert({router1, router2});
            }
        }
    }
    std::cout << "=========================================" << std::endl;
}

void Network::printAllRoutingTables() const {
    for (const auto& pair : routers) {
        pair.second->printRoutingTable();
    }
}

std::vector<std::string> Network::getRouterNames() const {
    std::vector<std::string> names;
    for (const auto& pair : routers) {
        names.push_back(pair.first);
    }
    return names;
}
