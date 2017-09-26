#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "vertex.h"

typedef struct QueueSt *queue;
typedef struct QueueNodeSt *qnode;

// Funciones de creación y destrucción
qnode createQueueNode(vertex v);
queue createQueue();
void destroyQueue(queue q);

// Funciones de operaciones en colas
bool isEmpty(queue q);
void enqueue(queue q, vertex v);
vertex dequeue(queue q);

// Funciones de impresión y debugging
void printQueue(queue q);