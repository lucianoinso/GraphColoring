#include "JonSnow.h"
#define MAX_LINE 4096

struct WinterSt {
    u32 NumeroDeVertices;
    u32 NumeroDeLados;
    u32 NumeroDeColoresUsados;
    vertex *orderedVertexArray;
    hash_map vertex_hashmap;
};

/* void ImprimirTodosLosVecinos(WinterIsHere W) {
    for (int i = 0; i < W->NumeroDeVertices; i++)
    {
        printf("vertex: %d neighbours are: \n", W->vertexArray[i]->name);
        printVertexNeighs(W->vertexArray[i]);
        printf("\n");
    }
}
*/

WinterIsHere WinterIsComing() {
    char input[MAX_LINE];
    int vCount, eCount;

    printf("Enter vertices count: \n");
    if(fgets(input, MAX_LINE, stdin) == NULL) {
        return NULL;
    }
    sscanf(input, "%d", &vCount);

    printf("Enter edge count: \n");
    if(fgets(input, MAX_LINE, stdin) == NULL) {
        return NULL;
    }
    sscanf(input, "%d", &eCount);

    printf("\n");

    WinterIsHere W = createGraph(vCount, eCount);

    u32 v1name, v2name;
    vertex v1 = NULL;
    vertex v2 = NULL;

    uint i = 0;
    uint j = 0;

    while(j < W->NumeroDeLados) {
    // MANUAL INPUT 
        if(fgets(input, MAX_LINE, stdin) == NULL) {
            return NULL;
        }
        sscanf(input, "%" SCNu32 " %" SCNu32, &v1name, &v2name);
        
        // Chequeamos si el vertice ingresado ya habia sido ingresado en la
        // carga anterior para ahorrar la busqueda ya que notamos que en varios 
        // de los archivos se suele cargar un vertice y luego todos sus vecinos
        // (Solo se aprecia diferencia de 1 segundo para grafos muy grandes
        // cant de vertices > 2 millones, cant de aristas > 10 millones)
        if(v1name != get_vertex_name(v1))
          v1 = search_node(v1name, W->vertex_hashmap);
        
        if(v2name != get_vertex_name(v2))
          v2 = search_node(v2name, W->vertex_hashmap);

        if(v1 == NULL && v2 == NULL) {
            v1 = add_entry_to_hash_map(i, v1name, 0, 1, W->vertex_hashmap);
            W->orderedVertexArray[i] = v1;
            i++;
            v2 = add_entry_to_hash_map(i, v2name, 0, 1, W->vertex_hashmap);
            W->orderedVertexArray[i] = v2;
            i++;
        } else if (v1 == NULL && v2 != NULL) {
            v1 = add_entry_to_hash_map(i, v1name, 0, 1, W->vertex_hashmap);
            W->orderedVertexArray[i] = v1;
            i++;
            set_vertex_grade(get_vertex_grade(v2) + 1, v2);
        } else if (v1 != NULL && v2 == NULL) {
            v2 = add_entry_to_hash_map(i, v2name, 0, 1, W->vertex_hashmap);
            W->orderedVertexArray[i] = v2;
            i++;
            set_vertex_grade(get_vertex_grade(v1) + 1, v1);
        } else { // v1 != NULL && v2 != NULL
            set_vertex_grade(get_vertex_grade(v1) + 1, v1);
            set_vertex_grade(get_vertex_grade(v2) + 1, v2);
        }
        make_vertex_neighbs(v1, v2); // We add the neighbours to the list of each vertex
        j++;
    }
//    W->orderedVertexSize = i;     // DEBUGING PURPOSE
    return W;
}

/*
    Destruye W y libera la memoria alocada. Retorna 1 si todo anduvo bien y 0 si no.
*/
int Primavera(WinterIsHere W) {
    W->vertex_hashmap = destroy_hash_map(W->vertex_hashmap);
    free(W->orderedVertexArray);
    W->orderedVertexArray = NULL;
    int result = (W->vertex_hashmap == NULL);
    free(W);
    W = NULL;
    return result;
}

// Funciones de Coloreo

u32 Greedy(WinterIsHere w) {
    uint colorMaximo = 1;
    uint colorAux = 0;
    uint *arrayColores = NULL;
    vertex vertexAux; 
    for (uint i = 0 ; i < NumeroDeVertices(w) /* + 1 */ ; i++) /* quitar parche de imput */ {
        vertexAux = w->orderedVertexArray[i];  //  Iteramos sobre el arreglo de vertices que en el orden en el que estan dados
        if(get_vertex_grade(vertexAux) >= colorMaximo) {
            arrayColores = calloc(colorMaximo, sizeof(u32));
        } else {
            arrayColores = calloc(get_vertex_grade(vertexAux), sizeof(u32));
        }
        neighb_t neighbHelp  = vertexAux->neighList;
        while(neighbHelp != NULL) {
            if ((get_vertex_color(neighbHelp->vertex_pt) <= get_vertex_grade(vertexAux))
                && (get_vertex_color(neighbHelp->vertex_pt) != 0)){
                    arrayColores[get_vertex_color(neighbHelp->vertex_pt) - 1] = 1;
            }
            neighbHelp = neighbHelp->next;
        }
        colorAux = 0;
        while ((colorAux < get_vertex_grade(vertexAux)) && (colorAux < colorMaximo) && (arrayColores[colorAux] == 1)){
            colorAux++;
        }
        colorAux++;
       // search_and_modify_node_values(vertexAux->tag, vertexAux->name, colorAux, vertexAux->grade,w->vertex_hashmap);
        set_vertex_color(colorAux, vertexAux);
        if (colorMaximo < colorAux)
            colorMaximo = colorAux;
        free(arrayColores);
        arrayColores = NULL;
    }
    return colorMaximo;
}


/*
    Devuelve 1 si W es bipartito, 0 si no.
    Además, si devuelve 1, colorea W con un coloreo propio de dos colores.
*/
int Bipartito(WinterIsHere W) {
    cleanColors(W);
    vertex v;
    for (int i = 0; i < W->NumeroDeVertices; i++){
        v = W->orderedVertexArray[i];
        if(v.color == 0){
            set_vertex_color(1, v);
            
        }
    }
}


// Funciones para extraer información de datos del grafo
void cleanColors(WinterIsHere W) {
    for (uint i = 0; i < W->NumeroDeVertices; i++)
        (W->orderedVertexArray[i])->color = 0;
}

u32 NumeroDeVertices(WinterIsHere W) {
    return W->NumeroDeVertices;
}

u32 NumeroDeLados(WinterIsHere W) {
    return W->NumeroDeLados;
}

u32 NumeroVerticesDeColor(WinterIsHere W, u32 i) {
    int count = 0;
    for (uint j = 0; j < W->NumeroDeVertices; j++) {
        if (get_vertex_color(W->orderedVertexArray[j]) == i) {
            count++;
        }
    }
    return count;
}

/* Devuelve la cantidad de colores usados en el coloreo que tiene en su momento W */
u32 NumeroDeColores(WinterIsHere W) {
    return W->NumeroDeColoresUsados;
}

u32 IesimoVerticeEnElOrden(WinterIsHere W, u32 i) {
    return get_vertex_tag(W->orderedVertexArray[i]);
}


int cmpNaturalOrder(const void *p, const void *q){
    vertex v1 = *(vertex const *) p;
    vertex v2 = *(vertex const *) q;
    return (get_vertex_name(v1) - get_vertex_name(v2));
}

int cmpWelshPowell(const void *p, const void *q){
    vertex v1 = *(vertex const *) p;
    vertex v2 = *(vertex const *) q;
    // Ordenado de mayor a menor
    return (get_vertex_grade(v2) - get_vertex_grade(v1));
}

// Funciones de ordenación
// Orden segun el nombre del vertice
void OrdenNatural(WinterIsHere W) {
    qsort(W->orderedVertexArray, W->NumeroDeVertices, sizeof(W->orderedVertexArray[0]), cmpNaturalOrder);
}

// Mayor a menor grado
void OrdenWelshPowell(WinterIsHere W) {
    qsort(W->orderedVertexArray, W->NumeroDeVertices, sizeof(W->orderedVertexArray[0]), cmpWelshPowell);
}

/* IMPLEMENT
void AleatorizarVertices(WinterIsHere W, u32 x) {
    printf("todo");
}

void ReordenManteniendoBloqueColores(WinterIsHere W, u32 x) {
    printf("todo");
}

*/
// Funciones de los vertices

u32 NombreDelVertice(WinterIsHere W, u32 x) {
    return get_vertex_name(W->orderedVertexArray[x]);
}

u32 ColorDelVertice(WinterIsHere W, u32 x) {
    return get_vertex_color(W->orderedVertexArray[x]);
}

u32 GradoDelVertice(WinterIsHere W, u32 x) {
    return get_vertex_grade(W->orderedVertexArray[x]);
}

u32 IesimoVecino(WinterIsHere W, u32 x, u32 i) {
    vertex v = W->orderedVertexArray[x];
    neighb_t auxList = get_vertex_neigh_list(v);
    uint j = 0;
    u32 tag = 0;
    while(auxList != NULL && j != i) {
        auxList = auxList->next;
        j++;
    }
    tag = get_vertex_tag(auxList->vertex_pt);
    return tag;
}

/*
u32 printColourVertices(WinterIsHere W, u32 i) {
    printf("Vertices de Color %d: ", i);

    int count = 0;
    u32 amount = NumeroVerticesDeColor(W, i);
    
    if(amount == 0) {
        printf("No hay vertices de color %d\n", i);
    } else {
        for (int j = 0; j < W->NumeroDeVertices; j++) {
            if (W->vertexArray[j]->colour == i) {
                if (count < amount - 1)
                    printf("%d,", W->vertexArray[j]->name);
                else
                    printf("%d.\n", W->vertexArray[j]->name);
                count++;
            }
        }
    }
    return amount;
}
*/

WinterIsHere createGraph(u32 vCount, u32 eCount) {
    WinterIsHere W = calloc(1, sizeof(struct WinterSt));
    W->NumeroDeLados = eCount;
    W->NumeroDeVertices = vCount;
    W->vertex_hashmap = create_hash_map(W->NumeroDeVertices);
    W->orderedVertexArray = malloc(W->NumeroDeVertices * sizeof(struct VertexSt));

    return W;
}

void dumpOrderedVertexArray(WinterIsHere W, FILE *f) {
    for (uint i = 0; i < W->NumeroDeVertices; i++) {
        fprintf(f, "Pos:%u, Name: %" SCNu32 "\n", i, get_vertex_name(W->orderedVertexArray[i]));
    }
}

// Main y funciones auxiliares

int main() {
/*
    u32 colores = 0;
    // Do some calculation.
    printf("\nLoading...\n");
    WinterIsHere W = WinterIsComing();

    printf ("Success!\n\n");
    printf("Running Greedy...\n\n");
    colores = Greedy(W);
    printf("Greedy ended successfuly!\n\n");
    printf("El coloreo calculado por Greedy es de %"SCNu32 " colores\n\n", colores);
//    printf("Orden natural (por nombre de menor a mayor): \n");
//    OrdenNatural(W);
//    dumpOrderedVertexArray(W, stdout);
    printf("Ordenando con orden Welsh-Powell\n\n");
    OrdenWelshPowell(W);
//    dumpOrderedVertexArray(W, stdout);
    // Probando orden Welsh Powell
//    printf("El vertice mas vecinero es: ");
//    print_vertex_data(W->orderedVertexArray[0]);
//    printf("El vertice menos vecinero es: ");
//    print_vertex_data(W->orderedVertexArray[W->NumeroDeVertices - 1]);
    printf("Limpiando los colores de los vertices para calcular de nuevo...");
    cleanColors(W);  // para recolorear con Greedy
    printf("Calculando con orden WelshPowell...\n");
    colores = Greedy(W);
    printf("Greedy ended successfuly!\n\n");
    printf("El coloreo calculado por Greedy es de %"SCNu32 " colores\n\n", colores);
//    printf("\n\n");
//    dump_hash_map(W->vertex_hashmap, stdout);
//    printf("\n\n");
//    printf("El vecino 1 de 3 es: %" SCNu32 "\n", IesimoVecino(W, 3, 1));  // vert, vec
//    printf("Numero de vertices con color 0: %" SCNu32 "\n", NumeroVerticesDeColor(W, 1));
//  printf("vertex count: %d\n", getVertexCount(W));
//  printColourVertices(W, 1);
    Primavera(W);
    return 0;
*/
    vertex v1 = create_vertex(1, 1, 0, 0);
    vertex v2 = create_vertex(2, 2, 0, 0);
    vertex v3 = create_vertex(3, 3, 0, 0);
    queue q = createQueue();
    enqueue(q, v2);
    printQueue(q);
    enqueue(q, v1);
    printQueue(q);
    enqueue(q, v3);
    printQueue(q);
    enqueue(q, v1);
    printQueue(q);
    vertex v4 = dequeue(q);
    printQueue(q);
    v4 = dequeue(q);
    printQueue(q);
    v4 = dequeue(q);
    printQueue(q);
    v4 = dequeue(q);
    printQueue(q);

//    v4 = dequeue(q);
//    v4 = dequeue(q);
    if (v4 != NULL)
        printf("Dequeued item is %d\n", get_vertex_tag(v4));

    destroyQueue(q);
    destroy_vertex(v1);
    destroy_vertex(v2);
    destroy_vertex(v3);
    return 0;
}