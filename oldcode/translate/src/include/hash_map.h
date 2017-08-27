/* "Copyright 2016 <Rodriguez - Mietzch>" */
#ifndef LAB1_SRC_INCLUDE_HASH_MAP_H_
#define LAB1_SRC_INCLUDE_HASH_MAP_H_
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "./word.h"

#define DEFAULT_N_BUCKETS 512

typedef struct st_hash_node *hash_node;
typedef struct st_hash_map *hash_map;

typedef unsigned int uint;

hash_map hash_map_create(uint size);
hash_node hash_node_create(char *key, char *value);
uint get_hash(word_t word, uint n_buckets);
int get_node_hash(hash_node h_node);
word_t get_node_key(hash_node h_node);
word_t get_node_value(hash_node h_node);
bool key_exists(char *key, hash_map h_map);
void append_node_to_hash_map(hash_node h_node, hash_map h_map);
hash_node get_hash_node(hash_map h_map, uint hash_num);
void hash_dump(hash_map h_map, FILE *f, bool reversed);
char *search_key(word_t key, hash_map h_map);
hash_node destroy_hash_node(hash_node h_node);
int add_entry_to_hash_map(char *key, char *value, hash_map h_map);
hash_map destroy_hash_map(hash_map h_map);
void dump_array(hash_map h_map);
void print_list(hash_map h_map, uint pos);

#endif  // LAB1_SRC_INCLUDE_HASH_MAP_H_
