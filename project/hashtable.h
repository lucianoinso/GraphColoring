#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "vertex.h"

typedef struct Ht_node_St *ht_node;
typedef struct Hashtable_St *hashtable;

hashtable create_hashtable(u32 n_buckets);
ht_node create_ht_node(vertex v);
u32 get_hash(u32 name, u32 n_buckets);
int get_node_hash(ht_node h_node);
u32 get_node_key(ht_node h_node);
vertex get_node_value(ht_node h_node);
bool key_exists(u32 x, hashtable h_map);
void append_node_to_hashtable(ht_node h_node, hashtable h_map);
ht_node get_ht_node(hashtable h_map, u32 hash_num);
void dump_hashtable(hashtable h_map, FILE *f);
vertex search_vertex(u32 name, hashtable h_map);
ht_node destroy_ht_node(ht_node h_node);
vertex add_entry_to_hashtable(u32 tag, u32 name, u32 color, u32 grade,
                              hashtable h_map);
hashtable destroy_hashtable(hashtable h_map);
void dump_array(hashtable h_map);
void print_list(hashtable h_map, u32 pos);
void print_nodes_per_bucket(hashtable h_map);