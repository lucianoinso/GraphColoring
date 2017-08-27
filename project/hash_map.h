#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "vertex.h"


typedef struct st_hash_node *hash_node;
typedef struct st_hash_map *hash_map;

typedef unsigned int uint;


hash_map create_hash_map(uint n_buckets);
hash_node create_hash_node(vertex v);
uint get_hash(u32 name, uint n_buckets);
int get_node_hash(hash_node h_node);
u32 get_node_key(hash_node h_node);
vertex get_node_value(hash_node h_node);
bool key_exists(u32 x, hash_map h_map);
void append_node_to_hash_map(hash_node h_node, hash_map h_map);
hash_node get_hash_node(hash_map h_map, uint hash_num);
void dump_hash_map(hash_map h_map, FILE *f);
vertex search_node(u32 name, hash_map h_map);
hash_node destroy_hash_node(hash_node h_node);
vertex add_entry_to_hash_map(u32 tag, u32 name, u32 color, u32 grade, hash_map h_map);
hash_map destroy_hash_map(hash_map h_map);
void dump_array(hash_map h_map);
void print_list(hash_map h_map, uint pos);
int search_and_modify_node_values(u32 tag, u32 name, u32 color, u32 grade, hash_map h_map);