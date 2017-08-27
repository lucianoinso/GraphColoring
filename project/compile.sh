#!/bin/bash
gcc -Wall -Werror -Wextra -std=c99 -O3 -c vertex.c hash_map.c
gcc -Wall -Werror -Wextra -std=c99 -O3 vertex.o hash_map.o JonSnow.c
