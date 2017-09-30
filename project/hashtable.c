#include "hashtable.h"

struct Ht_node_St {
    u32 key;
    vertex vnode;
    ht_node next;
};

struct Hashtable_St {
    ht_node *hash_array;
    u32 n_buckets;
};

// Funciones de creación y destrucción

hashtable create_hashtable(u32 n_buckets) {
    hashtable htable = NULL;
    if (n_buckets > 0) {
        htable = calloc(1, sizeof(struct Hashtable_St));
        if (htable != NULL) {
            htable->n_buckets = n_buckets;
            htable->hash_array = calloc(n_buckets, sizeof(hashtable));

            if (htable->hash_array == NULL) {
                destroy_hashtable(htable);
                htable = NULL;
            }
        }
    }
    return htable;
}

ht_node create_ht_node(vertex v) {
    ht_node h_node = calloc(1, sizeof(struct Ht_node_St));
    if (h_node != NULL) {
        h_node->key = get_vertex_name(v);
        h_node->vnode = v;
        h_node->next = NULL;
    }
    return h_node;
}

hashtable destroy_hashtable(hashtable htable) {
    if (htable != NULL) {
        ht_node h_node;
        ht_node aux;
        for (u32 i = 0; i < htable->n_buckets; i++) {
            h_node = get_node_list_at_pos(htable, i);

            while (h_node != NULL) {
                aux = h_node;
                h_node = h_node->next;
                aux = destroy_ht_node(aux);
            }
        }
        free(htable->hash_array);
        free(htable);
        htable = NULL;
    }
    return htable;
}

ht_node destroy_ht_node(ht_node h_node) {
    if (h_node != NULL) {
        destroy_vertex(h_node->vnode);
        free(h_node);
        h_node = NULL;
    }
    return h_node;
}

// Funciones de busqueda, inserción y modificación

u32 create_hash(u32 x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

u32 get_hash(u32 name, u32 n_buckets) {
    u32 hash = create_hash(name);
    return (hash % n_buckets);
}

vertex search_vertex(u32 name, hashtable htable) {
    u32 hash = get_hash(name, htable->n_buckets);

    ht_node aux;
    aux = htable->hash_array[hash];
    vertex v = NULL;

    bool found = false;

    while (aux != NULL && !found) {
        if (name == aux->key) {
            v = aux->vnode;
            found = true;
        } else {
            aux = aux->next;
        }
    }
    return v;
}

ht_node get_node_list_at_pos(hashtable htable, u32 pos) {
    return htable->hash_array[pos];
}

u32 get_node_key(ht_node h_node) {
    return h_node->key;
}

vertex get_node_value(ht_node h_node) {
    return h_node->vnode;
}

/* 
    Crea un vertice, lo agrega al hashtable y devuelve un puntero al vertice,
    si el vertice ya existe, lo modifica con los valores provistos
*/

vertex add_entry_to_hashtable(u32 tag, u32 name, u32 color, u32 grade,
                              hashtable htable) {
    vertex v = search_vertex(name, htable);
    if (v != NULL) {
        modify_vertex_values(tag, name, color, grade, v);
    } else {
        v = create_vertex(tag, name, color, grade);
        ht_node h_node = create_ht_node(v);
        append_node_to_hashtable(h_node, htable);
    }
    return v;
}


void append_node_to_hashtable(ht_node h_node, hashtable htable) {
    u32 hash_num = get_hash(h_node->key, htable->n_buckets);

    if (htable->hash_array[hash_num] == NULL) {
        htable->hash_array[hash_num] = h_node;
    } else {
        ht_node aux = htable->hash_array[hash_num];
        while (aux->next != NULL) {
            aux = aux->next;
        }
        aux->next = h_node;
    }
}

// Funciones de dump y debugging

void dump_hashtable(hashtable htable, FILE *f) {
    int listpos;
    for (u32 i = 0; i < htable->n_buckets; i++) {
        ht_node aux_node = htable->hash_array[i];

        fprintf(f, "Bucket %d: \n", i);
        fprintf(f, "=========\n");
        listpos = 1;
        while (aux_node != NULL) {
            fprintf(f, "List pos %d = ", listpos);
            print_vertex_data(aux_node->vnode);
            aux_node = aux_node->next;
            listpos++;
        }
    }
}

void print_bucket_elements(hashtable htable, u32 pos) {
    ht_node aux_node = htable->hash_array[pos];
    while (aux_node != NULL) {
        print_vertex_data(aux_node->vnode);
        aux_node = aux_node->next;
    }
}

void print_nodes_per_bucket(hashtable htable) {
    u32 *buckets_sum = calloc(htable->n_buckets, sizeof(u32));
    u32 max = 0;
    ht_node aux = NULL;

    for (u32 i = 0; i < htable->n_buckets; i++) {
        aux = htable->hash_array[i];
        while (aux != NULL) {
            buckets_sum[i]++;
            aux = aux->next;
        }
        if (buckets_sum[i] > max) {
            max = buckets_sum[i];
        }
    }
    u32 *amount_of_times = calloc(max + 1, sizeof(u32));
    for (u32 i = 0; i < htable->n_buckets; i++) {
        amount_of_times[buckets_sum[i]]++;
    }
    for (u32 i = 0; i < max + 1; i++) {
        printf("%"SCNu32" buckets have %"SCNu32 " vertices\n",
               amount_of_times[i], i);
    }
    free(buckets_sum);
    free(amount_of_times);
}
