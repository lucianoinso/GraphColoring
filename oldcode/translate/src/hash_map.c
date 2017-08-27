/* "Copyright 2016 <Rodriguez - Mietzsch>" */
#include "include/hash_map.h"

struct st_hash_node {
    word_t key;
    word_t value;
    hash_node next;
};

struct st_hash_map {
    hash_node *hash_array;  // array of vertices we are storing
    unsigned int n_buckets;
};

hash_map hash_map_create(uint n_buckets) {
    hash_map h_map = calloc(1, sizeof(struct st_hash_map));
    h_map->n_buckets = n_buckets;
    h_map->hash_array = calloc(n_buckets, sizeof(struct st_hash_node));

    return h_map;
}

char *search_key(word_t key, hash_map h_map) {
    uint hn = get_hash(key, h_map->n_buckets);

    hash_node aux;
    aux = h_map->hash_array[hn];

    char *value = NULL;
    bool found = false;

    while (aux != NULL && !found) {
        if (word_is_equal(key, aux->key)) {
            value = word_to_string(aux->value);
            found = true;
        } else {
            aux = aux->next;
        }
    }
    return value;
}


hash_node get_hash_node(hash_map h_map, uint hash_num) {
    return h_map->hash_array[hash_num];
}

hash_node hash_node_create(char *key, char *value) {
    hash_node h_node = calloc(1, sizeof(struct st_hash_node));

    h_node->key = word_from_string(key);
    h_node->value = word_from_string(value);
    h_node->next = NULL;

    return h_node;
}


uint get_hash(word_t word, uint n_buckets) {
    uint hash_result = 0;
    unsigned int i;
    char *aux_word = word_to_string(word);
    for (i = 0; i < strlen(aux_word); i++) {
        hash_result = ((hash_result * 31) + aux_word[i]) % n_buckets;
    }
    free(aux_word);
    return hash_result;
}


word_t get_node_key(hash_node h_node) {
    return h_node->key;
}


word_t get_node_value(hash_node h_node) {
    return h_node->value;
}


bool key_exists(char *key, hash_map h_map) {
    word_t word = word_from_string(key);
    char *result = search_key(word, h_map);
    bool exists = (result != NULL);
    free(word);
    free(result);
    return exists;
}


hash_node get_node_from_key(char *w, hash_map h_map) {
    word_t key = word_from_string(w);
    uint hn = get_hash(key, h_map->n_buckets);

    hash_node aux;
    aux = h_map->hash_array[hn];

    bool found = false;

    while (aux != NULL && !found) {
        if (word_is_equal(key, aux->key)) {
            found = true;
        } else {
            aux = aux->next;
        }
    }

    free(key);
    return aux;
}


/* Searches for a node with the given key and it modifies it's value with the 
   given value */
int modify_node_value_from_key(char *key, char *value, hash_map h_map) {
    hash_node h_node = get_node_from_key(key, h_map);
    if (h_node == NULL) {
        return -1;
    } else {
        free(h_node->value);
        h_node->value = word_from_string(value);
        return 0;
    }
}

/* add_entry_to_hash_map returns 1 if it adds a new entry, or 0 if it just
 * modifies an existing entry */
int add_entry_to_hash_map(char *key, char *value, hash_map h_map) {
    int result = 0;
    if (!key_exists(key, h_map)) {
        hash_node h_node = hash_node_create(key, value);
        append_node_to_hash_map(h_node, h_map);
        result = 1;
    } else {
        modify_node_value_from_key(key, value, h_map);
    }
    return result;
}


void append_node_to_hash_map(hash_node h_node, hash_map h_map) {
    uint hash_num = get_hash(h_node->key, h_map->n_buckets);

    if (h_map->hash_array[hash_num] == NULL) {
        h_map->hash_array[hash_num] = h_node;
    } else {
        hash_node aux = h_map->hash_array[hash_num];
        while (aux->next != NULL) {
            aux = aux->next;
        }
        aux->next = h_node;
    }
    char *value = word_to_string(get_node_value(h_node));

    free(value);
}


hash_node destroy_hash_node(hash_node h_node) {
    assert(h_node != NULL);
    free(h_node->key);
    free(h_node->value);
    free(h_node);
    h_node = NULL;
    return h_node;
}


hash_map destroy_hash_map(hash_map h_map) {
    unsigned int i;
    hash_node h_node;
    hash_node aux;

    for (i = 0; i < h_map->n_buckets; i++) {
        h_node = get_hash_node(h_map, i);

        while (h_node != NULL) {
            aux = h_node;
            h_node = h_node->next;
            aux = destroy_hash_node(aux);
        }
    }
    free(h_map->hash_array);
    free(h_map);
    return h_map;
}


void hash_dump(hash_map h_map, FILE *f, bool reversed) {
    char *key;
    char *value;

    for (uint i = 0; i < h_map->n_buckets; i++) {
        hash_node aux_node = h_map->hash_array[i];

        while (aux_node != NULL) {
            key = word_to_string(get_node_key(aux_node));
            value = word_to_string(get_node_value(aux_node));
            if (!reversed) {
                fprintf(f, "%s,%s\n", key, value);
            } else {
                fprintf(f, "%s,%s\n", value, key);
            }
            free(key);
            free(value);
            aux_node = aux_node->next;
        }
    }
}

void print_list(hash_map h_map, uint pos) {
    hash_node aux = h_map->hash_array[pos];
    char *key;
    char *value;

    while (aux != NULL) {
        key = word_to_string(aux->key);
        value = word_to_string(aux->value);
        printf("key: %s - value: %s\n", key, value);
        free(key);
        free(value);
        aux = aux->next;
    }
}


void dump_array(hash_map h_map) {
    for (uint i = 0 ; i < h_map->n_buckets; i++) {
        printf("position: %d, address: %p\n", i, (void *)h_map->hash_array[i]);
    }
}

