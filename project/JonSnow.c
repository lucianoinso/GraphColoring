#include "JonSnow.h"
// Indica el largo maximo de caracteres de cada una de las lineas de entrada.
#define MAX_LINE 4096

struct WinterSt {
    u32 NumeroDeVertices;
    u32 NumeroDeLados;
    u32 NumeroDeColoresUsados;
    // En vertexOrdByTagArray se guardan los punteros a los vertices en el orden
    // que elijamos para correr Greedy.
    vertex *vertexOrdByTagArray;
    hashtable vertexHashtable;
    u32 *vertexOrderArray;
};

// Funciones de Construcción/Destrucción del grafo

WinterIsHere WinterIsComing() {
    char input[MAX_LINE];
    u32 vCount, eCount;
    WinterIsHere W = NULL;

    // La variable loadFailed se usa para indicar si hubo un error en la
    // creación del grafo o si el formato del archivo es invalido.
    bool loadFailed = false;
    // parseResult se usa para chequear los resultados de los sscanf().
    int parseResult;

    // 'extraChars' se usa para chequear si el formato de entrada es valido.
    // Si hay algun texto luego de 'p edge n m' o de 'e v1 v2' se guarda en
    // esta variable, el valor de retorno de sscanf sera distinto de 2 entonces
    // sabremos que el formato es invalido.
    char *extraChars = malloc(sizeof(char) * MAX_LINE);
    if (extraChars == NULL) {
        loadFailed = true;
    }

    // Se descargan las primeras lineas que comienzan con 'c' si las hubiera.
    do {
        if(fgets(input, MAX_LINE, stdin) == NULL) {
            // Chequeamos que no haya lineas vacias.
            loadFailed = true;
        }
    }
    while(input[0] == 'c');

    // Se parsea la linea 'p edge n m' para obtener la cantidad de vertices 'n'
    // y aristas 'm'.
    parseResult = sscanf(input, "p edge %"SCNu32 " %"SCNu32" %s",
                         &vCount, &eCount, extraChars);

    // Chequeo si hay mas caracteres luego de 'n' y 'm', si fuese el caso
    // parseResult resultaria en "3" indicando un formato invalido.
    if (parseResult != 2) {
        loadFailed = true;
    }

    if (!loadFailed) {
        u32 v1name, v2name, i, j;
        vertex v1 = NULL;
        vertex v2 = NULL;

        // La variable 'i' se usa para llevar la cuenta de cuantos vertices se
        // van ingresando, ademas sirve para setear los tags de los mismos y 
        // para saber en que posición del arreglo de vertices ordenados debe ir
        // el vertice que se está ingresando.

        // La variable 'j' cuenta el numero de lados que se van ingresando,
        // cuando j es igual al numero de lados la carga termina.
        i = j = 0; 

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
            // Si el formato no es de la forma "e v1 v2", parseResult tendrá
            // un valor distinto de 2, si esto sucede seteamos loadFailed en
            // falso e interrumpimos la carga cortando el ciclo.
            if(parseResult != 2) {
                loadFailed = true;
                break;
            }

            // search_vertex toma el nombre de un vertice y un hashtable,
            // si el vertice con ese nombre no existe devolverá NULL, en caso
            // de que si exista devuelve un puntero al mismo.
            // En este paso se buscan ambos vertices de la entrada para ver si
            // ya están ingresados o si son nuevos, los cuatro casos posibles
            // operan de distinta manera segun el codigo que sigue.
            v1 = search_vertex(v1name, W->vertexHashtable);
            v2 = search_vertex(v2name, W->vertexHashtable);

            // add_entry_to_hasthtable es de la forma:
            // add_entry_to_hashtable(u32 tag, u32 name, u32 color, u32 grade,
            //                        hashtable htable);
            // En todos los casos 'i' da el nombre del tag del vertice,
            // v1name y v2name se extraen del archivo segun 'e v1name v2name',
            // el color se setea en 0 y el grado seteado varía segun el caso.
            if(v1 == NULL && v2 == NULL) {
                // En este caso ambos vertices son nuevos, y son vecinos por lo
                // tanto seteamos el grado en 1 a ambos, ya que poseen 1 solo
                // vecino por el momento.
                // Los vertices se crean y agregan a la hashtable mediante
                // add_entry_to_hashtable, esta función ademas de crear el
                // vertice segun los valores dados devuelve un puntero al mismo,
                // guardamos éste en el arreglo de vertices ordenados para así
                // poder ordenarlos de distintas formas segun el orden que
                // queramos que corra Greedy.
                v1 = add_entry_to_hashtable(i, v1name, 0, 1, W->vertexHashtable);
                W->vertexOrdByTagArray[i] = v1;
                i++;
                v2 = add_entry_to_hashtable(i, v2name, 0, 1, W->vertexHashtable);
                W->vertexOrdByTagArray[i] = v2;
                i++;
            } else if (v1 == NULL && v2 != NULL) {
                // En este caso el vertice v1 es nuevo, por lo tanto le ponemos
                // grado 1.
                v1 = add_entry_to_hashtable(i, v1name, 0, 1, W->vertexHashtable);
                W->vertexOrdByTagArray[i] = v1;
                i++;
                // El vertice v2 ya existe, por lo tanto incrementamos su grado
                // en 1.
                set_vertex_grade(v2, get_vertex_grade(v2) + 1);
            } else if (v1 != NULL && v2 == NULL) {
                v2 = add_entry_to_hashtable(i, v2name, 0, 1, W->vertexHashtable);
                W->vertexOrdByTagArray[i] = v2;
                i++;
                set_vertex_grade(v1, get_vertex_grade(v1) + 1);
            } else {  // v1 != NULL && v2 != NULL
                // Ambos vertices ya habian sido agregados previamente, por lo
                // tanto solamente incrementamos sus grados en 1
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

WinterIsHere createGraph(u32 vCount, u32 eCount) {
    WinterIsHere W = calloc(1, sizeof(struct WinterSt));
    if (W != NULL) {
        W->NumeroDeLados = eCount;
        W->NumeroDeVertices = vCount;
        W->NumeroDeColoresUsados = 0;
        W->vertexOrderArray = malloc(W->NumeroDeVertices * sizeof(u32));

        for (u32 i = 0; i < W->NumeroDeVertices; i++){
            W->vertexOrderArray[i] = i;
        }

        W->vertexHashtable = create_hashtable(W->NumeroDeVertices);
        W->vertexOrdByTagArray = malloc(W->NumeroDeVertices * sizeof(vertex));
        if(W->vertexHashtable == NULL || W->vertexOrdByTagArray == NULL) {
            Primavera(W);
            W = NULL;
        }
    }
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
        result = (W->vertexHashtable == NULL);
        free(W->vertexOrdByTagArray);
        W->vertexOrdByTagArray = NULL;
        free(W->vertexOrderArray);
        W->vertexOrderArray = NULL;
        free(W);
        W = NULL;
    }
    return result;
}

// Funciones de Coloreo

u32 Greedy(WinterIsHere W) {
    cleanColors(W);
    u32 colorMaximo = 1;
    u32 colorAux = 0;
    u32 *arrayColores = NULL;
    vertex vertexAux; 
    for (u32 i = 0 ; i < W->NumeroDeVertices; i++) {
        //  Iteramos sobre el arreglo de vertices en el orden en el que estan
        // OLD vertexAux = W->vertexOrdByTagArray[i]; END OLD

        vertexAux = W->vertexOrdByTagArray[W->vertexOrderArray[i]];
        
        if(get_vertex_grade(vertexAux) >= colorMaximo) {
            arrayColores = calloc(colorMaximo, sizeof(u32));
        } else {
            arrayColores = calloc(get_vertex_grade(vertexAux), sizeof(u32));
        }
        neighb_t neighbHelp = get_vertex_neigh_list(vertexAux);
        while(neighbHelp != NULL) {
            

            /*
            OLD
            if ((get_vertex_color(neighbHelp->vertex_pt) <= get_vertex_grade(vertexAux))
                && (get_vertex_color(neighbHelp->vertex_pt) != 0)) {
                    arrayColores[get_vertex_color(neighbHelp->vertex_pt) - 1] = 1;
            }
            END OLD
            */

            if ((get_vertex_color(W->vertexOrdByTagArray[neighbHelp->vNeighTag]) <= get_vertex_grade(vertexAux))
                && (get_vertex_color(W->vertexOrdByTagArray[neighbHelp->vNeighTag]) != 0)) {
                    arrayColores[get_vertex_color(W->vertexOrdByTagArray[neighbHelp->vNeighTag]) - 1] = 1;
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
        // Obtenemos el primer vertice que no este coloreado, le ponemos
        // color 1 y lo agregamos a la cola 'q'

        for (u32 i = 0; i < W->NumeroDeVertices; i++) {
            v = W->vertexOrdByTagArray[i];
            if(get_vertex_color(v) == 0) {
                set_vertex_color(v, 1);
                j++;
                enqueue(q,v);
                break;
            }
        }
        while(!isEmpty(q) && isBipartite) {
            // Obtenemos el primer vertice de la cola.
            v = dequeue(q);
            // Obtenemos los vecinos del vertice y los recorremos dentro del
            // ciclo while.
            vertex_neigh_list = get_vertex_neigh_list(v);
            while (vertex_neigh_list != NULL) {
                // Obtenemos el vertice desde dentro de la estructura del nodo
                // NeighbSt

                w = W->vertexOrdByTagArray[vertex_neigh_list->vNeighTag];
                // Si el vertice no esta coloreado continuamos
                if (get_vertex_color(w) == 0) {
                    // Encolamos el vertice en la cola 'q' para luego colorear
                    // sus vecinos en la proxima iteracion
                    enqueue(q, w);
                    // Sumamos 1 a la cantidad de vertices coloreados
                    j++;
                    // Seteamos el color del nuevo vertice segun el color del
                    // vertice que lo agrego, si el color del vertice que lo
                    // agrego es 1 entonces "3 - 1 = 2"
                    // si es 2 entonces "3 - 2 = 1"
                    set_vertex_color(w, 3 - get_vertex_color(v));
                } else if (get_vertex_color(w) == get_vertex_color(v)) {
                    // Si encontramos en la cola un vertice con el mismo color
                    // que el vertice que lo agregó estamos ante un grafo que 
                    // no es bipartito, entonces seteamos isBipartite en 0 para
                    // que corte los ciclos y devuelva 0 como resultado de la
                    // funcion y limpiamos los colores del grafo.
                    isBipartite = 0;
                    cleanColors(W);
                    break;
                }
                // Procedemos al siguiente vertice de la lista de vecinos.
                vertex_neigh_list = vertex_neigh_list->next;
            }
        }
    }
    // Liberamos la memoria de la cola 'q' y retornamos el resultado.
    destroyQueue(q);
    return isBipartite;
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
        if (get_vertex_color(W->vertexOrdByTagArray[j]) == i) {
            count++;
        }
    }
    return count;
}

/* 
    Devuelve la cantidad de colores usados en el coloreo que tiene en su
    momento W.
*/
u32 NumeroDeColores(WinterIsHere W) {
    return W->NumeroDeColoresUsados;
}

/*
    Devuelve la etiqueta del vertice número 'i' en el orden guardado en ese
    momento en W.
*/
u32 IesimoVerticeEnElOrden(WinterIsHere W, u32 i) {
    if (i < W->NumeroDeVertices)
        return get_vertex_tag(W->vertexOrdByTagArray[i]);
    else
        return 0;
}

// Funciones de numeros pseudoaleatorios

/*
    Algoritmo Xorshift32 que devuelve un numero aleatorio 
    de 32 bits, el state inicial (seed) tiene que ser distinto de 0
*/
u32 xorshift32(u32 state[static 1])
{
    u32 x;
    // Pequeño fix por si el seed es '0', el cual este algoritmo no acepta.
    state[0] != 0 ? (x = state[0]) : (x = -1);
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    state[0] = x;
    return x;
}

/*
    Calcula un numero aleatorio entre 0 y 'n - 1' tomando como seed 'x'
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


// Funciones de ordenación

/*
    Ordena en orden creciente segun el nombre de los vertices.
*/
void OrdenNatural(WinterIsHere W) {
    qsort(W->vertexOrdByTagArray, W->NumeroDeVertices,
          sizeof(W->vertexOrdByTagArray[0]), cmpNaturalOrder);
}

/*
    Ordena en orden decreciente segun el grado de los vertices
*/
void OrdenWelshPowell(WinterIsHere W) {
    qsort(W->vertexOrdByTagArray, W->NumeroDeVertices,
          sizeof(W->vertexOrdByTagArray[0]), cmpWelshPowell);
}


/*
    Ordena pseudoaleatoriamente los vertices de W deterministicamente a partir
    del valor de la variable x
*/
void AleatorizarVertices(WinterIsHere W, u32 x) {
    u32 i, j;
    vertex auxVert;
    u32 n = W->NumeroDeVertices;

    // Los ordenamos de menor a mayor para que el algoritmo sea deterministico
    qsort(W->vertexOrdByTagArray, n, sizeof(W->vertexOrdByTagArray[0]),
          cmpMinToMax);

    // Implementación del algoritmo de shuffling de Fisher-Yates
    for (i = n - 1; i > 0; i--) {
        // Le pasamos la cantidad de elementos del arreglo 'n' y la variable
        // 'x' como seed a rand_u32()
        j = rand_u32(n, &x) % (i + 1);
        auxVert = W->vertexOrdByTagArray[j];
        W->vertexOrdByTagArray[j] = W->vertexOrdByTagArray[i];
        W->vertexOrdByTagArray[i] = auxVert;
    }
}


void orderByRandomBlocksOfColor(WinterIsHere W, u32 x) {
    u32 ncolores = NumeroDeColores(W);
    
    if (ncolores > 0){
        u32 *color_amount = calloc((ncolores + 1),sizeof(u32));
        u32 i, j, tmp;
        u32 n = W->NumeroDeVertices;

        // Recorremos el arreglo de vertices para contar la cantidad de vertices
        // que tiene cada color y lo guardamos en el arreglo color_amount
        for (i = 0; i < n; i++) {
            color_amount[get_vertex_color(W->vertexOrdByTagArray[i])]++;
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

        u32 *color_position = malloc((ncolores + 1) * sizeof(u32));
        color_position[random_color_order[0]] = 0;
        for (i = 1; i < ncolores + 1; i++) {
            color_position[random_color_order[i]] = color_position[random_color_order[i-1]] +
                                                    color_amount[random_color_order[i-1]];
        }

        vertex *orderedByColourVertexArray = malloc(W->NumeroDeVertices *
                                                    sizeof(vertex));
        u32 auxcol;
        for (u32 i = 0; i < W->NumeroDeVertices; i++) {
            // Obtenemos el color del vertice que estamos por acomodar, nos fijamos
            // en el arreglo de posiciones donde deberia ir ese vertice segun ese
            // color e incrementamos la posicion en el arreglo de posiciones por
            // color para que ocupe la siguiente posición
            auxcol = get_vertex_color(W->vertexOrdByTagArray[i]);
            orderedByColourVertexArray[color_position[auxcol]] = W->vertexOrdByTagArray[i];
            color_position[auxcol]++;
        }
        // Liberamos el arreglo anterior y asignamos el nuevo arreglo a W
        free(W->vertexOrdByTagArray);
        W->vertexOrdByTagArray = orderedByColourVertexArray;

        free(color_position);
        free(random_color_order);
        free(color_amount);
    }
}

void orderByColorMaxThenMinToMax(WinterIsHere W) {
    // El algoritmo deja sin utilizar la posicion '0' para mayor legibilidad
    // del codigo, asi cada posición del arreglo se corresponde a un color
    // desde el 1 hasta el maximo color
    u32 ncolores = NumeroDeColores(W);
    
    if (ncolores > 0){
        u32 *color_amount = calloc((ncolores + 1), sizeof(u32));

        // Recorremos el arreglo para contar los colores
        for (u32 i = 0; i < W->NumeroDeVertices; i++) {
            color_amount[get_vertex_color(W->vertexOrdByTagArray[i])]++;
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
        vertex *orderedByColourVertexArray = malloc(W->NumeroDeVertices * sizeof(vertex));

        u32 auxcol;

        for (u32 i = 0; i < W->NumeroDeVertices; i++) {
            // Obtenemos el color del vertice que estamos por acomodar, nos fijamos en el arreglo
            // de posiciones donde deberia ir ese vertice segun ese color e incrementamos la posicion
            // en el arreglo de posiciones por color para que ocupe la siguiente posición
            auxcol = get_vertex_color(W->vertexOrdByTagArray[i]);
            orderedByColourVertexArray[color_position[auxcol]] = W->vertexOrdByTagArray[i];
            color_position[auxcol]++;

        }
        // Liberamos el arreglo anterior y asignamos el nuevo arreglo a W
        free(W->vertexOrdByTagArray);
        W->vertexOrdByTagArray = orderedByColourVertexArray;

        free(color_amount);
        free(color_position);
    }
}

void orderByColorMaxToMin(WinterIsHere W) {
    qsort(W->vertexOrdByTagArray, W->NumeroDeVertices,
          sizeof(W->vertexOrdByTagArray[0]), cmpMaxToMinByColor);
}

/*
    Ordena el arreglo segun la cantidad de colores en el orden
    segun el valor del parametro 'order' de la forma:
    order = 0 -> orden creciente
    order = 1 -> orden decreciente
    
    El algoritmo toma en cuenta el color '0' pero si no existe ningun vertice
    con ese color no afecta al funcionamiento del mismo.
*/
void orderByAmountOfColor(WinterIsHere W, bool order) {
    u32 ncolores = NumeroDeColores(W);
    if (ncolores > 0) {
        colAmount *colAmountArray = malloc((ncolores + 1) * sizeof(struct ColAmountSt));
        u32 *color_amount = calloc((ncolores + 1), sizeof(u32));

        // Recorremos el arreglo para contar los colores
        for (u32 i = 0; i < W->NumeroDeVertices; i++)
            color_amount[get_vertex_color(W->vertexOrdByTagArray[i])]++;

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

        u32 *color_position = malloc((ncolores + 1) * sizeof(u32));
        color_position[colAmountArray[0]->color] = 0;
        for (u32 i = 1; i < ncolores + 1; i++) {
            u32 prevCol = colAmountArray[i-1]->color;
            u32 currentCol = colAmountArray[i]->color;
            color_position[currentCol] = color_position[prevCol] +
                                         color_amount[prevCol];
        }

        vertex *orderedByColourVertexArray = malloc(W->NumeroDeVertices *
                                                    sizeof(vertex));
        u32 auxcol;
        for (u32 i = 0; i < W->NumeroDeVertices; i++) {
            // Obtenemos el color del vertice que estamos por acomodar, nos fijamos
            // en el arreglo de posiciones donde deberia ir ese vertice segun ese
            // color e incrementamos la posicion en el arreglo de posiciones por
            // color para que ocupe la siguiente posición
            auxcol = get_vertex_color(W->vertexOrdByTagArray[i]);
            orderedByColourVertexArray[color_position[auxcol]] = W->vertexOrdByTagArray[i];
            color_position[auxcol]++;
        }

        // Liberamos el arreglo anterior y asignamos el nuevo arreglo a W
        free(W->vertexOrdByTagArray);
        W->vertexOrdByTagArray = orderedByColourVertexArray;
        
        free(color_position);
        free(color_amount);
        for (u32 i = 0; i < (ncolores + 1); i++) {
            free(colAmountArray[i]);
        }
        free(colAmountArray);
    }
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
        // Le pasamos 0 si queremos que ordene de manera creciente
        //            1 si queremos que ordene de manera decreciente
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
        if (get_vertex_tag(W->vertexOrdByTagArray[i]) == x) {
            vname = get_vertex_name(W->vertexOrdByTagArray[i]);
            break;
        }
    }
    return vname;
}

u32 ColorDelVertice(WinterIsHere W, u32 x) {
    u32 vcolor = 0;
    for (u32 i = 0; i < W->NumeroDeVertices; i++) {
        if (get_vertex_tag(W->vertexOrdByTagArray[i]) == x) {
            vcolor = get_vertex_color(W->vertexOrdByTagArray[i]);
            break;
        }
    }
    return vcolor;
}

u32 GradoDelVertice(WinterIsHere W, u32 x) {
    u32 vgrade = 0;
    for (u32 i = 0; i < W->NumeroDeVertices; i++) {
        if (get_vertex_tag(W->vertexOrdByTagArray[i]) == x) {
            vgrade = get_vertex_grade(W->vertexOrdByTagArray[i]);
            break;
        }
    }
    return vgrade;
}
/*
u32 IesimoVecino(WinterIsHere W, u32 x, u32 i) {
    vertex v = NULL;
    u32 tag = 0;
    for (u32 i = 0; i < W->NumeroDeVertices; i++) {
        if (get_vertex_tag(W->vertexOrdByTagArray[i]) == x) {
            v = W->vertexOrdByTagArray[i];
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
*/
// Funciones auxiliares

/*
    Devuelve 1 si el coloreo es propio, 0 si no
*//*
int isValidColoring(WinterIsHere W) {
    vertex v, w;
    neighb_t vertex_neigh_list;
    int isValid = 1;
    for (u32 i = 0; i < W->NumeroDeVertices && isValid; i++) {
        v = W->vertexOrdByTagArray[i];
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
*/
/* 
    Setea todos los colores de los vertices en 0
*/
void cleanColors(WinterIsHere W) {
    for (u32 i = 0; i < W->NumeroDeVertices; i++) {
        set_vertex_color(W->vertexOrdByTagArray[i], 0);
    }
}

void dumpvertexOrdByTagArray(WinterIsHere W, FILE *f) {
    for (u32 i = 0; i < W->NumeroDeVertices; i++) {
        fprintf(f, "Pos:%u, ", i);
        print_vertex_data(W->vertexOrdByTagArray[i]);
    }
}

// ARREGLAR ALGORITMOS DE ORDENES PARA QUE USEN 'vertexOrderArray'
int main() {
    printf("Loading...\n");
    WinterIsHere W = WinterIsComing();
    printf("Loading done.\n");

    /*for (u32 i = 0; i < W->NumeroDeVertices; i++)
        printf("%"SCNu32",", GradoDelVertice(W,IesimoVerticeEnElOrden(W,i)));
    printf("\n\n");*/
//    dumpvertexOrdByTagArray(W, stdout);
    if (W != NULL) {
//        vertex v = search_vertex(2647511047, W->vertexHashtable);
//        print_vertex_data(v);
//        print_vertex_data(W->vertexOrdByTagArray[0]);
//        dumpvertexOrdByTagArray(W, stdout);
//        printf("\n");
//        AleatorizarVertices(W, 1);
//        dumpvertexOrdByTagArray(W, stdout);
//        printf("\n");
//        OrdenNatural(W);
//        dumpvertexOrdByTagArray(W, stdout);
        printf("\n");
        //AleatorizarVertices(W, 5);
        ReordenManteniendoBloqueColores(W, 0);
        dumpvertexOrdByTagArray(W, stdout);


//        int isBipartite = Bipartito(W);
//        printf("Es bipartito? %d\n", isBipartite);
//        ReordenManteniendoBloqueColores(W, 1);
//        dumpvertexOrdByTagArray(W, stdout);
        printf("\n");

//        dumpvertexOrdByTagArray(W, stdout);
//        for (u32 i = 0; i < 10; i++){

//            printf("i: %"SCNu32"\n\n", i % 2);
//            dumpvertexOrdByTagArray(W, stdout);
//        }
//        printf("\nOrden aleatorio: 1\n");
//        dumpvertexOrdByTagArray(W, stdout);

//        for (u32 i = 0; i < 10; i++){
            
    //        printf("Orden Aleatorio 0\n=================\n");
    //        dumpvertexOrdByTagArray(W, stdout);
//        AleatorizarVertices(W, 1);
    //        printf("Orden Aleatorio 1\n=================\n");
    //        dumpvertexOrdByTagArray(W, stdout);
    //        AleatorizarVertices(W, 2);
    //        printf("Orden Aleatorio 2\n=================\n");
    //        dumpvertexOrdByTagArray(W, stdout);
    //        AleatorizarVertices(W, 1); 
    //        printf("Orden Aleatorio 1\n=================\n");
    //        dumpvertexOrdByTagArray(W, stdout);}
        
        /*
        AleatorizarVertices(W, 2);
        printf("Orden Aleatorio 2\n=================\n");
        dumpvertexOrdByTagArray(W, stdout);
        printf("\n");*/

    //    cleanColors(W);
//        Greedy(W);
//        printf("El coloreo es de: %"SCNu32"\n", NumeroDeColores(W));
//        dump_hashtable(W->vertexHashtable, stdout);
//        dump_array(W->vertexHashtable);
        Primavera(W);
    }
    return 0;
//    dumpvertexOrdByTagArray(W, stdout);

/*    for (u32 i = 0; i < W->NumeroDeVertices; i++)
        printf("%"SCNu32",", GradoDelVertice(W,IesimoVerticeEnElOrden(W,i)));
    printf("\n\n");*/
//    dumpvertexOrdByTagArray(W, stdout);
//    u32 tag = 15;
//    u32 neig = 0;
//    printf("NOMBRE: %"SCNu32"\n", NombreDelVertice(W,tag));
//    printf("\n\nTag: %"SCNu32", Vecino %"SCNu32" es %"SCNu32"\n", tag, neig, IesimoVecino(W, tag, neig));
//    print_vertex_data(search_vertex(NombreDelVertice(W, tag), W->vertexHashtable));
//    print_vertex_data(search_vertex(NombreDelVertice(W,IesimoVecino(W, tag, neig)), W->vertexHashtable));
//    print_vertex_data(search_vertex(NombreDelVertice(W,1), W->vertexHashtable));

//    u32 colores = Greedy(W);
/*    ReordenManteniendoBloqueColores(W, 2);
    dumpvertexOrdByTagArray(W, stdout);
    printf("\n");
    ReordenManteniendoBloqueColores(W, 3);
    dumpvertexOrdByTagArray(W, stdout);
    printf("\n");
    ReordenManteniendoBloqueColores(W, 1);
    dumpvertexOrdByTagArray(W, stdout);
    printf("Coloreo: %"SCNu32"\n", NumeroDeColores(W));
*/
}
