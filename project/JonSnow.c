#include "JonSnow.h"
// Indica el largo maximo de caracteres de cada una de las lineas de entrada.
#define MAX_LINE 4096

struct WinterSt {
    u32 NumeroDeVertices;
    u32 NumeroDeLados;
    u32 NumeroDeColoresUsados;
    vertex *vertexOrdByTagArray; // Arreglo de punteros a los vertices en orden
                                 // creciente segun sus tags.
    hashtable vertexHashtable;  // Arreglo de la hashtable
    u32 *vertexOrderArray;  // Arreglo de tags con el orden en el que se corre
                            // Greedy.
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
    u32 maxColor, auxColor, neighTag;;
    u32 *colorArray = NULL;

    // Elegimos el primer vertice segun el orden en W->vertexOrderArray
    vertex v = W->vertexOrdByTagArray[W->vertexOrderArray[0]];
    // Le ponemos el color 1
    set_vertex_color(v, 1);
    maxColor = 1;
    for (u32 i = 1 ; i < W->NumeroDeVertices; i++) {
        // Iteramos desde el 2ndo vertice hasta el último en el orden dado
        // en W->vertexOrderArray para colorearlos en ese orden.
        v = W->vertexOrdByTagArray[W->vertexOrderArray[i]];

        // Creamos un arreglo de colores teniendo en cuenta el color maximo
        // obtenido hasta el momento para el tamaño del mismo.
        colorArray = calloc(maxColor, sizeof(u32));

        // Obtenemos la lista de los vecinos del vertice que estamos por
        // colorear
        neighb_t vNeighList = get_vertex_neigh_list(v);

        // Iteramos dentro de la lista en el orden en el que están
        // y vamos llenando el arreglo de colores segun vamos viendo que estan
        // siendo usados.
        while(vNeighList != NULL) {
            neighTag = vNeighList->vNeighTag;
            if (get_vertex_color(W->vertexOrdByTagArray[neighTag]) != 0) {
                // Decrementamos en 1 porque hacemos una relacion de colores a
                // indices de la forma: colores de 1 a N, indice de 0 a N-1,
                // indice = color - 1
                colorArray[get_vertex_color(W->vertexOrdByTagArray[neighTag]) - 1] = 1;
            }
            // Pasamos al siguiente vecino hasta que se acabe la lista.
            vNeighList = vNeighList->next;
        }
        // En auxColor guardaremos el minimo color no usado entre los vecinos
        // para luego asignarselo al vertice que estamos coloreando.
        auxColor = 0;
        // Mientras auxColor sea menor al color máximo usado en ese momento en
        // el grafo y ese color no esté ya asignado a algun vecino dentro del
        // arreglo de los colores de los vecinos 'colorArray' incrementamos.
        while ((auxColor < maxColor) && (colorArray[auxColor] == 1)) {
            auxColor++;
        }
        // En este momento ya tenemos el minimo color con el que podemos
        // colorear al vertice que no está utilizado por sus vecinos pero en la
        // forma en el que el arreglo guarda los colores (de 0 a n-1) por lo
        // tanto incrementamos 1 vez para obtener el color real.
        auxColor++;

        // Seteamos el vertice con el color obtenido.
        set_vertex_color(v, auxColor);
        // Nos fijamos si el color es mas grande que maxColor y si lo es lo
        // reemplazamos para que sirva para la próxima iteración y para obtener
        // el numero cromatico del grafo al finalizar el algoritmo.
        if (auxColor > maxColor)
            maxColor = auxColor;
        // Liberamos el arreglo de colores.
        free(colorArray);
        colorArray = NULL;
    }
    // Seteamos el numero de colores usados con el color máximo obtenido
    // durante el coloreo del grafo.
    W->NumeroDeColoresUsados = maxColor;
    return maxColor;
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
            v = W->vertexOrdByTagArray[W->vertexOrderArray[i]];
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
    vertexOrd *auxVOrdArray = malloc(W->NumeroDeVertices * sizeof(struct VertexOrdSt));
    for (u32 i = 0; i < W->NumeroDeVertices; i++){
        auxVOrdArray[i] = malloc(sizeof(struct VertexOrdSt));
        auxVOrdArray[i]->tag = i;
        auxVOrdArray[i]->order = get_vertex_name(W->vertexOrdByTagArray[i]);
    }

    qsort(auxVOrdArray, W->NumeroDeVertices, sizeof(auxVOrdArray[0]), cmpNaturalOrder);

    for (u32 i = 0; i < W->NumeroDeVertices; i++){
        W->vertexOrderArray[i] = auxVOrdArray[i]->tag;
        free(auxVOrdArray[i]);
    }
    free(auxVOrdArray);
}

/*
    Ordena en orden decreciente segun el grado de los vertices
*/
void OrdenWelshPowell(WinterIsHere W) {
    vertexOrd *auxVOrdArray = malloc(W->NumeroDeVertices * sizeof(struct VertexOrdSt));
    for (u32 i = 0; i < W->NumeroDeVertices; i++){
        auxVOrdArray[i] = malloc(sizeof(struct VertexOrdSt));
        auxVOrdArray[i]->tag = i;
        auxVOrdArray[i]->order = get_vertex_grade(W->vertexOrdByTagArray[i]);
    }

    qsort(auxVOrdArray, W->NumeroDeVertices, sizeof(auxVOrdArray[0]), cmpWelshPowell);

    for (u32 i = 0; i < W->NumeroDeVertices; i++){
        W->vertexOrderArray[i] = auxVOrdArray[i]->tag;
        free(auxVOrdArray[i]);
    }
    free(auxVOrdArray);
}


/*
    Ordena pseudoaleatoriamente los vertices de W deterministicamente a partir
    del valor de la variable x
*/
void AleatorizarVertices(WinterIsHere W, u32 x) {
    u32 i, j, auxTag;
    u32 n = W->NumeroDeVertices;

    // Los ordenamos de menor a mayor para que el algoritmo sea deterministico
    qsort(W->vertexOrderArray, n, sizeof(W->vertexOrderArray[0]), cmpMinToMax);

    // Implementación del algoritmo de shuffling de Fisher-Yates
    for (i = n - 1; i > 0; i--) {
        // Le pasamos la cantidad de elementos del arreglo 'n' y la variable
        // 'x' como seed a rand_u32()
        j = rand_u32(n, &x) % (i + 1);
        auxTag = W->vertexOrderArray[j];
        W->vertexOrderArray[j] = W->vertexOrderArray[i];
        W->vertexOrderArray[i] = auxTag;
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

        u32 auxcol;
        for (u32 i = 0; i < W->NumeroDeVertices; i++) {
            // Obtenemos el color del vertice que estamos por acomodar, nos fijamos
            // en el arreglo de posiciones donde deberia ir ese vertice segun ese
            // color e incrementamos la posicion en el arreglo de posiciones por
            // color para que ocupe la siguiente posición
            auxcol = get_vertex_color(W->vertexOrdByTagArray[i]);
            W->vertexOrderArray[color_position[auxcol]] = get_vertex_tag(W->vertexOrdByTagArray[i]);
            color_position[auxcol]++;
        }
        // Liberamos el arreglo anterior y asignamos el nuevo arreglo a W

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

        // Creamos un arreglo de posiciones para saber donde guardar el
        // siguiente vertice de color i, siendo el indice del arreglo el color
        // y el contenido la posición
        u32 *color_position = malloc((ncolores + 1) * sizeof(u32));

        // Ponemos que los vertices que tienen el color maximo (ncolores)
        // Empiezan en la posición 0, por la especificación de la función
        // cuando x = 0
        color_position[ncolores] = 0;   
        
        // Ponemos que los vertices de color 1 empiezen a ubicarse
        // desde la posicion siguiente a la cantidad de vertices
        // totales de color ncolores, ya que son los primeros
        // en el arreglo
        color_position[1] = color_amount[ncolores];
        
        for (u32 i = 2; i < (ncolores); i++) {
            // Ponemos las posiciones iniciales de los vertices que siguen a
            // los del color 1 segun la posicion del color anterior mas la
            // cantidad de vertices esperados del color anterior
            color_position[i] = color_position[i - 1] + color_amount[i - 1];
        }

        u32 auxcol;

        for (u32 i = 0; i < W->NumeroDeVertices; i++) {
            // Obtenemos el color del vertice que estamos por acomodar, nos
            // fijamos en el arreglo de posiciones donde deberia ir ese vertice 
            // segun ese color, lo insertamos e incrementamos la posicion en el
            // arreglo de posiciones por color para que ocupe la siguiente
            // posición
            auxcol = get_vertex_color(W->vertexOrdByTagArray[i]);
            W->vertexOrderArray[color_position[auxcol]] = get_vertex_tag(W->vertexOrdByTagArray[i]);
            color_position[auxcol]++;
        }

        free(color_amount);
        free(color_position);
    }
}

void orderByColorMaxToMin(WinterIsHere W) {
    vertexOrd *auxVOrdArray = malloc(W->NumeroDeVertices * sizeof(struct VertexOrdSt));
    for (u32 i = 0; i < W->NumeroDeVertices; i++){
        auxVOrdArray[i] = malloc(sizeof(struct VertexOrdSt));
        auxVOrdArray[i]->tag = i;
        auxVOrdArray[i]->order = get_vertex_color(W->vertexOrdByTagArray[i]);
    }

    qsort(auxVOrdArray, W->NumeroDeVertices, sizeof(auxVOrdArray[0]), cmpMaxToMinByColor);

    for (u32 i = 0; i < W->NumeroDeVertices; i++){
        W->vertexOrderArray[i] = auxVOrdArray[i]->tag;
        free(auxVOrdArray[i]);
    }
    free(auxVOrdArray);
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

        u32 auxcol;
        for (u32 i = 0; i < W->NumeroDeVertices; i++) {
            // Obtenemos el color del vertice que estamos por acomodar, nos fijamos
            // en el arreglo de posiciones donde deberia ir ese vertice segun ese
            // color e incrementamos la posicion en el arreglo de posiciones por
            // color para que ocupe la siguiente posición
            auxcol = get_vertex_color(W->vertexOrdByTagArray[i]);
            W->vertexOrderArray[color_position[auxcol]] = get_vertex_tag(W->vertexOrdByTagArray[i]);
            color_position[auxcol]++;
        }
        
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
    u32 name = 0;
    if (x < W->NumeroDeVertices){
        name = get_vertex_name(W->vertexOrdByTagArray[x]);
    }
    return name;
}

u32 ColorDelVertice(WinterIsHere W, u32 x) {
    u32 color = 0;
    if (x < W->NumeroDeVertices){
        color = get_vertex_color(W->vertexOrdByTagArray[x]);
    }
    return color;
}

u32 GradoDelVertice(WinterIsHere W, u32 x) {
    u32 grade = 0;
    if (x < W->NumeroDeVertices){
        grade =  get_vertex_grade(W->vertexOrdByTagArray[x]);
    }
    return grade;
}

u32 IesimoVecino(WinterIsHere W, u32 x, u32 i) {
    u32 tag = 0;

    if (x < W->NumeroDeVertices) {
        vertex v = W->vertexOrdByTagArray[x];
        neighb_t auxList = get_vertex_neigh_list(v);
        u32 j = 0;
        while(auxList != NULL && j != i) {
            auxList = auxList->next;
            j++;
        }
        if(auxList != NULL) {
            tag = auxList->vNeighTag;
        }
    }
    return tag;
}

// Funciones auxiliares

/*
    Devuelve 1 si el coloreo es propio, 0 si no
*/
int isValidColoring(WinterIsHere W) {
    vertex v, w;
    neighb_t vertex_neigh_list;
    int isValid = 1;
    for (u32 i = 0; i < W->NumeroDeVertices && isValid; i++) {
        v = W->vertexOrdByTagArray[i];
        vertex_neigh_list = get_vertex_neigh_list(v);
        while (vertex_neigh_list != NULL) {
            w = W->vertexOrdByTagArray[vertex_neigh_list->vNeighTag];
            if (get_vertex_color(w) == get_vertex_color(v)) {
                isValid = 0;
                break;
            }
            vertex_neigh_list = vertex_neigh_list->next;
        }
    }
    return isValid;
}

/* 
    Setea todos los colores de los vertices en 0
*/
void cleanColors(WinterIsHere W) {
    for (u32 i = 0; i < W->NumeroDeVertices; i++) {
        set_vertex_color(W->vertexOrdByTagArray[i], 0);
    }
    W->NumeroDeColoresUsados = 0;
}

void dumpVertexOrdByTagArray(WinterIsHere W) {
    for (u32 i = 0; i < W->NumeroDeVertices; i++) {
        printf("Pos:%u, ", i);
        print_vertex_data(W->vertexOrdByTagArray[W->vertexOrderArray[i]]);
    }
}

int main() {
    printf("Loading...\n");
    WinterIsHere W = WinterIsComing();
    printf("Loading done.\n");
    printf("Coloring is: %"SCNu32 "\n", W->NumeroDeColoresUsados);
    Primavera(W);
    return 0;
}