#include "vertex.h"

// Funciones de creación y destrucción

vertex create_vertex(u32 tag, u32 name, u32 color, u32 grade) {
    vertex v = calloc(1, sizeof(struct VertexSt));
    
    if (v != NULL){
        v->tag = tag;
        v->name = name;
        v->color = color;
        v->grade = grade;
        v->neighList = NULL;
        v->lastNeigh = NULL;
    }
    return v;
}

neighb_t destroy_list(neighb_t list) {
    neighb_t aux;
    
    while (list != NULL) {
        aux = list;
        list = list->next;
        // destroy the item if it has to
        free(aux);
    }
    return list;
}

vertex destroy_vertex(vertex v) {
    v->neighList = destroy_list(v->neighList);
    free(v);
    v = NULL;
    return v;
}

// Funciones getters

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
    if (v != NULL){
        return v->neighList;
    } else {
        return NULL;
    }
}

neighb_t get_last_neigh(vertex v){
    if (v != NULL){
        return v->lastNeigh;
    } else {
        return NULL;
    }
}

// Funciones de seteo y modificación

void set_vertex_tag(vertex v, u32 t) {
    if (v != NULL) {
        v->tag = t;
    }
}

void set_vertex_color(vertex v, u32 c) {
    if (v != NULL) {
        v->color = c;
    }
}

void set_vertex_grade(vertex v, u32 grade) {
    if (v != NULL) {
        v->grade = grade;
    }
}

void set_vertex_name(vertex v, u32 name) {
    if (v != NULL) {
        v->name = name;
    }
}

void modify_vertex_values(u32 tag, u32 name, u32 color, u32 grade, vertex v) {
    if (v != NULL){
        v->tag = tag;
        v->name = name;
        v->color = color;
        v->grade = grade;
    }
}

void append_vertex_to_neigh_list(vertex v, vertex vneigh) {
    if (v != NULL) {
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
}

void make_vertex_neighs(vertex v1, vertex v2){
    if (v1 != NULL && v2 != NULL) {
        append_vertex_to_neigh_list(v1,v2);
        append_vertex_to_neigh_list(v2,v1);
    }
}

// Funciones de impresión y debugging

void print_vertex_data(vertex v) {
    if (v != NULL){
        printf("Tag: %"SCNu32" Name: %"SCNu32" Color: %"SCNu32" Grade: %"SCNu32" \n",
                get_vertex_tag(v), get_vertex_name(v), get_vertex_color(v),
                get_vertex_grade(v));
        print_vertex_neighs(v);
    }
}

void print_all_neighs_data(vertex v) {
    if (v != NULL){
        neighb_t nglist = get_vertex_neigh_list(v);
        printf("Vertex %"SCNu32 " neighbours data:\n", v->name);
        while (nglist != NULL) {
            print_vertex_data(nglist->vertex_pt);
            nglist = nglist->next;
        }
    }
}

void print_vertex_neighs(vertex v) {
    if (v != NULL){
        neighb_t auxlist = v->neighList;
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
}