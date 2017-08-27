/* "Copyright 2016 <Rodriguez - Mietzch>" */
#ifndef LAB1_SRC_INCLUDE_WORD_H_
#define LAB1_SRC_INCLUDE_WORD_H_
#include <stdbool.h>

#define WORD_MAX_LENGTH 50

typedef struct st_word *word_t;

word_t word_from_string(char *source);
word_t word_destroy(word_t word);
unsigned int word_max_length(void);
unsigned int word_length(word_t word);
char *word_to_string(word_t word);
bool word_is_equal(word_t word, word_t other);
bool word_is_less_than(word_t word, word_t other);
word_t word_copy(word_t word);
#endif  // LAB1_SRC_INCLUDE_WORD_H_

