#include <stdlib.h>
#include <stdio.h>
#include "vertex.h"

typedef struct QueueSt *queue;
typedef struct QueueNodeSt *qnode;

struct QueueSt {
    qnode front, rear;
};

struct QueueNodeSt {
    vertex vertex_pt;
    qnode next;
};

qnode createNode(vertex v);
queue createQueue();
void enqueue(queue q, vertex v);
vertex dequeue(queue q);
void destroyQueue(queue q);
void printQueue(queue q);