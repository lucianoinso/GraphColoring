#!/bin/bash
gcc -Wall -Werror -Wextra -std=c99 -O3 -c vertex.c
gcc -Wall -Werror -Wextra -std=c99 -O3 vertex.o hashtable.c
