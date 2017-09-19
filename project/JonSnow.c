#include "JonSnow.h"

#define MAX_LINE 4096

struct WinterSt {
    u32 NumeroDeVertices;
    u32 NumeroDeLados;
    u32 NumeroDeColoresUsados;
    vertex *orderedVertexArray;
    hash_map vertex_hashmap;
};


/*  TODO:
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

    u32 i = 0;
    u32 j = 0;

    while(j < NumeroDeLados(W)) {
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
    Greedy(W);
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
    u32 colorMaximo = 1;
    u32 colorAux = 0;
    u32 *arrayColores = NULL;
    vertex vertexAux; 
    for (u32 i = 0 ; i < NumeroDeVertices(W); i++) {
        vertexAux = W->orderedVertexArray[i];  //  Iteramos sobre el arreglo de vertices en el orden en el que estan
        if(get_vertex_grade(vertexAux) >= colorMaximo) {
            arrayColores = calloc(colorMaximo, sizeof(u32));
        } else {
            arrayColores = calloc(get_vertex_grade(vertexAux), sizeof(u32));
        }
        neighb_t neighbHelp = get_vertex_neigh_list(vertexAux);
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
    u32 j = 0;
    queue q = createQueue();
    // Seteamos isBipartite en verdadero hasta que suceda lo contrario
    int isBipartite = 1;
    while (j < NumeroDeVertices(W) && isBipartite){
        // Obtenemos el primer vertice que no este coloreado le ponemos 
        // color 1 y lo agregamos a la cola 'q'
        for (u32 i = 0; i < NumeroDeVertices(W); i++){
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
                    // el vertice que lo agregó estamos ante un grafo que no es bipartito
                    // entonces seteamos isBipartite en 0 para que corte los ciclos y
                    // devuelva 0 como resultado de la funcion
                    isBipartite = 0;
                    cleanColors(W);
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
    for (u32 i = 0; i < NumeroDeVertices(W) && isValid; i++) {
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
    for (u32 i = 0; i < NumeroDeVertices(W); i++)
        set_vertex_color(W->orderedVertexArray[i], 0);
}

// Funciones para extraer información de datos del grafo

u32 NumeroDeVertices(WinterIsHere W) {
    return W->NumeroDeVertices;
}

u32 NumeroDeLados(WinterIsHere W) {
    return W->NumeroDeLados;
}

u32 NumeroVerticesDeColor(WinterIsHere W, u32 i) {
    u32 count = 0;
    for (u32 j = 0; j < NumeroDeVertices(W); j++) {
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
    if (i < NumeroDeVertices(W))
        return get_vertex_tag(W->orderedVertexArray[i]);
    else
        return -1;
}


// Funciones de ordenación

/*
    Ordena en orden creciente segun el nombre del vertice 
*/
void OrdenNatural(WinterIsHere W) {
    qsort(W->orderedVertexArray, NumeroDeVertices(W),
          sizeof(W->orderedVertexArray[0]), cmpNaturalOrder);
}

/*
    Mayor a menor segun el grado
*/
void OrdenWelshPowell(WinterIsHere W) {
    qsort(W->orderedVertexArray, NumeroDeVertices(W),
          sizeof(W->orderedVertexArray[0]), cmpWelshPowell);
}

/*
    Funcion que devuelve un numero aleatorio
    (fijarse lo del 0)
*/
u32 xorshift32(u32 state[static 1])
{
    u32 x = state[0];
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
    u32 n = NumeroDeVertices(W);
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

void orderByRandomBlocksOfColor(WinterIsHere W, u32 x){
    u32 ncolores = NumeroDeColores(W);
    u32 *color_amount = calloc((ncolores + 1),sizeof(u32));
    u32 i, j, tmp;
    u32 n = NumeroDeVertices(W);

    // Recorremos el arreglo para contar los colores
    for (i = 0; i < n; i++)
        color_amount[get_vertex_color(W->orderedVertexArray[i])]++;

    u32 *random_color_order = malloc((ncolores + 1) * sizeof(u32));
    for (i = 0; i < ncolores + 1; i++)
        random_color_order[i] = i;
    
    
    // Implementación de algoritmo de shuffling Fisher-Yates
    for (i = ncolores ; i > 0; i--) {
        // Le pasamos la cantidad de elementos del arreglo 'n' y la variable
        // 'x' como seed a rand_uint()
        j = rand_uint(n, &x) % (i + 1);
        tmp = random_color_order[j];
        random_color_order[j] = random_color_order[i];
        random_color_order[i] = tmp;
    }

    printf("Color amounts:\n");
    for (i = 0; i < ncolores + 1; i++){
        printf("%"SCNu32, color_amount[i]);
        (i != ncolores) ? printf(",") : printf(".\n");
    }

    printf("Random color order:\n");
    for (i = 0; i < ncolores + 1; i++){
        printf("%"SCNu32, random_color_order[i]);
        (i != ncolores) ? printf(",") : printf(".\n");
    }

    u32 *color_position = malloc((ncolores + 1) * sizeof(u32));
    color_position[random_color_order[0]] = 0;
    for (i = 1; i < ncolores + 1; i++){
        color_position[random_color_order[i]] = color_position[random_color_order[i-1]] + color_amount[random_color_order[i-1]];
    }

    printf("Color positions:\n");
    for (i = 0; i < ncolores + 1; i++){
        printf("%"SCNu32, color_position[i]);
        (i != ncolores) ? printf(",") : printf(".\n");
    }

    vertex *orderedByColourVertexArray = malloc(NumeroDeVertices(W) * sizeof(struct VertexSt));
    u32 auxcol;
    for (u32 i = 0; i < NumeroDeVertices(W); i++){
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
    printf("\n");
//    dumpOrderedVertexArray(W, stdout);

    free(color_position);
    free(random_color_order);
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

    vertex *orderedByColourVertexArray = malloc(NumeroDeVertices(W) * sizeof(struct VertexSt));
    u32 auxcol;
    for (u32 i = 0; i < NumeroDeVertices(W); i++){
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
    for (u32 i = 0; i < NumeroDeVertices(W); i++)
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
    qsort(W->orderedVertexArray, NumeroDeVertices(W),
          sizeof(W->orderedVertexArray[0]), cmpMaxToMinByColor);
}

/*
    Ordena el arreglo segun la cantidad de colores en el orden
    segun el valor del parametro 'order' de la forma:
    order = 0 -> De menor a mayor
    order = 1 -> De mayor a menor
    
    El algoritmo toma en cuenta el color '0' pero si no existe ningun vertice
    con ese color no afecta al funcionamiento del mismo
    asi cada posición del arreglo se corresponde a un color
    desde el 0 hasta el maximo color
*/
void orderByAmountOfColor(WinterIsHere W, bool order){
    u32 ncolores = NumeroDeColores(W);
    colAmount *colAmountArray = malloc((ncolores + 1) * sizeof(struct ColAmountSt));
    u32 *color_amount = calloc((ncolores + 1),sizeof(u32));

    // Recorremos el arreglo para contar los colores
    for (u32 i = 0; i < NumeroDeVertices(W); i++)
        color_amount[get_vertex_color(W->orderedVertexArray[i])]++;

    for (u32 i = 0; i < (ncolores + 1); i++) {
        colAmountArray[i] = malloc(sizeof(struct ColAmountSt));
        colAmountArray[i]->color = i;
        colAmountArray[i]->amount = color_amount[i];
    }

    if(order == 0)  // Menor a mayor
        qsort(colAmountArray, ncolores + 1,
              sizeof(colAmountArray[0]), cmpMinToMaxColAmount);
    else if(order == 1)  // Mayor a menor
        qsort(colAmountArray, ncolores + 1,
              sizeof(colAmountArray[0]), cmpMaxToMinColAmount);

    for (u32 i = 0; i < ncolores + 1; i++){
        printf("Color:%"SCNu32", amount:%"SCNu32"\n",
               colAmountArray[i]->color,
               colAmountArray[i]->amount);
    }
    printf("\n");

    u32 *color_position = malloc((ncolores + 1) * sizeof(u32));
    color_position[colAmountArray[0]->color] = 0;
    for (u32 i = 1; i < ncolores + 1; i++){
        u32 prevCol = colAmountArray[i-1]->color;
        u32 currentCol = colAmountArray[i]->color;
        color_position[currentCol] = color_position[prevCol] +
                                     color_amount[prevCol];
    }

    vertex *orderedByColourVertexArray = malloc(NumeroDeVertices(W) * sizeof(struct VertexSt));
    u32 auxcol;
    for (u32 i = 0; i < NumeroDeVertices(W); i++){
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
    
    free(color_position);
    free(color_amount);
    for (u32 i = 0; i < (ncolores + 1); i++) {
        free(colAmountArray[i]);
    }
    free(colAmountArray);
}


void ReordenManteniendoBloqueColores(WinterIsHere W, u32 x) {
    if (x == 0){
        orderByColorMaxThenMinToMax(W);
    }
    else if (x == 1){
        orderByColorMaxToMin(W);
    }
    else if (x == 2){
        orderByAmountOfColor(W, 0);
        // Le pasamos 0 si queremos que ordene de menor a mayor
        //            1 si queremos que ordene de mayor a menor
    }
    else if (x == 3){
        orderByAmountOfColor(W, 1);
    }
    else if (x > 3){
        orderByRandomBlocksOfColor(W, x);
    }
}


// Funciones de los vertices

u32 NombreDelVertice(WinterIsHere W, u32 x) {
    u32 vname = 0;
    for (u32 i = 0; i < NumeroDeVertices(W); i++){
        if (get_vertex_tag(W->orderedVertexArray[i]) == x){
            vname = get_vertex_name(W->orderedVertexArray[i]);
            break;
        }
    }
    return vname;
}

u32 ColorDelVertice(WinterIsHere W, u32 x) {
    u32 vcolor = 0;
    for (u32 i = 0; i < NumeroDeVertices(W); i++){
        if (get_vertex_tag(W->orderedVertexArray[i]) == x){
            vcolor = get_vertex_color(W->orderedVertexArray[i]);
            break;
        }
    }
    return vcolor;
}

u32 GradoDelVertice(WinterIsHere W, u32 x) {
    u32 vgrade = 0;
    for (u32 i = 0; i < NumeroDeVertices(W); i++){
        if (get_vertex_tag(W->orderedVertexArray[i]) == x){
            vgrade = get_vertex_grade(W->orderedVertexArray[i]);
            break;
        }
    }
    return vgrade;
}

u32 IesimoVecino(WinterIsHere W, u32 x, u32 i) {
    vertex v = NULL;
    u32 tag = 0;
    for (u32 i = 0; i < NumeroDeVertices(W); i++){
        if (get_vertex_tag(W->orderedVertexArray[i]) == x){
            v = W->orderedVertexArray[i];
            break;
        }
    }
    if (v != NULL){
        neighb_t auxList = get_vertex_neigh_list(v);
        u32 j = 0;

        while(auxList != NULL && j != i) {
            auxList = auxList->next;
            j++;
        }
        if(auxList != NULL){
            tag = get_vertex_tag(auxList->vertex_pt);
        }
    }
    return tag;
}


WinterIsHere createGraph(u32 vCount, u32 eCount) {
    WinterIsHere W = calloc(1, sizeof(struct WinterSt));
    W->NumeroDeLados = eCount;
    W->NumeroDeVertices = vCount;
    W->vertex_hashmap = create_hash_map(NumeroDeVertices(W));
    W->orderedVertexArray = malloc(NumeroDeVertices(W) * sizeof(struct VertexSt));

    return W;
}

void dumpOrderedVertexArray(WinterIsHere W, FILE *f) {
    for (u32 i = 0; i < NumeroDeVertices(W); i++) {
        fprintf(f, "Pos:%u, ", i);
        print_vertex_data(W->orderedVertexArray[i]);
    }
}

// Main y funciones auxiliares

u32 xorshift128(u32 state[static 4])
{
    u32 t = state[3];
    t ^= t << 11;
    t ^= t >> 8;
    state[3] = state[2]; state[2] = state[1]; state[1] = state[0];
    t ^= state[0];
    t ^= state[0] >> 19;    
    state[0] = t;
    return t;
}

int main() {
    printf("Loading...\n");
    WinterIsHere W = WinterIsComing();
    printf("Loading done.\n");

    /*for (u32 i = 0; i < NumeroDeVertices(W); i++)
        printf("%"SCNu32",", GradoDelVertice(W,IesimoVerticeEnElOrden(W,i)));
    printf("\n\n");*/
    OrdenWelshPowell(W);

/*    for (u32 i = 0; i < NumeroDeVertices(W); i++)
        printf("%"SCNu32",", GradoDelVertice(W,IesimoVerticeEnElOrden(W,i)));
    printf("\n\n");*/
    dumpOrderedVertexArray(W, stdout);
    u32 tag = 1;
    u32 neig = 49;
    printf("\n\nTag: %"SCNu32", Vecino %"SCNu32" es %"SCNu32"\n", tag, neig, IesimoVecino(W, tag, neig));

//    u32 colores = Greedy(W);
/*    ReordenManteniendoBloqueColores(W, 2);
    dumpOrderedVertexArray(W, stdout);
    printf("\n");
    ReordenManteniendoBloqueColores(W, 3);
    dumpOrderedVertexArray(W, stdout);
    printf("\n");
    ReordenManteniendoBloqueColores(W, 1);
    dumpOrderedVertexArray(W, stdout);
    printf("Coloreo: %"SCNu32"\n", NumeroDeColores(W));
*/
    Primavera(W);
    return 0;
}