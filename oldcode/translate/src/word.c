/* "Copyright 2016 <Rodriguez - Mietzsch>" */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "include/word.h"


struct st_word {
    char content[WORD_MAX_LENGTH + 1];
};

unsigned int word_max_length(void) {
    return (WORD_MAX_LENGTH);
}

unsigned int word_length(word_t word) {
    assert(word != NULL);
    return (strlen(word->content));
}

bool word_is_equal(word_t word, word_t other) {
    assert(word != NULL);
    assert(other != NULL);
    return (strncmp(word->content, other->content, WORD_MAX_LENGTH) == 0);
}

bool word_is_less_than(word_t word, word_t other) {
    assert(word != NULL);
    assert(other != NULL);
    return (strncmp(word->content, other->content, WORD_MAX_LENGTH) < 0);
}

word_t word_from_string(char *s) {
    word_t result = NULL;

    assert(s != NULL);

    result = calloc(1, sizeof(struct st_word));
    assert(result != NULL);

    strncpy(result->content, s, WORD_MAX_LENGTH + 1);
    result->content[WORD_MAX_LENGTH] = '\0';

    return (result);
}

char *word_to_string(word_t word) {
    char *result = NULL;

    assert(word != NULL);

    result = calloc(WORD_MAX_LENGTH + 1, sizeof(char));
    assert(result != NULL);

    strncpy(result, word->content, WORD_MAX_LENGTH + 1);

    return (result);
}

word_t word_copy(word_t word) {
    assert(word != NULL);
    return (word_from_string(word->content));
}

word_t word_destroy(word_t word) {
    free(word);
    word = NULL;
    return (word);
}

