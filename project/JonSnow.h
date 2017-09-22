/*
    Autor: Luciano Rodriguez
    E-mail: lucianoinso@gmail.com
*/

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "hashtable.h"
#include "queue.h"
#include "qsortCmpFunctions.h"

typedef uint32_t u32;
typedef struct WinterSt *WinterIsHere;

// Funciones de Construcción/Destrucción del grafo
WinterIsHere WinterIsComing();
int Primavera(WinterIsHere W);

// Funciones de Coloreo
u32 Greedy(WinterIsHere W);
int Bipartito(WinterIsHere W);

// Funciones para extraer información de datos del grafo
u32 NumeroDeVertices(WinterIsHere W);
u32 NumeroDeLados(WinterIsHere W);
u32 NumeroVerticesDeColor(WinterIsHere W, u32 i);
u32 NumeroDeColores(WinterIsHere W);
u32 IesimoVerticeEnElOrden(WinterIsHere W, u32 i);

// Funciones de ordenación
void OrdenNatural(WinterIsHere W);
void OrdenWelshPowell(WinterIsHere W);
void AleatorizarVertices(WinterIsHere W, u32 x);
void ReordenManteniendoBloqueColores(WinterIsHere W, u32 x);

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