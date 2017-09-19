#include "hash_map.h"
#include <time.h>

struct st_hash_node {
    u32 key;
    vertex vnode;
    hash_node next;
};

struct st_hash_map {
    hash_node *hash_array;
    unsigned int n_buckets;
};

hash_map create_hash_map(uint n_buckets) {
    hash_map h_map = calloc(1, sizeof(struct st_hash_map));
    h_map->n_buckets = n_buckets;
    h_map->hash_array = calloc(n_buckets, sizeof(struct st_hash_node));

    return h_map;
}

hash_node create_hash_node(vertex v) {
    hash_node h_node = calloc(1, sizeof(struct st_hash_node));

    h_node->key = get_vertex_name(v);
    h_node->vnode = v;
    h_node->next = NULL;

    return h_node;
}

vertex search_node(u32 name, hash_map h_map) {
    uint hash = get_hash(name, h_map->n_buckets);

    hash_node aux;
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

hash_node get_hash_node(hash_map h_map, uint hash_num) {
    return h_map->hash_array[hash_num];
}

/* D. J. Bernstein hash function */
int djb_hash(const char* cp)
{
    int hash = 5381;
    while (*cp)
        hash = 33 * hash ^ (unsigned char) *cp++;
    return hash;
}

/* Fowler/Noll/Vo (FNV) hash function, variant 1a */
int fnv1a_hash(const char* cp)
{
    int hash = 0x811c9dc5;
    while (*cp) {
        hash ^= (unsigned char) *cp++;
        hash *= 0x01000193;
    }
    return hash;
}

u32 create_hash(unsigned int x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

uint get_hash(u32 name, uint n_buckets) {
//    char *binary_string = (char *)&name;
    u32 hash = create_hash(name);
    return ((hash % n_buckets));
}


u32 get_node_key(hash_node h_node) {
    return h_node->key;
}


vertex get_node_value(hash_node h_node) {
    return h_node->vnode;
}


hash_node get_node(u32 name, hash_map h_map) {
    uint hash = get_hash(name, h_map->n_buckets);

    hash_node aux;
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

/* Searches for a node with the given key and it modifies it's value with the 
   given value */
int search_and_modify_node_values(u32 tag, u32 name, u32 color, u32 grade, hash_map h_map) {
    hash_node h_node = get_node(name, h_map);
    if (h_node == NULL) {
        return -1;
    } else {
        set_vertex_tag(h_node->vnode, tag);
        set_vertex_color(h_node->vnode, color);
        set_vertex_grade(h_node->vnode, grade);
        return 0;
    }
}

/* add_entry_to_hash_map adds and returns the created node */
vertex add_entry_to_hash_map(u32 tag, u32 name, u32 color, u32 grade, hash_map h_map) {
    vertex v = search_node(name, h_map);
    if (v != NULL) {
        modify_vertex_values(tag, name, color, grade, v);
    } else {
        v = create_vertex(tag, name, color, grade);
        hash_node h_node = create_hash_node(v);
        append_node_to_hash_map(h_node, h_map);
    }
    return v;
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
}

hash_node destroy_hash_node(hash_node h_node) {
    if(h_node != NULL) {
        destroy_vertex(h_node->vnode);
        free(h_node);
        h_node = NULL;
    }
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
    h_map = NULL;
    return h_map;
}


void dump_hash_map(hash_map h_map, FILE *f) {
    int listpos;
    for (uint i = 0; i < h_map->n_buckets; i++) {
        hash_node aux_node = h_map->hash_array[i];

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

void print_list(hash_map h_map, uint pos) {
    hash_node aux_node = h_map->hash_array[pos];

    while (aux_node != NULL) {
        print_vertex_data(aux_node->vnode);
        aux_node = aux_node->next;
    }
}

void dump_array(hash_map h_map) {
    for (uint i = 0 ; i < h_map->n_buckets; i++) {
        printf("position: %d, address: %p\n", i, (void *)h_map->hash_array[i]);
    }
}

/*
int main() {
    srand(time(NULL));   // should only be called once
    int hash_map_size = 1024*1024;
    hash_map hm = create_hash_map(hash_map_size);
    int i;
    for (i = 0; i < hash_map_size *2.5; i++)
    {
        add_entry_to_hash_map(i, rand(), rand() % hash_map_size, rand() % hash_map_size, hm);
    }

    add_entry_to_hash_map(i + 1, 1, 0, 3, hm);
    add_entry_to_hash_map(i + 2, 2, 0, 3, hm);
    add_entry_to_hash_map(i + 3, 3, 0, 3, hm);
    add_entry_to_hash_map(i + 4, 4, 0, 2, hm);
    add_entry_to_hash_map(i + 5, 5, 0, 3, hm);

    printf("Done loading\n");
    
    vertex v1 = search_node(1, hm);
    vertex v2 = search_node(2, hm);
    vertex v3 = search_node(3, hm);
    vertex v4 = search_node(4, hm);
    vertex v5 = search_node(5, hm);

    make_vertex_neighbs(v1, v2);
    make_vertex_neighbs(v1, v3);
    make_vertex_neighbs(v1, v5);
    make_vertex_neighbs(v2, v5);
    make_vertex_neighbs(v2, v3);
    make_vertex_neighbs(v3, v4);
    make_vertex_neighbs(v4, v5);

    printf("Vertex 1 neighs:\n");
    printVertexNeighs(v1);
    printf("Vertex 2 neighs:\n");
    printVertexNeighs(v2);
    printf("Vertex 3 neighs:\n");
    printVertexNeighs(v3);
    printf("Vertex 4 neighs:\n");
    printVertexNeighs(v4);
    printf("Vertex 5 neighs:\n");
    printVertexNeighs(v5);
    printf("\n");

    print_vertex_data(v4);
    print_all_neighs_data(v4);

//    dump_hash_map(hm, stdout);
    hm = destroy_hash_map(hm);

    return 0;
}
*/
