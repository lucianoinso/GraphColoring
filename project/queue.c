#include "queue.h"

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

/*int main() {
    vertex v1 = create_vertex(1, 1, 0, 0);
    vertex v2 = create_vertex(2, 2, 0, 0);
    vertex v3 = create_vertex(3, 3, 0, 0);
    queue q = createQueue();
    enqueue(q, v2);
    printQueue(q);
    enqueue(q, v1);
    printQueue(q);
    enqueue(q, v3);
    printQueue(q);
    enqueue(q, v1);
    printQueue(q);
    vertex v4 = dequeue(q);
    printQueue(q);
    v4 = dequeue(q);
    printQueue(q);
    v4 = dequeue(q);
    printQueue(q);
    v4 = dequeue(q);
    printQueue(q);

//    v4 = dequeue(q);
//    v4 = dequeue(q);

    if (v4 != NULL)
        printf("Dequeued item is %d\n", get_vertex_tag(v4));

    destroyQueue(q);
    destroy_vertex(v1);
    destroy_vertex(v2);
    destroy_vertex(v3);
    return 0;
}*/