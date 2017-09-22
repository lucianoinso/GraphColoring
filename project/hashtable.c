#include "hashtable.h"
#include <time.h>

struct Ht_node_St {
    u32 key;
    vertex vnode;
    ht_node next;
};

struct Hashtable_St {
    ht_node *hash_array;
    u32 n_buckets;
};

hashtable create_hashtable(u32 n_buckets) {
    hashtable h_map = NULL;
    if (n_buckets > 0){
        h_map = calloc(1, sizeof(struct Hashtable_St));
        if (h_map != NULL){
            h_map->n_buckets = n_buckets;
            h_map->hash_array = calloc(n_buckets, sizeof(struct Ht_node_St));
            if (h_map->hash_array == NULL){
                destroy_hashtable(h_map);
                h_map = NULL;
            }
        }
    }
    return h_map;
}

ht_node create_ht_node(vertex v) {
    ht_node h_node = calloc(1, sizeof(struct Ht_node_St));
    if (h_node != NULL){
        h_node->key = get_vertex_name(v);
        h_node->vnode = v;
        h_node->next = NULL;
    }
    return h_node;
}

vertex search_vertex(u32 name, hashtable h_map) {
    u32 hash = get_hash(name, h_map->n_buckets);

    ht_node aux;
    aux = h_map->hash_array[hash];
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

ht_node get_ht_node(hashtable h_map, u32 hash_num) {
    return h_map->hash_array[hash_num];
}

u32 create_hash(u32 x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

u32 get_hash(u32 name, u32 n_buckets) {
    u32 hash = create_hash(name);
    return ((hash % n_buckets));
}

u32 get_node_key(ht_node h_node) {
    return h_node->key;
}

vertex get_node_value(ht_node h_node) {
    return h_node->vnode;
}

ht_node get_node(u32 name, hashtable h_map) {
    u32 hash = get_hash(name, h_map->n_buckets);

    ht_node aux;
    aux = h_map->hash_array[hash];

    bool found = false;

    while (aux != NULL && !found) {
        if (name == aux->key) {
            found = true;
        } else {
            aux = aux->next;
        }
    }
    return aux;
}


/* Crea un vertice, lo agrega al hashtable y devuelve un puntero al vertice,
   si el vertice ya existe, lo modifica con los valores provistos */
vertex add_entry_to_hashtable(u32 tag, u32 name, u32 color, u32 grade,
                              hashtable h_map) {
    vertex v = search_vertex(name, h_map);
    if (v != NULL) {
        modify_vertex_values(tag, name, color, grade, v);
    } else {
        v = create_vertex(tag, name, color, grade);
        ht_node h_node = create_ht_node(v);
        append_node_to_hashtable(h_node, h_map);
    }
    return v;
}

void append_node_to_hashtable(ht_node h_node, hashtable h_map) {
    u32 hash_num = get_hash(h_node->key, h_map->n_buckets);

    if (h_map->hash_array[hash_num] == NULL) {
        h_map->hash_array[hash_num] = h_node;
    } else {
        ht_node aux = h_map->hash_array[hash_num];
        while (aux->next != NULL) {
            aux = aux->next;
        }
        aux->next = h_node;
    }
}

ht_node destroy_ht_node(ht_node h_node) {
    if(h_node != NULL) {
        destroy_vertex(h_node->vnode);
        free(h_node);
        h_node = NULL;
    }
    return h_node;
}

hashtable destroy_hashtable(hashtable h_map) {
    if(h_map != NULL){
        ht_node h_node;
        ht_node aux;
        for (u32 i = 0; i < h_map->n_buckets; i++) {
            h_node = get_ht_node(h_map, i);
    
            while (h_node != NULL) {
                aux = h_node;
                h_node = h_node->next;
                aux = destroy_ht_node(aux);
            }
        }
        free(h_map->hash_array);
        free(h_map);
    }
    h_map = NULL;
    return h_map;
}


void dump_hashtable(hashtable h_map, FILE *f) {
    int listpos;
    for (u32 i = 0; i < h_map->n_buckets; i++) {
        ht_node aux_node = h_map->hash_array[i];

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

void print_list(hashtable h_map, u32 pos) {
    ht_node aux_node = h_map->hash_array[pos];

    while (aux_node != NULL) {
        print_vertex_data(aux_node->vnode);
        aux_node = aux_node->next;
    }
}

void dump_array(hashtable h_map) {
    for (u32 i = 0 ; i < h_map->n_buckets; i++) {
        printf("position: %d, address: %p\n", i, (void *)h_map->hash_array[i]);
    }
}

void print_nodes_per_bucket(hashtable h_map) {
    u32 *buckets_sum = calloc(h_map->n_buckets, sizeof(u32));
    u32 max = 0;

    ht_node aux = NULL;    
    for (u32 i = 0; i < h_map->n_buckets; i++){
        aux = h_map->hash_array[i];
        while(aux != NULL){
            buckets_sum[i]++;
            aux = aux->next;
        }
        if(buckets_sum[i] > max) {
            max = buckets_sum[i];
        }
    }
    u32 *amount_of_times = calloc(max + 1, sizeof(u32));
    for (u32 i = 0; i < h_map->n_buckets; i++){
        amount_of_times[buckets_sum[i]]++;
    }
    for (u32 i = 0; i < max + 1; i++){
        printf("%"SCNu32": %"SCNu32"\n", i, amount_of_times[i]);
    }
    free(buckets_sum);
    free(amount_of_times);
}


/*
int main() {
    srand(time(NULL));   // should only be called once
    int hashtable_size = 1024*1024;
    hashtable hm = create_hashtable(hashtable_size);
    int i;
    for (i = 0; i < hashtable_size *2.5; i++)
    {
        add_entry_to_hashtable(i, rand(), rand() % hashtable_size, rand() % hashtable_size, hm);
    }

    add_entry_to_hashtable(i + 1, 1, 0, 3, hm);
    add_entry_to_hashtable(i + 2, 2, 0, 3, hm);
    add_entry_to_hashtable(i + 3, 3, 0, 3, hm);
    add_entry_to_hashtable(i + 4, 4, 0, 2, hm);
    add_entry_to_hashtable(i + 5, 5, 0, 3, hm);

    printf("Done loading\n");
    
    vertex v1 = search_vertex(1, hm);
    vertex v2 = search_vertex(2, hm);
    vertex v3 = search_vertex(3, hm);
    vertex v4 = search_vertex(4, hm);
    vertex v5 = search_vertex(5, hm);

    make_vertex_neighs(v1, v2);
    make_vertex_neighs(v1, v3);
    make_vertex_neighs(v1, v5);
    make_vertex_neighs(v2, v5);
    make_vertex_neighs(v2, v3);
    make_vertex_neighs(v3, v4);
    make_vertex_neighs(v4, v5);

    printf("Vertex 1 neighs:\n");
    print_vertex_neighs(v1);
    printf("Vertex 2 neighs:\n");
    print_vertex_neighs(v2);
    printf("Vertex 3 neighs:\n");
    print_vertex_neighs(v3);
    printf("Vertex 4 neighs:\n");
    print_vertex_neighs(v4);
    printf("Vertex 5 neighs:\n");
    print_vertex_neighs(v5);
    printf("\n");

    print_vertex_data(v4);
    print_all_neighs_data(v4);

//    dump_hashtable(hm, stdout);
    hm = destroy_hashtable(hm);

    return 0;
}
*/
