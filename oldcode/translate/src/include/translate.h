/* "Copyright 2016 <Rodriguez - Mietzch>" */
#ifndef LAB1_SRC_INCLUDE_TRANSLATE_H_
#define LAB1_SRC_INCLUDE_TRANSLATE_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "./dict.h"

#define MAX_LINE_LENGTH 400

void separate_words();
char *call_menu(char* word, dict user_dict, dict temp_dict, dict ign_dic);
char *translate_word(char *w, dict dic, dict user_dict, dict temp_dict,
                     dict ign_dic, bool reversed);
void translate_document(char *input_file, char *output_file, dict dic,
                        dict user_dict, dict temp_dict, dict ign_dic,
                        bool reversed);
#endif  // LAB1_SRC_INCLUDE_TRANSLATE_H_

