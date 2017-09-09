#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "hash_map.h"
#include "queue.h"

typedef uint32_t u32;
typedef struct WinterSt *WinterIsHere;

// Funciones de Construccion/Destruccion del grafo

WinterIsHere WinterIsComing();  // Creates a graph and calculates it's colouring
int Primavera(WinterIsHere W);  // Destroys a graph

// Funciones de Coloreo

u32 Greedy(WinterIsHere W);       // Problablemente mal implementada
int Bipartito(WinterIsHere W);    // Falta implementar

// Funciones para extraer información de datos del grafo

u32 NumeroDeVertices(WinterIsHere W);
u32 NumeroDeLados(WinterIsHere W);
u32 NumeroVerticesDeColor(WinterIsHere W, u32 i); // cant vertices que tienen color i
u32 NumeroDeColores(WinterIsHere W); // cant colores usados en el coloreo que W usa en ese momento
u32 IesimoVerticeEnElOrden(WinterIsHere W, u32 i);

// Funciones de ordenación

void OrdenNatural(WinterIsHere W);      // Falta implementar
void OrdenWelshPowell(WinterIsHere W);  // Falta implementar
void AleatorizarVertices(WinterIsHere W, u32 x);  // Falta implementar
void ReordenManteniendoBloqueColores(WinterIsHere W, u32 x);  // Falta implementar

// Funciones de los vertices

u32 NombreDelVertice(WinterIsHere W, u32 x);
u32 ColorDelVertice(WinterIsHere W, u32 x);
u32 GradoDelVertice(WinterIsHere W, u32 x);
u32 IesimoVecino(WinterIsHere W, u32 x, u32 i);

// Funciones auxiliares
WinterIsHere createGraph(u32 vCount, u32 eCount);
void dumpOrderedVertexArray(WinterIsHere W, FILE *f);
void cleanColors(WinterIsHere W);
int isValidColoring(WinterIsHere W);