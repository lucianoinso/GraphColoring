/* "Copyright 2016 <Rodriguez - Mietzsch>" */
#include <string.h>
#include "include/dict.h"

struct st_dict{
    hash_map h_map;
    uint size;
};

dict dict_create(uint size) {
    // if the dictionary is too short we create an array with a
    // default number of buckets
    if (size < 16) {
        size = DEFAULT_N_BUCKETS;
    }

    dict emptydict = calloc(1, sizeof(struct st_dict));
    emptydict->h_map = hash_map_create(size);
    return(emptydict);
}

dict dict_append(dict d, char* key, char* value) {
    int result = 0;
    result = add_entry_to_hash_map(key, value, d->h_map);
    d->size += result;

    return d;
}

void dict_destroy(dict d) {
    destroy_hash_map(d->h_map);
    free(d);
}

uint get_dict_size(dict d) {
    return d->size;
}

bool dict_is_empty(dict d) {
    return(d->size == 0);
}

dict dict_from_file(char *filename, bool reversed) {
    FILE *f = fopen(filename, "r");

    if (f == NULL) {
        printf("Error opening file\n");
        return NULL;
    }

    uint count = 0;

    char line[MAX_LINE_LENGTH];
    char separator[5] = ",";
    char *key;
    char *value;

    while (1) {
        if (fgets(line, MAX_LINE_LENGTH, f) == NULL) break;
        count++;
    }

    rewind(f);

    dict d = dict_create(count);

    while (1) {
        if (fgets(line, MAX_LINE_LENGTH, f) == NULL) break;

        if (!reversed) {
            key = strtok(line, separator);
            value = strtok(NULL, separator);

            if (value[strlen(value)-1] == '\n')
                 value[strlen(value)-1] = '\0';
        } else {
            value = strtok(line, separator);
            key = strtok(NULL, separator);

            if (key[strlen(key)-1] == '\n')
                 key[strlen(key)-1] = '\0';
        }

        d = dict_append(d, key, value);
    }
    fclose(f);
    return d;
}

void dict_to_file(dict d, char *filename, bool reversed) {
    FILE *f = NULL;

    f = fopen(filename, "w");
    if (f != NULL) {
        dict_dump(d, f, reversed);
        fclose(f);
    } else {
        printf("Cannot dump dictionary to filename %s.\n", filename);
    }
}

hash_map get_hash_map_from_dict(dict d) {
    return d->h_map;
}


bool word_exists(char *w, dict d) {
    return key_exists(w, d->h_map);
}


void dict_dump(dict d, FILE *f, bool reversed) {
    hash_dump(d->h_map, f, reversed);
}

char *search_word(char *w, dict d) {
    word_t word = word_from_string(w);
    char *def = search_key(word, d->h_map);
    free(word);
    return def;
}

