#include "JonSnow.h"
// Indica la cantidad maxima de caracteres que se pueden leer de una linea
#define MAX_LINE 4096

struct WinterSt {
    u32 NumeroDeVertices;
    u32 NumeroDeLados;
    u32 NumeroDeColoresUsados;
    // En orderedVertexArray se guardarán los punteros a los vertices en el
    // orden que elijamos para correr Greedy
    vertex *orderedVertexArray;
    hashtable vertexHashtable;
};


WinterIsHere WinterIsComing() {
    char input[MAX_LINE];
    u32 vCount, eCount;
    WinterIsHere W = NULL;

    // La variable loadFailed servirá para indicar si hubo un error en la 
    // creación del grafo o el formato del archivo es invalido
    bool loadFailed = false;
    // parseResult servirá para chequear los resultados de los sscanf()
    int parseResult; 

    // La variable 'extraChars' servirá para chequear si hay mas caracteres
    // luego de 'p edge %d %d' y en caso de que los haya tomar el formato como
    // invalido
    char *extraChars = malloc(sizeof(char) * MAX_LINE);
    if (extraChars == NULL) {
        loadFailed = true;
    }

    // Descartamos las primeras lineas que comiencen con 'c'
    do {
        if(fgets(input, MAX_LINE, stdin) == NULL) {
            // Chequeamos que no haya lineas vacias
            loadFailed = true;
        }
    }
    while(input[0] == 'c');

    // Parseamos la linea 'p edge n m' para obtener la cantidad de vertices 'n'
    // y aristas 'm'
    parseResult = sscanf(input, "p edge %"SCNu32 " %"SCNu32" %s",
                         &vCount, &eCount, extraChars);

    // Chequeo si hay mas caracteres luego de 'n' y 'm', estos seran guardados
    // en extraChars, y parseResult resultaria en "3" indicando un formato
    // invalido
    if (parseResult != 2) {
        loadFailed = true;
    }

    printf("cantidad de parametros: %d\n", parseResult);
    printf("Resultado: %s\n", loadFailed ? "true" : "false");

    if (!loadFailed) {
        u32 v1name, v2name;
        vertex v1 = NULL;
        vertex v2 = NULL;

        u32 i = 0;
        u32 j = 0;

        W = createGraph(vCount, eCount);
        if (W == NULL) {
            loadFailed = true;
        }

        while(!loadFailed && j < W->NumeroDeLados) {
            if(fgets(input, MAX_LINE, stdin) == NULL) {
                loadFailed = true;
                break;
            }
            parseResult = sscanf(input, "e %" SCNu32 " %" SCNu32 " %s",
                                 &v1name, &v2name, extraChars);
            if(parseResult != 2) {
                loadFailed = true;
                break;
            }

            v1 = search_vertex(v1name, W->vertexHashtable);
            v2 = search_vertex(v2name, W->vertexHashtable);

            // add_entry_to_hasthtable es de la forma:
            // add_entry_to_hashtable(u32 tag, u32 name, u32 color, u32 grade,
            //                        hashtable h_map);
            // en todos los casos 'i' da el nombre del tag del vertice,
            // v1name y v2name se extraen del archivo segun 'e v1name v2name' y
            // el color lo seteamos en 0 y el grado seteado varía segun el caso
            if(v1 == NULL && v2 == NULL) {
                // En este caso ambos vertices son nuevos, y son vecinos por lo
                // tanto seteamos el grado en 1 a ambos, ya que poseen 1 solo
                // vecino por el momento
                // El vertice se crea y agrega a la hashtable mediante
                // add_entry_to_hashtable, esta función ademas de crear el
                // vertice segun los valores dados devuelve un puntero al mismo
                // guardamos éste en el arreglo de vertices ordenados para así
                // poder ordenarlos de distintas formas segun el orden que
                // queramos que corra Greedy
                v1 = add_entry_to_hashtable(i, v1name, 0, 1, W->vertexHashtable);
                W->orderedVertexArray[i] = v1;
                i++;
                v2 = add_entry_to_hashtable(i, v2name, 0, 1, W->vertexHashtable);
                W->orderedVertexArray[i] = v2;
                i++;
            } else if (v1 == NULL && v2 != NULL) {
                // En este caso el vertice v1 es nuevo, por lo tanto le ponemos
                // grado 1
                v1 = add_entry_to_hashtable(i, v1name, 0, 1, W->vertexHashtable);
                W->orderedVertexArray[i] = v1;
                i++;
                // El vertice v2 ya existía, por lo tanto solamente
                // incrementamos su grado en 1.
                set_vertex_grade(v2, get_vertex_grade(v2) + 1);
            } else if (v1 != NULL && v2 == NULL) {
                v2 = add_entry_to_hashtable(i, v2name, 0, 1, W->vertexHashtable);
                W->orderedVertexArray[i] = v2;
                i++;
                set_vertex_grade(v1, get_vertex_grade(v1) + 1);
            } else {  // v1 != NULL && v2 != NULL
                // Ambos vertices ya habian sido agregados previamente,
                // por lo tanto solamente incrementamos sus grados en 1
                set_vertex_grade(v1, get_vertex_grade(v1) + 1);
                set_vertex_grade(v2, get_vertex_grade(v2) + 1);
            }
            // Agregamos los vertices a la lista de vecinos del otro
            make_vertex_neighs(v1, v2);
            // Incrementamos la cantidad de aristas agregadas
            j++;

            // Ademas de su uso para indicar el siguiente lugar disponible en
            // el arreglo de vertices ordenados y el tag del vertice a agregar
            // se utilizará 'i' para chequear que la cantidad de vertices que
            // dice 'n' en 'p edge n m' concuerde con la cantidad de vertices
            // dentro del archivo, ponemos la comprobación dentro del ciclo
            // para que corte la carga si es que se pasa, asi ahorramos tiempo
            // en un archivo que sabemos que tiene un formato erroneo,
            // igualmente se chequea que (i == n) fuera del ciclo una vez que
            // se completó la carga por si la cantidad 'n' fuese mayor a la
            // cantidad real de vertices dentro del archivo
            if (i > vCount) {
                loadFailed = true;
            }
        }
    
        if (i != vCount) {
            loadFailed = true;
        }
        printf("cantidad de parametros: %d\n", parseResult);
        printf("Resultado: %s\ni:%"SCNu32 " n:%"SCNu32"\n", (loadFailed ? "true" : "false"), i, vCount);
    }

    if(!loadFailed) {
        // Si todo resultó bien se corre Greedy
        Greedy(W);
    } else {
        // Si la carga falló se libera la memoria y se setea W en NULL
        Primavera(W);
        W = NULL;
    }
    free(extraChars);
    return W;
}

/*
    Destruye W y libera la memoria alocada. Retorna 1 si todo anduvo bien y
    0 si no.
*/
int Primavera(WinterIsHere W) {
    int result = 1;
    if (W != NULL) {
        W->vertexHashtable = destroy_hashtable(W->vertexHashtable);
        free(W->orderedVertexArray);
        W->orderedVertexArray = NULL;
        result = (W->vertexHashtable == NULL);
        free(W);
        W = NULL;
    }
    return result;
}

/* Funciones de Coloreo */

u32 Greedy(WinterIsHere W) {
    cleanColors(W);
    u32 colorMaximo = 1;
    u32 colorAux = 0;
    u32 *arrayColores = NULL;
    vertex vertexAux; 
    for (u32 i = 0 ; i < W->NumeroDeVertices; i++) {
        //  Iteramos sobre el arreglo de vertices en el orden en el que estan
        vertexAux = W->orderedVertexArray[i];  
        if(get_vertex_grade(vertexAux) >= colorMaximo) {
            arrayColores = calloc(colorMaximo, sizeof(u32));
        } else {
            arrayColores = calloc(get_vertex_grade(vertexAux), sizeof(u32));
        }
        neighb_t neighbHelp = get_vertex_neigh_list(vertexAux);
        while(neighbHelp != NULL) {
            if ((get_vertex_color(neighbHelp->vertex_pt) <= get_vertex_grade(vertexAux))
                && (get_vertex_color(neighbHelp->vertex_pt) != 0)) {
                    arrayColores[get_vertex_color(neighbHelp->vertex_pt) - 1] = 1;
            }
            neighbHelp = neighbHelp->next;
        }
        colorAux = 0;
        while ((colorAux < get_vertex_grade(vertexAux)) && (colorAux < colorMaximo) && (arrayColores[colorAux] == 1)) {
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
    while (j < W->NumeroDeVertices && isBipartite) {
        // Obtenemos el primer vertice que no este coloreado le ponemos 
        // color 1 y lo agregamos a la cola 'q'
        for (u32 i = 0; i < W->NumeroDeVertices; i++) {
            if(get_vertex_color(W->orderedVertexArray[i]) == 0) {
                v = W->orderedVertexArray[i];
                set_vertex_color(v, 1);
                j++;
                enqueue(q,v);
                break;
            }
        }
        while(!isEmpty(q) && isBipartite) {
            v = dequeue(q);
            // Obtenemos el primer vertice de la cola
            vertex_neigh_list = get_vertex_neigh_list(v);
            // Obtenemos los vecinos del vertice y los recorremos dentro del while
            while (vertex_neigh_list != NULL) {
                // Obtenemos el vertice desde dentro de la estructura del nodo NeighbSt
                w = vertex_neigh_list->vertex_pt;
                // Si el vertice no esta coloreado continuamos
                if (get_vertex_color(w) == 0) {
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
int isValidColoring(WinterIsHere W) {
    vertex v, w;
    neighb_t vertex_neigh_list;
    int isValid = 1;
    for (u32 i = 0; i < W->NumeroDeVertices && isValid; i++) {
        v = W->orderedVertexArray[i];
        vertex_neigh_list = get_vertex_neigh_list(v);
        while (vertex_neigh_list != NULL) {
            w = vertex_neigh_list->vertex_pt;
            if (get_vertex_color(w) == get_vertex_color(v)) {
                isValid = 0;
                break;
            }
            vertex_neigh_list = vertex_neigh_list->next;
        }
    }
    return isValid;
}


void cleanColors(WinterIsHere W) {
    for (u32 i = 0; i < W->NumeroDeVertices; i++) {
        set_vertex_color(W->orderedVertexArray[i], 0);
    }
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
    for (u32 j = 0; j < W->NumeroDeVertices; j++) {
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
    if (i < W->NumeroDeVertices)
        return get_vertex_tag(W->orderedVertexArray[i]);
    else
        return -1;
}


// Funciones de ordenación

/*
    Ordena en orden creciente segun el nombre de los vertices
*/
void OrdenNatural(WinterIsHere W) {
    qsort(W->orderedVertexArray, W->NumeroDeVertices,
          sizeof(W->orderedVertexArray[0]), cmpNaturalOrder);
}

/*
    Ordena en orden decreciente segun el grado de los vertices
*/
void OrdenWelshPowell(WinterIsHere W) {
    qsort(W->orderedVertexArray, W->NumeroDeVertices,
          sizeof(W->orderedVertexArray[0]), cmpWelshPowell);
}

/*
    Algoritmo Xorshift32 que devuelve un numero aleatorio 
    de 32 bits, el state inicial (seed) tiene que ser distinto de 0
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

/*
    Calcula un numero aleatorio de 0 a 'n' tomando como seed 'x'
    con distribucion mas uniforme que solo utilizar 'rand() % n'
*/
static u32 rand_u32(u32 n, u32 *x) {
    u32 rand_u32_max = 0xFFFFFFFF;
    u32 limit = rand_u32_max - rand_u32_max % n;
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
    u32 *random_order_array = malloc(n * sizeof(u32));

    // Aqui se guardaran los nombres de los vertices segun el orden aleatorio
    // que queremos que tenga el arreglo de los vertices ordenados

    // Obtenemos los nombres de los vertices tal cual están en el arreglo
    // de vertices ordenados y los guardamos en el arreglo nuevo
    for (i = 0; i < n; i++) {
        random_order_array[i] = get_vertex_name(W->orderedVertexArray[i]);
    }
    // Ordeno de menor a mayor para que el shuffling sea deterministico segun
    // el parametro 'x'
    qsort(random_order_array, n, sizeof(random_order_array[0]), cmpMinToMax);
    
    // Implementación del algoritmo de shuffling de Fisher-Yates
    for (i = n - 1; i > 0; i--) {
        // Le pasamos la cantidad de elementos del arreglo 'n' y la variable
        // 'x' como seed a rand_u32()
        j = rand_u32(n, &x) % (i + 1);
        tmp = random_order_array[j];
        random_order_array[j] = random_order_array[i];
        random_order_array[i] = tmp;
    }

    // Guardamos los vertices segun el orden obtenido en el shuffle realizado
    // en random_order_array, al estar guardados por sus nombres tenemos que
    // buscarlos
    for (i = 0; i < n ; i++) {
        W->orderedVertexArray[i] = search_vertex(random_order_array[i],
                                                   W->vertexHashtable);
    }
    free(random_order_array);
}

void orderByRandomBlocksOfColor(WinterIsHere W, u32 x) {
    u32 ncolores = NumeroDeColores(W);
    u32 *color_amount = calloc((ncolores + 1),sizeof(u32));
    u32 i, j, tmp;
    u32 n = W->NumeroDeVertices;

    // Recorremos el arreglo de vertices para contar la cantidad de vertices
    // que tiene cada color y lo guardamos en el arreglo color_amount
    for (i = 0; i < n; i++) {
        color_amount[get_vertex_color(W->orderedVertexArray[i])]++;
    }
    // Creamos otro arreglo con todos los colores utilizados para luego hacer
    // un shuffling y obtener el orden aleatorio
    u32 *random_color_order = malloc((ncolores + 1) * sizeof(u32));
    for (i = 0; i < ncolores + 1; i++) {
        random_color_order[i] = i;
    }
    // Hacemos el shuffling de los colores utilizando una implementación
    // del algoritmo de Fisher-Yates
    for (i = ncolores ; i > 0; i--) {
        // Le pasamos la cantidad de elementos del arreglo 'n' y la variable
        // 'x' como seed a rand_u32()
        j = rand_u32(n, &x) % (i + 1);
        tmp = random_color_order[j];
        random_color_order[j] = random_color_order[i];
        random_color_order[i] = tmp;
    }

    printf("Color amounts:\n");
    for (i = 0; i < ncolores + 1; i++) {
        printf("%"SCNu32, color_amount[i]);
        (i != ncolores) ? printf(",") : printf(".\n");
    }

    printf("Random color order:\n");
    for (i = 0; i < ncolores + 1; i++) {
        printf("%"SCNu32, random_color_order[i]);
        (i != ncolores) ? printf(",") : printf(".\n");
    }

    u32 *color_position = malloc((ncolores + 1) * sizeof(u32));
    color_position[random_color_order[0]] = 0;
    for (i = 1; i < ncolores + 1; i++) {
        color_position[random_color_order[i]] = color_position[random_color_order[i-1]] + color_amount[random_color_order[i-1]];
    }

    printf("Color positions:\n");
    for (i = 0; i < ncolores + 1; i++) {
        printf("%"SCNu32, color_position[i]);
        (i != ncolores) ? printf(",") : printf(".\n");
    }

    vertex *orderedByColourVertexArray = malloc(W->NumeroDeVertices * sizeof(struct VertexSt));
    u32 auxcol;
    for (u32 i = 0; i < W->NumeroDeVertices; i++) {
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

void orderByColorMaxThenMinToMax(WinterIsHere W) {
    // El algoritmo deja sin utilizar la posicion '0' para mayor legibilidad
    // del codigo, asi cada posición del arreglo se corresponde a un color
    // desde el 1 hasta el maximo color
    u32 ncolores = NumeroDeColores(W);
    u32 *color_amount = calloc((ncolores + 1),sizeof(u32));

    // Recorremos el arreglo para contar los colores
    for (u32 i = 0; i < W->NumeroDeVertices; i++) {
        color_amount[get_vertex_color(W->orderedVertexArray[i])]++;
    }

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
    for (u32 i = 2; i < (ncolores); i++) {
        color_position[i] = color_position[i - 1] + color_amount[i - 1];
        // Ponemos las posiciones iniciales de los vertices que siguen a los del color 1
        // segun la posicion del color anterior mas la cantidad de vertices esperados
        // del color anterior
    }

    vertex *orderedByColourVertexArray = malloc(W->NumeroDeVertices * sizeof(struct VertexSt));
    u32 auxcol;
    for (u32 i = 0; i < W->NumeroDeVertices; i++) {
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

void orderByColorMaxToMin(WinterIsHere W) {
    qsort(W->orderedVertexArray, W->NumeroDeVertices,
          sizeof(W->orderedVertexArray[0]), cmpMaxToMinByColor);
}

/*
    Ordena el arreglo segun la cantidad de colores en el orden
    segun el valor del parametro 'order' de la forma:
    order = 0 -> De menor a mayor
    order = 1 -> De mayor a menor
    
    El algoritmo toma en cuenta el color '0' pero si no existe ningun vertice
    con ese color no afecta al funcionamiento del mismo
*/
void orderByAmountOfColor(WinterIsHere W, bool order) {
    u32 ncolores = NumeroDeColores(W);
    colAmount *colAmountArray = malloc((ncolores + 1) * sizeof(struct ColAmountSt));
    u32 *color_amount = calloc((ncolores + 1), sizeof(u32));

    // Recorremos el arreglo para contar los colores
    for (u32 i = 0; i < W->NumeroDeVertices; i++)
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

    for (u32 i = 0; i < ncolores + 1; i++) {
        printf("Color:%"SCNu32", amount:%"SCNu32"\n",
               colAmountArray[i]->color,
               colAmountArray[i]->amount);
    }
    printf("\n");

    u32 *color_position = malloc((ncolores + 1) * sizeof(u32));
    color_position[colAmountArray[0]->color] = 0;
    for (u32 i = 1; i < ncolores + 1; i++) {
        u32 prevCol = colAmountArray[i-1]->color;
        u32 currentCol = colAmountArray[i]->color;
        color_position[currentCol] = color_position[prevCol] +
                                     color_amount[prevCol];
    }

    vertex *orderedByColourVertexArray = malloc(W->NumeroDeVertices * sizeof(struct VertexSt));
    u32 auxcol;
    for (u32 i = 0; i < W->NumeroDeVertices; i++) {
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
    if (x == 0) {
        orderByColorMaxThenMinToMax(W);
    }
    else if (x == 1) {
        orderByColorMaxToMin(W);
    }
    else if (x == 2) {
        orderByAmountOfColor(W, 0);
        // Le pasamos 0 si queremos que ordene de menor a mayor
        //            1 si queremos que ordene de mayor a menor
    }
    else if (x == 3) {
        orderByAmountOfColor(W, 1);
    }
    else if (x > 3) {
        orderByRandomBlocksOfColor(W, x);
    }
}


// Funciones de los vertices

u32 NombreDelVertice(WinterIsHere W, u32 x) {
    u32 vname = 0;
    for (u32 i = 0; i < W->NumeroDeVertices; i++) {
        if (get_vertex_tag(W->orderedVertexArray[i]) == x) {
            vname = get_vertex_name(W->orderedVertexArray[i]);
            break;
        }
    }
    return vname;
}

u32 ColorDelVertice(WinterIsHere W, u32 x) {
    u32 vcolor = 0;
    for (u32 i = 0; i < W->NumeroDeVertices; i++) {
        if (get_vertex_tag(W->orderedVertexArray[i]) == x) {
            vcolor = get_vertex_color(W->orderedVertexArray[i]);
            break;
        }
    }
    return vcolor;
}

u32 GradoDelVertice(WinterIsHere W, u32 x) {
    u32 vgrade = 0;
    for (u32 i = 0; i < W->NumeroDeVertices; i++) {
        if (get_vertex_tag(W->orderedVertexArray[i]) == x) {
            vgrade = get_vertex_grade(W->orderedVertexArray[i]);
            break;
        }
    }
    return vgrade;
}

u32 IesimoVecino(WinterIsHere W, u32 x, u32 i) {
    vertex v = NULL;
    u32 tag = 0;
    for (u32 i = 0; i < W->NumeroDeVertices; i++) {
        if (get_vertex_tag(W->orderedVertexArray[i]) == x) {
            v = W->orderedVertexArray[i];
            break;
        }
    }
    if (v != NULL) {
        neighb_t auxList = get_vertex_neigh_list(v);
        u32 j = 0;

        while(auxList != NULL && j != i) {
            auxList = auxList->next;
            j++;
        }
        if(auxList != NULL) {
            tag = get_vertex_tag(auxList->vertex_pt);
        }
    }
    return tag;
}


WinterIsHere createGraph(u32 vCount, u32 eCount) {
    WinterIsHere W = calloc(1, sizeof(struct WinterSt));
    if (W != NULL) {
        W->NumeroDeLados = eCount;
        W->NumeroDeVertices = vCount;
        W->vertexHashtable = create_hashtable(W->NumeroDeVertices);
        W->orderedVertexArray = malloc(W->NumeroDeVertices *
                                       sizeof(struct VertexSt));
        if(W->vertexHashtable == NULL || W->orderedVertexArray == NULL) {
            Primavera(W);
            W = NULL;
        }
    }
    return W;
}

void dumpOrderedVertexArray(WinterIsHere W, FILE *f) {
    for (u32 i = 0; i < W->NumeroDeVertices; i++) {
        fprintf(f, "Pos:%u, ", i);
        print_vertex_data(W->orderedVertexArray[i]);
    }
}

// Main y funciones auxiliares

u32 xorshift128(u32 state[static 4]) {
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

    /*for (u32 i = 0; i < W->NumeroDeVertices; i++)
        printf("%"SCNu32",", GradoDelVertice(W,IesimoVerticeEnElOrden(W,i)));
    printf("\n\n");*/
//    dumpOrderedVertexArray(W, stdout);
    if (W != NULL) {
//        OrdenWelshPowell(W);
        printf("\n\n");
    //    cleanColors(W);
//        Greedy(W);
        print_nodes_per_bucket(W->vertexHashtable);
//        printf("El coloreo es de: %"SCNu32"\n", NumeroDeColores(W));
//        dump_hashtable(W->vertexHashtable, stdout);
//        dump_array(W->vertexHashtable);
        Primavera(W);
    }
    return 0;
}
//    dumpOrderedVertexArray(W, stdout);

/*    for (u32 i = 0; i < W->NumeroDeVertices; i++)
        printf("%"SCNu32",", GradoDelVertice(W,IesimoVerticeEnElOrden(W,i)));
    printf("\n\n");*/
//    dumpOrderedVertexArray(W, stdout);
//    u32 tag = 15;
//    u32 neig = 0;
//    printf("NOMBRE: %"SCNu32"\n", NombreDelVertice(W,tag));
//    printf("\n\nTag: %"SCNu32", Vecino %"SCNu32" es %"SCNu32"\n", tag, neig, IesimoVecino(W, tag, neig));
//    print_vertex_data(search_vertex(NombreDelVertice(W, tag), W->vertexHashtable));
//    print_vertex_data(search_vertex(NombreDelVertice(W,IesimoVecino(W, tag, neig)), W->vertexHashtable));
//    print_vertex_data(search_vertex(NombreDelVertice(W,1), W->vertexHashtable));

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
