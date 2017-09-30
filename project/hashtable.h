#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "vertex.h"

typedef struct Ht_node_St *ht_node;
typedef struct Hashtable_St *hashtable;

// Funciones de creación y destrucción

hashtable create_hashtable(u32 n_buckets);
ht_node create_ht_node(vertex v);
hashtable destroy_hashtable(hashtable htable);
ht_node destroy_ht_node(ht_node h_node);

// Funciones de busqueda, inserción y modificación

u32 get_hash(u32 name, u32 n_buckets);
vertex search_vertex(u32 name, hashtable htable);
ht_node get_node_list_at_pos(hashtable htable, u32 pos);
u32 get_node_key(ht_node h_node);
vertex get_node_value(ht_node h_node);
vertex add_entry_to_hashtable(u32 tag, u32 name, u32 color, u32 grade,
                              hashtable htable);
void append_node_to_hashtable(ht_node h_node, hashtable htable);

// Funciones de dump y debugging

void dump_hashtable(hashtable htable, FILE *f);
void print_bucket_elements(hashtable htable, u32 pos);
void print_nodes_per_bucket(hashtable htable);
