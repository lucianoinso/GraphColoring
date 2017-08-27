/* "Copyright 2016 <Rodriguez - Mietzch>" */
#ifndef LAB1_SRC_INCLUDE_DICT_H_
#define LAB1_SRC_INCLUDE_DICT_H_
#include <stdlib.h>
#include <stdbool.h>
#include "./hash_map.h"

#define MAX_LINE_LENGTH 400

typedef struct st_dict *dict;

dict dict_create(uint size);
uint get_dict_size(dict d);
dict dict_append(dict d, char* key, char* value);
void dict_destroy(dict d);
bool dict_is_empty(dict d);
dict dict_from_file(char *filename, bool reversed);
void dict_to_file(dict d, char *filename, bool reversed);
bool word_exists(char *w, dict d);
char *search_word(char *w, dict d);
hash_map get_hash_map_from_dict(dict d);
void dict_dump(dict d, FILE *f, bool reversed);
#endif  // LAB1_SRC_INCLUDE_DICT_H_
