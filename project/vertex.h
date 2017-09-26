#ifndef _VERTEXH_
#define _VERTEXH_

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

typedef uint32_t u32;
typedef struct VertexSt *vertex;
typedef struct NeighbSt *neighb_t;

struct NeighbSt {
    vertex vertex_pt;
    neighb_t next;
};

struct VertexSt {
    u32 tag;
    u32 name;
    u32 color;
    u32 grade;
    neighb_t neighList;
    neighb_t lastNeigh;
};

// Funciones de creación y destrucción
vertex create_vertex(u32 tag, u32 name, u32 color, u32 grade);
vertex destroy_vertex(vertex v);

// Funciones getters
u32 get_vertex_tag(vertex v);
u32 get_vertex_color(vertex v);
u32 get_vertex_grade(vertex v);
u32 get_vertex_name(vertex v);
neighb_t get_vertex_neigh_list(vertex v);
neighb_t get_last_neigh(vertex v);

// Funciones de seteo y modificación
void set_vertex_tag(vertex v, u32 t);
void set_vertex_color(vertex v, u32 c);
void set_vertex_grade(vertex v, u32 grade);
void set_vertex_name(vertex v, u32 name);
void modify_vertex_values(u32 tag, u32 name, u32 color, u32 grade, vertex v);
void append_vertex_to_neigh_list(vertex v, vertex vneigh);
void make_vertex_neighs(vertex v1, vertex v2);

// Funciones de impresión y debugging
void print_vertex_data(vertex v);
void print_all_neighs_data(vertex v);
void print_vertex_neighs(vertex v);

#endif