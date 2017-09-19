#include "testing.h"

void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
}

/* void ImprimirTodosLosVecinos(WinterIsHere W) {
    for (int i = 0; i < W->NumeroDeVertices; i++)
    {
        printf("vertex: %d neighbours are: \n", W->vertexArray[i]->name);
        printVertexNeighs(W->vertexArray[i]);
        printf("\n");
    }
}
*/

/*
u32 printColourVertices(WinterIsHere W, u32 i) {
    printf("Vertices de Color %d: ", i);

    int count = 0;
    u32 amount = NumeroVerticesDeColor(W, i);
    
    if(amount == 0) {
        printf("No hay vertices de color %d\n", i);
    } else {
        for (int j = 0; j < NumeroDeVertices(W); j++) {
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