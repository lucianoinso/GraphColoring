/* "Copyright 2016 <Rodriguez - Mietzsch>" */
#include "include/translate.h"

bool is_cap(char *word) {
    return (word[0] >= 65 && word[0] <= 90);
}

char *capitalize_string(char *word) {
    if (word == NULL || strlen(word) == 0 || is_cap(word)) {
        return word;
    }
    word[0] = toupper(word[0]);
    return word;
}

char *decapitalize_string(char *word) {
    if (word == NULL || strlen(word) == 0 || !is_cap(word)) {
        return word;
    }
    word[0] = tolower(word[0]);
    return word;
}

char *trunc_from_to(char *word, int start, int end) {
    word += start;
    word[end - start]  = '\0';
    return word;
}

void remove_all_chars(char *s, const char *toremove) {
    while ((s=strstr(s, toremove)))
        memmove(s, s + strlen(toremove), 1 + strlen(s + strlen(toremove)));
}

char *translate_word(char *w, dict dic, dict user_dict, dict temp_dict,
                     dict ign_dic, bool reversed) {
    int size = strlen(w);
    char strt_spc_chars[MAX_LINE_LENGTH] = "";
    char end_spc_chars[MAX_LINE_LENGTH] = "";
    char *st_sp_chrs = strt_spc_chars;
    char *end_sp_chrs = end_spc_chars;
    char buffer[MAX_LINE_LENGTH];
    int i = 0;

    int strt_sp_cnt = 0;  // start special chars count
    int end_sp_cnt = 0;  // end special chars count

    while (!isalpha(w[i]) && i < size) {
        strt_sp_cnt++;
        i++;
    }

    i = size - 1;
    while (!isalpha(w[i]) && i >= 0) {
        end_sp_cnt++;
        i--;
    }

    strncpy(st_sp_chrs, &w[0], strt_sp_cnt);
    // st_sp_chrs has the init nonAlpha chars
    strncpy(end_sp_chrs, &w[size - end_sp_cnt], end_sp_cnt);
    // end_sp_chrs has end NonAlpha chars

    char* word_without_spc = trunc_from_to(w, strt_sp_cnt, size - end_sp_cnt);
    // strip the first and last special characters of the string

    bool is_capd = is_cap(word_without_spc);

    char *decaped_word = decapitalize_string(word_without_spc);
    char *trad_word = NULL;


    if (!word_exists(decaped_word, ign_dic)) {
        trad_word = search_word(decaped_word, user_dict);

        if (trad_word == NULL) {  // word not in user dictionary
            trad_word = search_word(decaped_word, dic);  // check original dict
            if (trad_word == NULL) {  // word not in original dictionary
                trad_word = search_word(decaped_word, temp_dict);
                // check temp dict for this run
                if (trad_word == NULL) {  // word not in any dictionary
                    trad_word = call_menu(decaped_word, user_dict, temp_dict,
                                          ign_dic);
                }
            }
        }

        if (reversed) {
            remove_all_chars(st_sp_chrs, "¿");
            remove_all_chars(end_sp_chrs, "¡");
        }

    } else {
        trad_word = calloc(WORD_MAX_LENGTH + 1, sizeof(char));
        strncpy(trad_word, decaped_word, WORD_MAX_LENGTH + 1);
        trad_word[WORD_MAX_LENGTH] = '\0';
    }

    if (is_capd) {
        trad_word = capitalize_string(trad_word);
    }

    memset(buffer, '\0', sizeof(buffer));
    strncpy(buffer, st_sp_chrs, strt_sp_cnt);
    strncat(buffer, trad_word, WORD_MAX_LENGTH);
    strncat(buffer, end_sp_chrs, end_sp_cnt);

    strncpy(trad_word, buffer, WORD_MAX_LENGTH);
    return trad_word;
}


char *call_menu(char* word, dict user_dict, dict temp_dict, dict ign_dic) {
    bool valid_option = false;
    char option[4];

    char *new_trans = calloc(WORD_MAX_LENGTH + 1, sizeof(char));
    assert(new_trans != NULL);

    while (!valid_option) {
        printf("No hay traducción para la palabra: %s\n\n", word);
        printf("Ignorar (i) - Ignorar Todas (h) - Traducir como (t) - ");
        printf("Traducir siempre como (s)\n");

        scanf("%s", option);
        if (option[0] == '\0')
            continue;

        switch (option[0]) {
            case 'i':   // ignore word just this time
                strncpy(new_trans, word, WORD_MAX_LENGTH + 1);
                new_trans[WORD_MAX_LENGTH] = '\0';
                valid_option = true;
                break;
            case 'h':
                strncpy(new_trans, word, WORD_MAX_LENGTH + 1);
                new_trans[WORD_MAX_LENGTH] = '\0';
                ign_dic = dict_append(ign_dic, word, word);
                // always ignore, add to dictign
                valid_option = true;
                break;
            case 't':
                printf("Traducir %s como: ", word);
                scanf("%s", new_trans);
                dict_append(temp_dict, word, new_trans);
                valid_option = true;
                break;

            case 's':
                printf("Traducir %s siempre como: ", word);
                scanf("%s", new_trans);
                user_dict = dict_append(user_dict, word, new_trans);
                valid_option = true;
                break;

            default :
                printf("invalid input: %c\n", option[0]);
                break;
        }
    }
    return new_trans;
}

char *translate_line(char *line, dict dic, dict user_dict, dict temp_dict,
                     dict ign_dic, bool reversed) {
    char buffer[MAX_LINE_LENGTH] = "";
    char* word;
    const char separator[2] = " ";
    /* get the first word from the message, seperated by
     * space character */

    word = strtok(line, separator);

    /* Get the next words from the message, NULL must be
     * used to get tokens from the previous string now
     * the following loop gets the rest of the words until the
     * end of the message */

    do {
        word = translate_word(word, dic, user_dict, temp_dict, ign_dic,
                              reversed);
        strncat(buffer, word, WORD_MAX_LENGTH);
        strncat(buffer, " ", WORD_MAX_LENGTH);
        free(word);
    } while ((word = strtok(NULL, separator)) != NULL);

    buffer[strlen(buffer)-1] = '\0';  // removes the last space originally added
                                      // to separate words
    memset(line, '\0', MAX_LINE_LENGTH);
    strncpy(line, buffer, MAX_LINE_LENGTH);
    return line;
}


void translate_document(char *input_file, char *output_file, dict dic,
                        dict user_dict, dict temp_dict, dict ign_dic,
                        bool reversed) {
    FILE *input_f = fopen(input_file, "r");
    FILE *output_f = fopen(output_file, "w");
    char line[MAX_LINE_LENGTH];
    char *l = line;
    if (input_f == NULL) {
        printf("Error opening input file\n");
        fclose(output_f);
        return;
    }

    if (output_f == NULL) {
        printf("Error opening output file\n");
        fclose(input_f);
        return;
    }

    while (fgets(line, MAX_LINE_LENGTH, input_f) != NULL) {
        l = translate_line(line , dic, user_dict, temp_dict, ign_dic, reversed);
        fprintf(output_f, "%s", l);
    }

    fclose(input_f);
    fclose(output_f);
}

