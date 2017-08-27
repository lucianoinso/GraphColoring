#include "vertex.h"

vertex create_vertex(u32 tag, u32 name, u32 color, u32 grade) {
    vertex v = calloc(1, sizeof(struct VertexSt));
    set_vertex_tag(tag, v);
    set_vertex_name(name, v);
    set_vertex_color(color, v);
    set_vertex_grade(grade, v);
    v->neighList = NULL;
    v->lastNeigh = NULL;
    return v;
}

vertex destroy_vertex(vertex v) {
    v->neighList = list_destroy(v->neighList);
    free(v);
    v = NULL;
    return v;
}

void set_vertex_tag(u32 t, vertex v) {
    v->tag = t;
}

void set_vertex_color(u32 c, vertex v) {
    v->color = c;
}

void set_vertex_grade(u32 grade, vertex v) {
    v->grade = grade;
}

void set_vertex_name(u32 name, vertex v) {
    v->name = name;
}

u32 get_vertex_tag(vertex v) {
    if (v != NULL) {
        return v->tag;
    } else {
        return 0;
    }
}

u32 get_vertex_color(vertex v) {
    if (v != NULL) {
        return v->color;
    } else {
        return 0;
    }
}

u32 get_vertex_grade(vertex v) {
    if (v != NULL) {
        return v->grade;
    } else {
        return 0;
    }
}

u32 get_vertex_name(vertex v) {
    if (v != NULL) {
        return v->name;
    } else {
        return 0;
    }
}

neighb_t get_vertex_neigh_list(vertex v){
    return v->neighList;
}

neighb_t get_last_neigh(vertex v){
    return v->lastNeigh;
}

neighb_t list_empty(void) {
    neighb_t list = NULL;
    return list;
}

void append_vertex_to_neigh_list(vertex vneigh, vertex v) {
    neighb_t aux = v->neighList;
    neighb_t new_neighb = NULL;

    new_neighb = calloc(1, sizeof(struct NeighbSt));
    new_neighb->vertex_pt = vneigh;
    new_neighb->next = NULL;

    if (aux == NULL) {
        v->neighList = new_neighb;
        v->lastNeigh = new_neighb;
    } else {    // list tiene al menos un elemento
        (v->lastNeigh)->next = new_neighb;
        v->lastNeigh = new_neighb;
    }
}


void make_vertex_neighbs(vertex v1, vertex v2){
    append_vertex_to_neigh_list(v1,v2);
    append_vertex_to_neigh_list(v2,v1);
}

neighb_t list_destroy(neighb_t list) {
    neighb_t aux;
    
    while (list != NULL) {
        aux = list;
        list = list->next;
        // destroy the item if it has to
        free(aux);
    }
    return list;
}

void print_neigh_list(neighb_t list) {
    neighb_t auxlist = list;
    vertex v1;
    printf("Neighbours: ");
    while(auxlist != NULL) {
        v1 = auxlist->vertex_pt;

        if(auxlist->next != NULL) {
            printf("%" SCNu32 ", ", v1->name);
        } else {
            printf("%" SCNu32 "\n", v1->name); 
        }

        auxlist = auxlist->next;
    }
    printf("\n");
}

void printVertexNeighs(vertex v) {
    print_neigh_list(v->neighList);
}

void print_vertex_data(vertex v) {
    printf("Tag: %"SCNu32" Name: %"SCNu32" Color: %"SCNu32" Grade: %"SCNu32" \n",
            get_vertex_tag(v), get_vertex_name(v), get_vertex_color(v),
            get_vertex_grade(v));
    printVertexNeighs(v);
}


void print_all_neighs_data(vertex v) {
    neighb_t nglist = get_vertex_neigh_list(v);
    printf("Vertex %"SCNu32 " neighbours data:\n", v->name);
    while (nglist != NULL) {
        print_vertex_data(nglist->vertex_pt);
        nglist = nglist->next;
    }
}

void modify_vertex_values(u32 tag, u32 name, u32 color, u32 grade, vertex v) {
    v->tag = tag;
    v->name = name;
    v->color = color;
    v->grade = grade;
}

/*
int main(void) {

    vertex v1 = create_vertex(1, 1 ,0, 3);
    print_vertex_data(v1);

    vertex v2 = create_vertex(2, 2 ,0, 3);
    print_vertex_data(v2);

    vertex v3 = create_vertex(3, 3 ,0, 3);
    print_vertex_data(v3);

    vertex v4 = create_vertex(4, 4 ,0, 2);
    print_vertex_data(v4);

    vertex v5 = create_vertex(5, 5 ,0, 3);
    print_vertex_data(v5);

    make_vertex_neighbs(v1, v2);
    make_vertex_neighbs(v1, v3);
    make_vertex_neighbs(v1, v5);
    make_vertex_neighbs(v2, v5);
    make_vertex_neighbs(v2, v3);
    make_vertex_neighbs(v3, v4);
    make_vertex_neighbs(v4, v5);

    printf("Vertex 1 neighs:\n");
    printVertexNeighs(v1);
    printf("Vertex 2 neighs:\n");
    printVertexNeighs(v2);
    printf("Vertex 3 neighs:\n");
    printVertexNeighs(v3);
    printf("Vertex 4 neighs:\n");
    printVertexNeighs(v4);
    printf("Vertex 5 neighs:\n");
    printVertexNeighs(v5);
    printf("\n");

    print_all_neighs_data(v4);

    v1 = destroy_vertex(v1);
    v2 = destroy_vertex(v2);
    v3 = destroy_vertex(v3);
    v4 = destroy_vertex(v4);
    v5 = destroy_vertex(v5);

    printf("\n");
    return 0;
}*/
