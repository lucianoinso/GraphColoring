#include "JonSnow.h"
#include <time.h>
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
}2
*/
/*
    En caso de error debe devolver NULL
    Errores posibles:
    * Falla en alocar memoria
    * Formato de entrada no valido
*/
WinterIsHere WinterIsComing() {
    char input[MAX_LINE];
    char *line = malloc(sizeof(char)*4096);;
    int vCount, eCount;

    // Descartamos las primeras lineas que comienzan con 'c'
    do {
        if(fgets(input, MAX_LINE, stdin) == NULL) {
            return NULL;
        }
        sscanf(input, "%s", line);
    }
    while(*line == 'c');
    free(line);

    // Parseamos la linea que empieza con 'p edge' para obtener la cantidad
    // de vertices y aristas
    sscanf(input, "p edge %d %d", &vCount, &eCount);

//    printf("Vertices count: %d\n", vCount);
//    printf("Edges count: %d", eCount);
//    printf("\n");

    WinterIsHere W = createGraph(vCount, eCount);

    u32 v1name, v2name;
    vertex v1 = NULL;
    vertex v2 = NULL;

    uint i = 0;
    uint j = 0;

    while(j < W->NumeroDeLados) {
        if(fgets(input, MAX_LINE, stdin) == NULL) {
            return NULL;
        }
        sscanf(input, "e %" SCNu32 " %" SCNu32, &v1name, &v2name);

        // Chequeamos si alguno de los vertices ingresados ya habia sido ingresado en la
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
            set_vertex_grade(v2, get_vertex_grade(v2) + 1);
        } else if (v1 != NULL && v2 == NULL) {
            v2 = add_entry_to_hash_map(i, v2name, 0, 1, W->vertex_hashmap);
            W->orderedVertexArray[i] = v2;
            i++;
            set_vertex_grade(v1, get_vertex_grade(v1) + 1);
        } else { // v1 != NULL && v2 != NULL
            set_vertex_grade(v1, get_vertex_grade(v1) + 1);
            set_vertex_grade(v2, get_vertex_grade(v2) + 1);
        }
        make_vertex_neighbs(v1, v2); // Agregamos los vertices a la lista de vecinos del otro
        j++;
    }

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

u32 Greedy(WinterIsHere W) {
    uint colorMaximo = 1;
    uint colorAux = 0;
    uint *arrayColores = NULL;
    vertex vertexAux; 
    for (uint i = 0 ; i < NumeroDeVertices(W); i++) {
        vertexAux = W->orderedVertexArray[i];  //  Iteramos sobre el arreglo de vertices en el orden en el que estan
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
       // search_and_modify_node_values(vertexAux->tag, vertexAux->name, colorAux, vertexAux->grade,W->vertex_hashmap);
        set_vertex_color(vertexAux, colorAux);
        if (colorMaximo < colorAux)
            colorMaximo = colorAux;
        free(arrayColores);
        arrayColores = NULL;
    }
    W->NumeroDeColoresUsados = colorMaximo;
    return colorMaximo;
}

/*
    Devuelve 1 si W es bipartito, 0 si no.
    Además, si devuelve 1, colorea W con un coloreo propio de dos colores.
*/
int Bipartito(WinterIsHere W) {
    cleanColors(W);
    vertex v, w;
    neighb_t vertex_neigh_list;
    uint j = 0;
    queue q = createQueue();
    // Seteamos isBipartite en verdadero hasta que suceda lo contrario
    int isBipartite = 1;
    while (j < W->NumeroDeVertices && isBipartite){
        // Obtenemos el primer vertice que no este coloreado le ponemos 
        // color 1 y lo agregamos a la cola 'q'
        for (uint i = 0; i < W->NumeroDeVertices; i++){
            if(get_vertex_color(W->orderedVertexArray[i]) == 0){
                v = W->orderedVertexArray[i];
                set_vertex_color(v, 1);
                j++;
                enqueue(q,v);
                break;
            }
        }
        while(!isEmpty(q) && isBipartite){
            v = dequeue(q);
            // Obtenemos el primer vertice de la cola
            vertex_neigh_list = get_vertex_neigh_list(v);
            // Obtenemos los vecinos del vertice y los recorremos dentro del while
            while (vertex_neigh_list != NULL){
                // Obtenemos el vertice desde dentro de la estructura del nodo NeighbSt
                w = vertex_neigh_list->vertex_pt;
                // Si el vertice no esta coloreado continuamos
                if (get_vertex_color(w) == 0){
                    // Encolamos el vertice en la cola 'q' para luego colorear sus vecinos en la proxima iteracion
                    enqueue(q, w);
                    // Sumamos 1 a la cantidad de vertices coloreados
                    j++;
                    // Seteamos el color del nuevo vertice segun el color del vertice que lo agrego
                    // Si el color del vertice que lo agrego es 1 => "3 - 1 = 2", si es 2 => "3 - 2 = 1"
                    set_vertex_color(w, 3 - get_vertex_color(v));
                } else if (get_vertex_color(w) == get_vertex_color(v)) {
                    // Si encontramos en la cola un vertice con el mismo color que
                    // el vertice que lo agrego estamos ante un grafo que no es bipartito
                    // entonces seteamos isBipartite en 0 para que corte los ciclos y
                    // devuelva 0 como resultado de la funcion
                    isBipartite = 0;
                    break;
                }
                // Procedemos al siguiente vertice dentro de la lista de vecinos
                vertex_neigh_list = vertex_neigh_list->next;
            }
        }
    }
    destroyQueue(q);
    return isBipartite;
}

/*
    Devuelve 1 si el coloreo es propio, 0 si no
*/
int isValidColoring(WinterIsHere W){
    vertex v, w;
    neighb_t vertex_neigh_list;
    int isValid = 1;
    for (uint i = 0; i < W->NumeroDeVertices && isValid; i++) {
        v = W->orderedVertexArray[i];
        vertex_neigh_list = get_vertex_neigh_list(v);
        while (vertex_neigh_list != NULL){
            w = vertex_neigh_list->vertex_pt;
            if (get_vertex_color(w) == get_vertex_color(v)){
                isValid = 0;
                break;
            }
            vertex_neigh_list = vertex_neigh_list->next;
        }
    }
    return isValid;
}

void cleanColors(WinterIsHere W) {
    for (uint i = 0; i < W->NumeroDeVertices; i++)
        (W->orderedVertexArray[i])->color = 0;
}

// Funciones para extraer información de datos del grafo

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

/* 
    Devuelve la cantidad de colores usados en el coloreo que tiene en su
    momento W
*/

u32 NumeroDeColores(WinterIsHere W) {
    return W->NumeroDeColoresUsados;
}

u32 IesimoVerticeEnElOrden(WinterIsHere W, u32 i) {
    return get_vertex_tag(W->orderedVertexArray[i]);
}


int cmpNaturalOrder(const void *p, const void *q){
    vertex v1 = *(vertex const *) p;
    vertex v2 = *(vertex const *) q;
    u32 v1name = get_vertex_name(v1);
    u32 v2name = get_vertex_name(v2);
    return (v1name > v2name) - (v1name < v2name);
}

int cmpMinToMax(const void *p, const void *q){
    u32 name1 = *(u32 const *) p;
    u32 name2 = *(u32 const *) q;
    return (name1 > name2) - (name1 < name2);
}

int cmpWelshPowell(const void *p, const void *q){
    vertex v1 = *(vertex const *) p;
    vertex v2 = *(vertex const *) q;
    u32 v1grade = get_vertex_grade(v1);
    u32 v2grade = get_vertex_grade(v2);
    // Ordenado depende el grado de mayor a menor
    return (v2grade > v1grade) - (v2grade < v1grade);
}

// Funciones de ordenación

/*
    Orden segun el nombre del vertice
*/
void OrdenNatural(WinterIsHere W) {
    qsort(W->orderedVertexArray, W->NumeroDeVertices,
          sizeof(W->orderedVertexArray[0]), cmpNaturalOrder);
}

/*
    Mayor a menor segun el grado
*/
void OrdenWelshPowell(WinterIsHere W) {
    qsort(W->orderedVertexArray, W->NumeroDeVertices,
          sizeof(W->orderedVertexArray[0]), cmpWelshPowell);
}

/*
    Funcion que devuelve un numero aleatorio
    (fijarse lo del 0)
*/
uint32_t xorshift32(uint32_t state[static 1])
{
    uint32_t x = state[0];
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    state[0] = x;
    return x;
}


static u32 rand_uint(u32 n, u32 *x) {
    u32 rand_uint_max = 0xFFFFFFFF;
    u32 limit = rand_uint_max - rand_uint_max % n;
    u32 rnd;
    do {
        rnd = xorshift32(x);
    } 
    while (rnd >= limit);
    return rnd % n;
}

void AleatorizarVertices(WinterIsHere W, u32 x) {
    u32 i, j, tmp;
    u32 n = W->NumeroDeVertices;
    u32 *random_order_array = malloc(n*sizeof(u32));

    for (i = 0; i < n; i++)
        random_order_array[i] = get_vertex_name(W->orderedVertexArray[i]);

    qsort(random_order_array, n, sizeof(random_order_array[0]), cmpMinToMax);
    
    for (i = n - 1; i > 0; i--) {
        // Le pasamos la cantidad de elementos del arreglo 'n' y la variable
        // 'x' como seed a rand_uint()
        j = rand_uint(n, &x) % (i + 1);
        tmp = random_order_array[j];
        random_order_array[j] = random_order_array[i];
        random_order_array[i] = tmp;
   }

    for (i = 0; i < n ; i++)
        W->orderedVertexArray[i] = search_node(random_order_array[i],
                                               W->vertex_hashmap);

    free(random_order_array);
}


/*
    Si W esta coloreado con r colores y VC1 son los vertices coloreados
    con 1, VC2 los coloreados con 2, etc, entonces esta funcion ordena
    los vertices poniendo primero los vertices de VCj1, luego los de
    VCj2, etc, donde j1,j2,...,jr se determinan a partir de x de la
    siguiente manera:

    x = 0 -> j1 = r, j2 = 1, j3 = 2, ... , jr = r - 1
    x = 1 -> j1 = r, j2 = r - 1, ... , jr-1 = 2, jr = 1
    x = 2 -> |VCj1| <= |VCj2| <= ... <= |VCjr|
    x = 3 -> |VCj1| >= |VCj2| >= ... >= |VCjr|
    x > 3 -> se usa alguna función de pseudoaleatoriedad que dependa
    deterministicamente de x para generar un orden aleatorio de los
    numeros {1,2,...,j}, obteniendo un orden j1,j2,...,jr.
*/


/*
int cmpMinToMaxColors(const void *p, const void *q){
    vertex v1 = *(vertex const *) p;
    vertex v2 = *(vertex const *) q;
    u32 v1color = get_vertex_color(v1);
    u32 v2color = get_vertex_color(v2);
    return (v1color > v2color) - (v1color < v2color);
}*/

int cmpMaxToMinWithColors(const void *p, const void *q){
    vertex v1 = *(vertex const *) p;
    vertex v2 = *(vertex const *) q;
    u32 v1color = get_vertex_color(v1);
    u32 v2color = get_vertex_color(v2);
    return (v2color > v1color) - (v2color < v1color);
}

void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
}

void orderByRandomBlocksOfColor(WinterIsHere W, u32 x){
    u32 ncolores = NumeroDeColores(W);
    u32 *color_amount = calloc((ncolores + 1),sizeof(u32));
    u32 i, j, tmp;
    u32 n = NumeroDeVertices(W);

    // Recorremos el arreglo para contar los colores
    for (i = 0; i < n; i++)
        color_amount[get_vertex_color(W->orderedVertexArray[i])]++;

    u32 *color_position = malloc((ncolores + 1) * sizeof(u32));
    for (i = 0; i < ncolores + 1; i++)
        color_position[i] = i;
    
    for (i = ncolores - 1; i > 0; i--) {
        // Le pasamos la cantidad de elementos del arreglo 'n' y la variable
        // 'x' como seed a rand_uint()
        j = rand_uint(n, &x) % (i + 1);
        tmp = color_position[j];
        color_position[j] = color_position[i];
        color_position[i] = tmp;
    }

    for (i = 0; i < ncolores ; i++)
        printf("%"SCNu32",", color_position[i]);
    printf("\n");


    /* TODO: check this
    color_position[color_position[0]] = 0;
    for (i = 1; i < ncolores; i++){
        color_position[color_position[i]] = color_position[color_position[i - 1]] + color_amount[color_position[i - 1]];
    }
    */
    for (i = 0; i < ncolores ; i++)
        printf("%"SCNu32",", color_position[i]);
    printf("\n");
    
/*
    vertex *orderedByColourVertexArray = malloc(W->NumeroDeVertices * sizeof(struct VertexSt));
    u32 auxcol;
    for (u32 i = 0; i < W->NumeroDeVertices; i++){
        // Obtenemos el color del vertice que estamos por acomodar, nos fijamos en el arreglo
        // de posiciones donde deberia ir ese vertice segun ese color e incrementamos la posicion
        // en el arreglo de posiciones por color para que ocupe la siguiente posición
        auxcol = get_vertex_color(W->orderedVertexArray[i]);
        orderedByColourVertexArray[color_position[auxcol]] = W->orderedVertexArray[i];
        color_position[auxcol]++;
    }
    // Liberamos el arreglo anterior y asignamos el nuevo arreglo a W
    free(W->orderedVertexArray);
    W->orderedVertexArray = orderedByColourVertexArray;
*/

    free(color_position);
    free(color_amount);

}

void orderByColorMaxThenMinToMax(WinterIsHere W){
    // El algoritmo deja sin utilizar la posicion '0' para mayor legibilidad
    // del codigo, asi cada posición del arreglo se corresponde a un color
    // desde el 1 hasta el maximo color
    u32 ncolores = NumeroDeColores(W);
    u32 *color_amount = calloc((ncolores + 1),sizeof(u32));

    // Recorremos el arreglo para contar los colores
    for (u32 i = 0; i < NumeroDeVertices(W); i++)
        color_amount[get_vertex_color(W->orderedVertexArray[i])]++;

    u32 *color_position = malloc((ncolores + 1) * sizeof(u32));
    // Creamos un arreglo de posiciones para saber donde guardar el siguiente vertice
    // de color i, siendo el indice del arreglo el color y el contenido la posición

    color_position[ncolores] = 0;   // Ponemos que los vertices que tienen el color maximo (ncolores)
                                    // Empiezan en la posición 0, por la especificación de la función
                                    // cuando x = 0
    color_position[1] = color_amount[ncolores];  // Ponemos que los vertices de color 1 empiezen a ubicarse
                                                 // desde la posicion siguiente a la cantidad de vertices
                                                 // totales de color ncolores, ya que son los primeros
                                                 // en el arreglo
    for (u32 i = 2; i < (ncolores); i++){
        color_position[i] = color_position[i - 1] + color_amount[i - 1];
        // Ponemos las posiciones iniciales de los vertices que siguen a los del color 1
        // segun la posicion del color anterior mas la cantidad de vertices esperados
        // del color anterior
    }

    vertex *orderedByColourVertexArray = malloc(W->NumeroDeVertices * sizeof(struct VertexSt));
    u32 auxcol;
    for (u32 i = 0; i < W->NumeroDeVertices; i++){
        // Obtenemos el color del vertice que estamos por acomodar, nos fijamos en el arreglo
        // de posiciones donde deberia ir ese vertice segun ese color e incrementamos la posicion
        // en el arreglo de posiciones por color para que ocupe la siguiente posición
        auxcol = get_vertex_color(W->orderedVertexArray[i]);
        orderedByColourVertexArray[color_position[auxcol]] = W->orderedVertexArray[i];
        color_position[auxcol]++;
    }
    // Liberamos el arreglo anterior y asignamos el nuevo arreglo a W
    free(W->orderedVertexArray);
    W->orderedVertexArray = orderedByColourVertexArray;

    /*
    for (u32 i = 0; i < W->NumeroDeVertices; i++)
        orderedByColourVertexArray[i] = W->orderedVertexArray[i];
    
    for (u32 i = 0; i < 300; i++)
        print_vertex_data(orderedByColourVertexArray[i]);

    for (u32 i = 0; i < (ncolores + 1); i++)
        printf("%"SCNu32":%"SCNu32" - ", i, color_amount[i]);
    
    printf("\n");
    for (u32 i = 1; i < (ncolores + 1); i++)
        printf("Col: %"SCNu32", Start pos: %"SCNu32"\n", i, color_position[i]);
    */

    free(color_amount);
    free(color_position);

}

void orderByColorMaxToMin(WinterIsHere W){
    qsort(W->orderedVertexArray, W->NumeroDeVertices,
          sizeof(W->orderedVertexArray[0]), cmpMaxToMinWithColors);
    dumpOrderedVertexArray(W, stdout);
}

void orderByAmountOfColorMinToMax(WinterIsHere W){
    // El algoritmo deja sin utilizar la posicion '0' para mayor legibilidad
    // del codigo, asi cada posición del arreglo se corresponde a un color
    // desde el 1 hasta el maximo color
    u32 ncolores = NumeroDeColores(W);
    printf("%"SCNu32, ncolores);
/*
    u32 ncolores = NumeroDeColores(W);
    u32 *color_amount = calloc((ncolores + 1),sizeof(u32));

    // Recorremos el arreglo para contar los colores
    for (u32 i = 0; i < NumeroDeVertices(W); i++)
        color_amount[get_vertex_color(W->orderedVertexArray[i])]++;

    u32 *color_position = malloc((ncolores + 1) * sizeof(u32));
    // Creamos un arreglo de posiciones para saber donde guardar el siguiente vertice
    // de color i, siendo el indice del arreglo el color y el contenido la posición

    color_position[ncolores] = 0;   // Ponemos que los vertices que tienen el color maximo (ncolores)
                                    // Empiezan en la posición 0, por la especificación de la función
                                    // cuando x = 0
    color_position[1] = color_amount[ncolores];  // Ponemos que los vertices de color 1 empiezen a ubicarse
                                                 // desde la posicion siguiente a la cantidad de vertices
                                                 // totales de color ncolores, ya que son los primeros
                                                 // en el arreglo
    for (u32 i = 2; i < (ncolores); i++){
        color_position[i] = color_position[i - 1] + color_amount[i - 1];
        // Ponemos las posiciones iniciales de los vertices que siguen a los del color 1
        // segun la posicion del color anterior mas la cantidad de vertices esperados
        // del color anterior
    }

    vertex *orderedByColourVertexArray = malloc(W->NumeroDeVertices * sizeof(struct VertexSt));
    u32 auxcol;
    for (u32 i = 0; i < W->NumeroDeVertices; i++){
        // Obtenemos el color del vertice que estamos por acomodar, nos fijamos en el arreglo
        // de posiciones donde deberia ir ese vertice segun ese color e incrementamos la posicion
        // en el arreglo de posiciones por color para que ocupe la siguiente posición
        auxcol = get_vertex_color(W->orderedVertexArray[i]);
        orderedByColourVertexArray[color_position[auxcol]] = W->orderedVertexArray[i];
        color_position[auxcol]++;
    }
    // Liberamos el arreglo anterior y asignamos el nuevo arreglo a W
    free(W->orderedVertexArray);
    W->orderedVertexArray = orderedByColourVertexArray;


//    for (u32 i = 0; i < W->NumeroDeVertices; i++)
//        orderedByColourVertexArray[i] = W->orderedVertexArray[i];
    
//    for (u32 i = 0; i < 300; i++)
//        print_vertex_data(orderedByColourVertexArray[i]);

//    for (u32 i = 0; i < (ncolores + 1); i++)
//        printf("%"SCNu32":%"SCNu32" - ", i, color_amount[i]);
    
//   printf("\n");
//    for (u32 i = 1; i < (ncolores + 1); i++)
//        printf("Col: %"SCNu32", Start pos: %"SCNu32"\n", i, color_position[i]);

    free(color_amount);
    free(color_position);
    */
}

void orderByAmountOfColorMaxToMin(WinterIsHere W){
    u32 ncolores = NumeroDeColores(W);
    printf("%"SCNu32, ncolores);
}

void ReordenManteniendoBloqueColores(WinterIsHere W, u32 x) {
    if (x == 0){
        orderByColorMaxThenMinToMax(W);
    }
    else if (x == 1){
        orderByColorMaxToMin(W);
    }
    else if (x == 2){
        orderByAmountOfColorMinToMax(W);
    }
    else if (x == 3){
        orderByAmountOfColorMaxToMin(W);
    }
    else if (x > 3){
        orderByRandomBlocksOfColor(W, x);
    }
    printf("Cantidad de colores:%"SCNu32"\n", NumeroDeColores(W));
}


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
        fprintf(f, "Pos:%u, ", i);
        print_vertex_data(W->orderedVertexArray[i]);
    }
}

// Main y funciones auxiliares

uint32_t xorshift128(uint32_t state[static 4])
{
    uint32_t t = state[3];
    t ^= t << 11;
    t ^= t >> 8;
    state[3] = state[2]; state[2] = state[1]; state[1] = state[0];
    t ^= state[0];
    t ^= state[0] >> 19;    
    state[0] = t;
    return t;
}



int main() { 
/*
    int ones = 0;
    int zeros = 0;

    srand((unsigned)time(NULL));
    int r;
    for (int i = 0; i < 1000; i++) {
        r = rand() % 2;
        if (r == 1)
            ones++;
        if (r == 0)
            zeros++;
    }
    printf("Zeros: %d, Ones: %d\n", zeros, ones);

    u32 *seed = malloc(sizeof(u32));
    *seed = 24;
    for (int i = 0; i < 10; i++) {
        u32 result = xorshift32(seed) % 20;
//        if (result == 1)
//            ones++;
//        if (result == 0)
//            zeros++;
        printf("resultado: %"SCNu32"\n", result);
    }
//    printf("Zeros: %d, Ones: %d\n", zeros, ones);
    
    return 0;
*/
    printf("Loading...\n");
    WinterIsHere W = WinterIsComing();
    printf("Loading done.\n");
    u32 colores = Greedy(W);
    ReordenManteniendoBloqueColores(W, 5);
  //  printf("Cantidad de colores usados:%"SCNu32"\n", NumeroDeColores(W));
    printf("Coloreo: %"SCNu32"\n", colores);
//    dumpOrderedVertexArray(W, stdout);
//    ReordenManteniendoBloqueColores(W, 1);
//    ReordenManteniendoBloqueColores(W, 2);
//    ReordenManteniendoBloqueColores(W, 3);
//    ReordenManteniendoBloqueColores(W, 24);

    Primavera(W);
    return 0;
/*
    u32 x = 10;
    printf("Dump with seed %"SCNu32" :\n", x);
    AleatorizarVertices(W, x);
    printf("value of [0]:%"SCNu32"\n", get_vertex_name(W->orderedVertexArray[0]));
//    dumpOrderedVertexArray(W, stdout);
    
    x = 11;
    printf("Dump with seed %"SCNu32" :\n", x);
    AleatorizarVertices(W, x);
    printf("value of [0]:%"SCNu32"\n", get_vertex_name(W->orderedVertexArray[0]));
//    dumpOrderedVertexArray(W, stdout);
    
    x = 12;
    printf("Dump with seed %"SCNu32" :\n", x);
    AleatorizarVertices(W, x);
    printf("value of [0]:%"SCNu32"\n", get_vertex_name(W->orderedVertexArray[0]));

    x = 10;
    printf("Dump with seed %"SCNu32" :\n", x);
    AleatorizarVertices(W, x);
    printf("value of [0]:%"SCNu32"\n", get_vertex_name(W->orderedVertexArray[0]));
//    dumpOrderedVertexArray(W, stdout);

    x = 11;
    printf("Dump with seed %"SCNu32" :\n", x);
    AleatorizarVertices(W, x);
    printf("value of [0]:%"SCNu32"\n", get_vertex_name(W->orderedVertexArray[0]));

    x = 12;
    printf("Dump with seed %"SCNu32" :\n", x);
    AleatorizarVertices(W, x);
    printf("value of [0]:%"SCNu32"\n", get_vertex_name(W->orderedVertexArray[0]));
//    dumpOrderedVertexArray(W, stdout);


//    dumpOrderedVertexArray(W, stdout);
    Primavera(W);
    return 0;*/
/*
    u32 colores = 0;
//    int isBipartite;
    int isValid;
    // Do some calculation.
    printf("\nLoading...\n");
    WinterIsHere W = WinterIsComing();
    //printf ("Calculando si es bipartito...\n\n");
//    isBipartite = Bipartito(W);
    printf ("Success!\n\n");
    printf("Running Greedy...\n\n");
    colores = Greedy(W);
    printf("Greedy ended successfuly!\n\n");
    printf("El coloreo calculado por Greedy es de %"SCNu32 " colores\n\n", colores);
//    printf ("El resultado de si es bipartito es: %d\n", isBipartite);
    isValid = isValidColoring(W);
    printf ("El resultado de si es un coloreo propio es: %d\n", isValid);
    cleanColors(W);
    OrdenWelshPowell(W);
    colores = Greedy(W);
    printf("Greedy ended successfuly!\n\n");
    printf("El coloreo calculado por Greedy con orden Welsh-Powell es de %"SCNu32 " colores\n\n", colores);
    isValid = isValidColoring(W);
    printf ("El resultado de si es un coloreo propio es: %d\n", isValid);
    Primavera(W);
    return 0;


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
*/
}