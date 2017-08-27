#include "test.h"

struct _neighb_t {
	vertex neighb;
	list_t next;
};

struct _graphSt {
    u32 vertexCount;
    u32 edgeCount;
    u32 arraySize;
    vertex *vertexArray; // array of vertices we are storing
};

struct _vertexSt {
    u32 name;
    u32 colour;
    u32 grade;
    u32 neigh;
    list_t neighList;
};


list_t list_empty(void) {
	list_t list = NULL;

	return list;
}

list_t list_destroy(list_t list) {
	list_t aux;
	
	while (list != NULL) {
		aux = list;
		list = list->next;
        // destroy the item if it has to
		free(aux);
	}
	return list;
}

list_t list_append(list_t list, vertex neighb) {

	list_t aux;
	list_t newnode = NULL;

    newnode = calloc(1, sizeof(struct _neighb_t));
	
	newnode->neighb = neighb;
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
	
	return list;
}

void print_list(list_t list) {

	list_t auxlist = list;
	vertex v1;
	while(auxlist != NULL)
	{
	    v1 = auxlist->neighb;
		
		if(auxlist->next != NULL)
		    printf("%" SCNu32 ",", v1->name);
		else    // it's the last element
		    printf("%" SCNu32 ".", v1->name); 
		
		auxlist = auxlist->next;
	}
// PRE: 'list' is a valid list pointer.
}

void print_all_neighs(graph g) {
    for (int i = 0; i < g->vertexCount; i++)
    {
        printf("vertex: %d neighbours are: \n", g->vertexArray[i]->name);
        printVertexNeighs(g->vertexArray[i]);
        printf("\n");
    }
}


int getVertexPosition(graph g, u32 vertexName) {
    int place = -1;
    int found = 0;
    int k;

    k = g->arraySize - 1;
    while(k >= 0 && !found)
    {
        if (g->vertexArray[k]->name == vertexName) {
            found = 1;
            place = k;
        }
        k--;
    }
    return place;
}
// getVertexPosition returns the position of a vertex inside an array
// if it doesnt find it, it returns -1

vertex addVertexToGraph(graph g, u32 vertexName)
{
    vertex v = calloc(1, sizeof(struct _vertexSt));
    v->grade = 1;
    v->neighList = list_empty();
    v->name = vertexName;
    v->colour = 0;

    g->vertexArray[g->arraySize] = v;
    g->arraySize++;
    printf("size: %d\n", g->arraySize);

    return v;
}
// creates a new vertex with the given vertexName, and returns a pointer
// to the new vertex


graph newGraph() {
    graph g = NULL;

    g = calloc(1, sizeof(struct _graphSt));

    char input[1024];
    int vCount, eCount;

    printf("Enter vertices count: \n");
    fgets(input, sizeof(input), stdin);
    sscanf(input, "%d", &vCount);

    printf("Enter edge count: \n");
    fgets(input, sizeof(input), stdin);
    sscanf(input, "%d", &eCount);

    printf("\n");

    g->vertexCount = vCount;
    g->edgeCount = eCount;

    g->vertexArray = calloc(g->vertexCount, sizeof(struct _vertexSt));
    g->arraySize = 0;

    u32 v1Name, v2Name;
    int j = 0;
    
    while(j < g->edgeCount)
    {
        printf("size: %d\n", g->arraySize);
        fgets(input, sizeof(input), stdin);
        sscanf(input, "%" SCNu32 " %" SCNu32, &v1Name, &v2Name);

        int posFoundV1 = getVertexPosition(g, v1Name);
        int posFoundV2 = getVertexPosition(g, v2Name);
        vertex v1 = NULL;
        vertex v2 = NULL;

        if(posFoundV1 == -1)
        {
            v1 = addVertexToGraph(g, v1Name);
        } else {
            g->vertexArray[posFoundV1]->grade++;
        }

        if(posFoundV2 == -1)
        {
            v2 = addVertexToGraph(g, v2Name);
        } else {
            g->vertexArray[posFoundV2]->grade++;
        }

        if (v1 == NULL) {
            v1 = g->vertexArray[posFoundV1];
        }

        if (v2 == NULL) {
            v2 = g->vertexArray[posFoundV2];
        }
        
        v2->neighList = list_append(v2->neighList, v1);
        v1->neighList = list_append(v1->neighList, v2);
        
        // We add the neighbours to the list of each vertex
        j++;
    }

    print_all_neighs(g);

    return g;
}

int destroyGraph(graph g) {
    int success = 1;
    for (int i = 0; i < g->vertexCount; i++) {
        (g->vertexArray[i])->neighList = list_destroy((g->vertexArray[i])->neighList);
        success &= ((g->vertexArray[i]->neighList) == NULL);
        free(g->vertexArray[i]);
    }
    free(g->vertexArray);
    free(g);
    return success;
}

int getVertexCount(graph g)
{
    return g->vertexCount;
}

int main()
{
    graph g = newGraph();
    printf("vertex count: %d\n", getVertexCount(g));
    printColourVertices(g, 1);
    destroyGraph(g);
}

u32 vertexColor(vertex x) {
    return x->colour;
}

u32 vertexGrade(vertex x) {
    return x->grade;
}

u32 vertexName(vertex x) {
    return x->name;
}

void printVertexNeighs(vertex x) {
    print_list(x->neighList);
}

u32 vertexCount(graph g) {
    return g->vertexCount;
}

u32 edgeCount(graph g) {
    return g->edgeCount;
}

vertex ithOrderVertex(graph g, u32 i) {
    return g->vertexArray[i];
}

vertex ithNeigh(graph g, vertex x, u32 i) {
    list_t auxlist = x->neighList;

    int j = 0;
    while(auxlist != NULL && j != i)
    {
        auxlist = auxlist->next;
        j++;
    }

    return auxlist->neighb;
}

u32 colourVertexCount(graph g, u32 i) {
    int count = 0;
    for (int j = 0; j < g->vertexCount; j++) {
        if (g->vertexArray[j]->colour == i) {
            count++;
        }
    }
    return count;
}

u32 printColourVertices(graph g, u32 i) {
    printf("Vertices de Color %d: ", i);
    
    int count = 0;
    u32 amount = colourVertexCount(g, i);
    
    if(amount == 0) {
        printf("No hay vertices de color %d\n", i);
    } else {
        for (int j = 0; j < g->vertexCount; j++) {
            if (g->vertexArray[j]->colour == i) {
                if (count < amount - 1)
                    printf("%d,", g->vertexArray[j]->name);
                else
                    printf("%d.\n", g->vertexArray[j]->name);
                count++;
            }
        }
    }
    return amount;
}

u32 colorAmount(graph g) {
    u32 maxColour = 0;
    for (int j = 0; j < g->vertexCount; j++) {
        if (g->vertexArray[j]->colour > maxColour) {
            maxColour = g->vertexArray[j]->colour;
        }
    }
    return maxColour;
}

int chidos(graph g) {

}

u32 greedy(graph g) {

}

void naturalOrder(graph g) {

}

void welshPowellOrder(graph g) {

}

void restrictedRandomReorder(graph g) {

}

void bigToSmall(graph g) {

}

void smallToBig(graph g) {

}

void revert(graph g) {

}

void specificOrder(graph g,u32* x) {

}
