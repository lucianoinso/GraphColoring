/* "Copyright 2016 <Rodriguez - Mietzsch>" */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "include/translate.h"

struct arguments {
    char *input_file;
    char *output_file;
    char *ignored_words_file;
    char *dict_file;
    bool reversed;
    bool required;
};

void print_usage() {
    printf("Usage: main -i FILE [-d FILE] [-g FILE] ");
    printf("[-o FILE] [-r]\n\n");
    printf("-i FILE | --input=FILE\t: Input document. (Required)\n");
    printf("-d FILE | --dictionary=FILE\t: Translation dictionary.\n");
    printf("-g FILE | --ignored=FILE\t: Ignored words dictionary.\n");
    printf("-o FILE | --output=FILE\t: Output document.\n");
    printf("-r      | --reverse\t: Translation direction.\n");
}

struct arguments parser(int argc, char **argv, struct arguments args) {
    int c;

    while (1) {
        static struct option long_options[] = {
            {"input", required_argument,       0, 'i'},
            {"dictionary", required_argument,       0, 'd'},
            {"ignored",  required_argument, 0, 'g'},
            {"output", required_argument, 0, 'o'},
            {"reverse", no_argument, 0, 'r'},
            {0, 0, 0, 0}
        };
        int option_index = 0;

        c = getopt_long(argc, argv, "i:d:g:o:r",
                         long_options, &option_index);

        if (c == -1) {
            break;
        }

        switch (c) {
            case 'i':
                args.input_file = optarg;
                args.required = true;
                break;

            case 'd':
                args.dict_file = optarg;
                break;

            case 'g':
                args.ignored_words_file = optarg;
                break;

            case 'o':
                args.output_file = optarg;
                break;

            case 'r':
                args.reversed = true;
                break;

            case '?':
                args.required = false;
                break;
            default :
            break;
        }
    }
    return args;
}


bool valid_filename(char *filename) {
    FILE *file;

    if ((file = fopen(filename, "r"))) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}


int main(int argc, char **argv) {
    struct arguments args = {
        args.input_file = NULL,
        args.output_file = NULL,
        args.ignored_words_file = NULL,
        args.dict_file = NULL,
        args.reversed = NULL,
        args.required = false
    };

    args = parser(argc, argv, args);

    char *user_dict_filename = "userdict.txt";
    char *default_ign_words_filename = "ign_dic.txt";
    dict dic, ign_dic, user_dict, temp_dict;

    if (args.required && valid_filename(args.input_file)) {
        if (valid_filename(args.dict_file)) {
            dic = dict_from_file(args.dict_file, args.reversed);
        } else {
            dic = dict_create(1);  // compatibility effects
        }
        if (valid_filename(args.ignored_words_file)) {
            ign_dic = dict_from_file(args.ignored_words_file, args.reversed);
        } else {
            ign_dic = dict_create(DEFAULT_N_BUCKETS);
        }
        if (args.output_file == NULL) {
            args.output_file = "output.txt";
        }
        if (valid_filename(user_dict_filename)) {
            user_dict = dict_from_file(user_dict_filename, args.reversed);
        } else {
            user_dict = dict_create(DEFAULT_N_BUCKETS);
        }

        temp_dict = dict_create(DEFAULT_N_BUCKETS);

        translate_document(args.input_file, args.output_file, dic, user_dict,
                           temp_dict, ign_dic, args.reversed);

        dict_to_file(user_dict, user_dict_filename, args.reversed);
        dict_to_file(ign_dic, default_ign_words_filename, args.reversed);
        dict_destroy(user_dict);
        dict_destroy(dic);
        dict_destroy(temp_dict);
        dict_destroy(ign_dic);
    } else {
        print_usage();
    }

    return 0;
}

