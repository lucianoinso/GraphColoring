struct _vertexSt {
    u32 name;
    u32 colour;
    u32 grade;
    list_t neighbs;
};

struct _graphSt {
    HashTable vertexTable;
    u32 vertexCount;
    u32 edgeCount;
};

typedef struct _graphSt *graph;
typedef struct _vertexSt *vertex;

/////////////////////////////
// lists                   //
/////////////////////////////
//#include "list.h"

typedef struct _node_t  *list_t;

struct _node_t{
	vertex v;
	list_t next;
};

list_t list_empty(void){
	list_t list = NULL;

	return list;
}

list_t list_destroy(list_t list){
	list_t aux;
	
	while (list != NULL){
		aux = list;
		list = list->next;
		vertex_destroy(aux->elem);
		free(aux);
	}
	return list;
}

unsigned int list_length(list_t list){
    unsigned int length = 0;
	list_t auxlist = list;
	
    while (auxlist!=NULL){
    	length++;
    	auxlist = auxlist->next;
    }
    return length;
}

data_t list_search(list_t list, index_t index){

	list_t aux = NULL;
	list_t auxlist = list;
	
	bool found = false;
	while (auxlist != NULL && !found)
	{
		found = index_is_equal((pair_fst(auxlist->elem)) ,index);

		if (found)
			aux = auxlist;
		
		auxlist = auxlist->next;
	}
	
	if (found)
		return pair_snd(aux->elem);
	else
		return NULL;
	
/*
 * The caller must NOT free the resources allocated for the result when done
 * using it.
 */
 
}

list_t list_append(list_t list, index_t index, data_t data){

	unsigned int oldlength = list_length(list);

	list_t aux;
	list_t newnode = NULL;

    newnode = calloc(1, sizeof(struct _node_t));

	pair_t pair = pair_from_index_data(index, data);
	
	newnode->elem = pair;
	newnode->next = NULL;

	if (list == NULL)
		list = newnode;
	else // list tiene al menos un elemento
	{
		aux = list;
		while(aux->next != NULL)
			aux = aux->next;

		aux->next = newnode;
	}
	
	aux = NULL;
	assert(list_length(list)  == oldlength + 1);
	
	return list;

/*
 * The given 'index' and 'data' are inserted in the list,
 * so they can not be destroyed by the caller.
 */
}


void print_list(list_t list){

	list_t auxlist = list;
	while(auxlist != NULL)
	{
	    vertexSt vertex = auxlist->vertex;
		printf("%s,", (list->vertex).name);
		auxlist = auxlist->next;
	}
	printf(".");
// PRE: 'list' is a valid list pointer, and 'fd' is a valid file descriptor.
}

/////////////////////////////// END LIST FUNCTIONS ////////////////////


graph newGraph() {
    // allocar espacio, lee un grafo desde STDIN y llena
    // el grafo con esos datos
    // ademas de cargar, asigna el color 0 a cada vertice para indicar
    // que estan todos sin colorear
    
    // en caso de error devolver NULL (formato no valido de entrada, 
    // no se pudo allocar la memoria

    // ProcessTheInput
    graph g = NULL

    g = calloc(1, sizeof(Struct _graphSt));

    scanf("%d", vCount);
    scanf("%d", eCount);

    g->vertexCount = vCount;
    g->edgeCount = eCount;

    return g;
}

int destroyGraph(graph g) {
    // destruye g y libera la memoria alocada. retorna 1 si todo anduvo bien
    // 0 si no.
}

u32 vertexColor(vertexSt x) {
    return x->colour;
}

u32 GradoDelVertice(vertexSt x) {
    return x->grade;
}

u32 NombreDelVertice(vertexSt x) {
    return x->name;
}

void ImprimirVecinosDelVertice(vertexSt x, graph g) {
    print_list(x->neighbs);
}

u32 NumeroDeVertices(graph g) {
    return g->vertexCount;
}

u32 NumeroDeLados(graph g) {
    return g->edgeCount;
}

u32 NumeroVerticesDeColor(graph g, u32 i) {

}

u32 ImprimirVerticesDeColor(graph g, u32 i) {

}

u32 CantidadDeColores(graph g) {

}

vertexSt IesimoVerticeEnElOrden(graph g, u32 i) {

}

vertexSt IesimoVecino(graph g, vertexSt x, u32 i) {

}

int Chidos(graph g) {

}

u32 Greedy(graph g) {

}

void OrdenNatural(graph g) {
}

void OrdenWelshPowell(graph g) {
}

void ReordenAleatorioRestringido(graph g) {

}

void GrandeChico(graph g) {

}

void ChicoGrande(graph g) {

}

void Revierte(graph g) {

}

void OrdenEspecifico(graph g,u32* x) {

}
