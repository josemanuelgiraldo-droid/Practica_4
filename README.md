# Simulador de Red de Enrutadores

**Universidad de Antioquia - Facultad de Ingeniería**  
**Práctica 4 - Informática II**

## Descripción

Este proyecto implementa un simulador de red de enrutadores que permite modelar, configurar y analizar redes de computadores. Utiliza el algoritmo de Dijkstra para calcular las rutas más cortas entre enrutadores.

## Características

- ✅ Modelamiento orientado a objetos de enrutadores y redes
- ✅ Uso de contenedores STL (map, vector, set)
- ✅ Algoritmo de Dijkstra para cálculo de rutas óptimas
- ✅ Gestión dinámica de enrutadores y enlaces
- ✅ Actualización automática de tablas de enrutamiento
- ✅ Carga y guardado de topologías desde archivos
- ✅ Generación aleatoria de redes para pruebas
- ✅ Interfaz interactiva por consola

## Estructura del Proyecto

```
.
├── router.h          # Declaración de la clase Router
├── router.cpp        # Implementación de la clase Router
├── network.h         # Declaración de la clase Network
├── network.cpp       # Implementación de la clase Network
├── main.cpp          # Programa principal con menú interactivo
├── Makefile          # Archivo para compilación
└── README.md         # Esta documentación
```

## Compilación

### Opción 1: Usando Makefile (Recomendado)

```bash
make
```

### Opción 2: Compilación manual

```bash
g++ -std=c++11 -Wall -Wextra -O2 -o simulador_red main.cpp router.cpp network.cpp
```

## Ejecución

```bash
./simulador_red
```

O usando make:

```bash
make run
```

## Uso del Programa

### Menú Principal

El programa ofrece un menú interactivo con las siguientes opciones:

1. **Agregar enrutador**: Añade un nuevo enrutador a la red
2. **Eliminar enrutador**: Elimina un enrutador existente
3. **Agregar enlace**: Crea un enlace bidireccional entre dos enrutadores
4. **Eliminar enlace**: Elimina un enlace existente
5. **Actualizar costo de enlace**: Modifica el costo de un enlace
6. **Mostrar topología**: Visualiza la estructura de la red
7. **Mostrar tablas de enrutamiento**: Muestra las tablas de todos los enrutadores
8. **Consultar costo de envío**: Calcula el costo mínimo entre dos enrutadores
9. **Consultar camino de envío**: Muestra la ruta óptima entre dos enrutadores
10. **Cargar red desde archivo**: Carga una topología desde un archivo de texto
11. **Guardar red en archivo**: Guarda la topología actual en un archivo
12. **Generar red aleatoria**: Crea una red aleatoria para pruebas
13. **Cargar ejemplo del PDF**: Carga la red de ejemplo de la práctica

### Formato de Archivo de Topología

Los archivos de topología deben seguir este formato:

```
# Comentarios comienzan con #
Router1 Router2 Costo
A B 4
A C 10
B C 3
```

### Ejemplo de Uso

```bash
# Crear archivo de ejemplo
make ejemplo

# Ejecutar el programa
./simulador_red

# En el menú, seleccionar opción 10 para cargar
# Ingresar: red_ejemplo.txt
```

## Clases Principales

### Clase `Router`

Representa un enrutador individual en la red.

**Atributos principales:**
- `name`: Nombre del enrutador
- `routingTable`: Tabla de enrutamiento con destinos, costos y caminos

**Métodos principales:**
- `addRoute()`: Agrega una ruta a la tabla
- `getCostTo()`: Obtiene el costo a un destino
- `getPathTo()`: Obtiene el camino completo a un destino
- `printRoutingTable()`: Muestra la tabla de enrutamiento

### Clase `Network`

Representa la red completa de enrutadores interconectados.

**Atributos principales:**
- `routers`: Mapa de enrutadores en la red
- `topology`: Matriz de adyacencia con los enlaces y sus costos

**Métodos principales:**
- `addRouter()` / `removeRouter()`: Gestión de enrutadores
- `addLink()` / `removeLink()`: Gestión de enlaces
- `dijkstra()`: Algoritmo para calcular rutas más cortas
- `updateAllRoutingTables()`: Actualiza todas las tablas después de cambios
- `getPacketCost()` / `getPacketPath()`: Consultas sobre rutas
- `loadFromFile()` / `saveToFile()`: Persistencia de datos
- `generateRandom()`: Generación de redes aleatorias

## Ejemplo del PDF

La práctica incluye un ejemplo con 4 enrutadores (A, B, C, D):

```
     10
  A ---- C
  |  \   |
4 |   \  | 2
  |    \ |
  B --- D
    1  /
      / 3
```

Para cargar este ejemplo, usar la opción 13 del menú.

## Algoritmo de Dijkstra

El programa utiliza el algoritmo de Dijkstra para calcular las rutas más cortas:

1. Inicializa todas las distancias como infinito (excepto el origen)
2. Selecciona el nodo no visitado con menor distancia
3. Actualiza las distancias a sus vecinos
4. Marca el nodo como visitado
5. Repite hasta visitar todos los nodos alcanzables

## Contenedores STL Utilizados

- `std::map`: Para tablas de enrutamiento y topología
- `std::vector`: Para almacenar caminos y listas de enrutadores
- `std::set`: Para nodos no visitados en Dijkstra y evitar duplicados
- `std::string`: Para nombres de enrutadores

## Limpieza

Para eliminar archivos compilados:

```bash
make clean
```

## Requisitos

- Compilador C++ con soporte para C++11 o superior
- Make (opcional, para usar el Makefile)

## Autores

Basado en la Práctica 4 de Informática II  
Profesores: Augusto Salazar Jiménez y Jaime Vergara  
Universidad de Antioquia

## Notas

- Los enlaces son bidireccionales por defecto
- Las tablas de enrutamiento se actualizan automáticamente después de cambios
- Los costos deben ser números enteros positivos
- Los nombres de enrutadores distinguen entre mayúsculas y minúsculas
