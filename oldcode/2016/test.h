#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

typedef struct _graphSt *graph;
typedef struct _vertexSt *vertex;
typedef uint32_t u32;
typedef struct _neighb_t  *list_t;

graph newGraph();
int destroyGraph(graph g);
u32 vertexColor(vertex x);
u32 vertexGrade(vertex x);
u32 vertexName(vertex x);
void printVertexNeighs(vertex x);

u32 vertexCount(graph g);
u32 edgeCount(graph g);
u32 colourVertexCount(graph g, u32 i);
u32 printColourVertices(graph g, u32 i);
u32 colorAmount(graph g);
vertex ithOrderVertex(graph g, u32 i);
vertex ithNeigh(graph g, vertex x, u32 i);

int chidos(graph g);
u32 greedy(graph g);

void naturalOrder(graph g);
void welshPowellOrder(graph g);
void restrictedRandomReorder(graph g);
void bigToSmall(graph g);
void smallToBig(graph g);
void revert(graph g);
void specificOrder(graph g,u32* x);
