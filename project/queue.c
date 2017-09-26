#include "queue.h"

struct QueueSt {
    qnode front, rear;
};

struct QueueNodeSt {
    vertex vertex_pt;
    qnode next;
};

// Funciones de creación y destrucción

qnode createQueueNode(vertex v){
    qnode n = malloc(sizeof(struct QueueNodeSt));
    n->vertex_pt = v;
    n->next = NULL;
    return n;
}

queue createQueue(){
    queue q = malloc(sizeof(struct QueueSt));
    q->front = q->rear = NULL;
    return q;
}

void destroyQueue(queue q){
    if (q != NULL){
        qnode aux;
        while (q->front != NULL) {
            aux = q->front->next;
            dequeue(q);
            q->front = aux;
        }
        free(q);
    }
}

// Funciones de operaciones en colas

bool isEmpty(queue q){
    return (q->front == NULL);
}

void enqueue(queue q, vertex v){
    if (q != NULL){
        qnode n = createQueueNode(v);
        if (q->rear == NULL){
            q->front = q->rear = n;
        } else {
            q->rear->next = n;
            q->rear = n;
        }
    }
}

vertex dequeue(queue q){
    if (q != NULL){
        if (q->front == NULL){
            return NULL;
        } else {
            qnode n = q->front;
            vertex v = n->vertex_pt;
            q->front = q->front->next;
            if (q->front == NULL)
                q->rear = NULL;
            free(n);
            return v;
        }
    } else {
        return NULL;
    }
}

// Funciones de impresión y debugging

void printQueue(queue q){
    if (q != NULL){
        qnode aux = q->front;
        printf("[");
        while (aux != NULL){
            vertex v1 = aux->vertex_pt;
            printf("%d", get_vertex_tag(v1));
            aux = aux->next;
            if (aux != NULL)
                printf(":");
        }
        printf("]\n");
    }
}