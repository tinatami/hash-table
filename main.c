/* Tina Tami
 * 
 * The programme puts the words from a given file in a hashtable. 
 * The user is able to look up a word, and the programme
 * will return every line number that word occurs in, after
 * stripping it from special characters and converting it
 * to lowercase. */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#include "hash_table.h"
#include "hash_func.h"
#include "array.h"

#define LINE_LENGTH 256

#define TABLE_START_SIZE 65536
#define MAX_LOAD_FACTOR 1.0
#define HASH_FUNCTION hash_djb2

#define START_TESTS 6
#define MAX_TESTS 4
#define HASH_TESTS 3


/* Creates a hash table with word index for the specified file 
 * and parameters. */
struct table *create_from_file(char *filename, unsigned long start_size,
                double max_load, unsigned long (*hash_func)(unsigned char *)) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
        exit(1);

    char *line = malloc((LINE_LENGTH + 1) * sizeof(char));
    struct table *hash_table = table_init(start_size, max_load, hash_func);
    char *split_line;
    int line_number = 1;
    while (fgets(line, LINE_LENGTH, fp)) {
        for (unsigned int i = 0; line[i] != '\0'; i++) {
            if (line[i] >= 'A' && line[i] <= 'Z') {
                line[i] = (char) (line[i] + 32);
            }
            else if (line[i] < 'a' || line[i] > 'z') {
                line[i] = ' ';
            }
        }
        split_line = strtok(line, " ");
        while (split_line != NULL) {
            table_insert(hash_table, split_line, line_number);
            split_line = strtok(NULL, " ");
        }
        line_number++;
    }
    fclose(fp);
    free(line);
    return hash_table;
}

/* Reads words from stdin and prints line lookup results per word. */
void stdin_lookup(struct table *hash_table) {
    char *line = malloc((LINE_LENGTH + 1) * sizeof(char));
    struct array *output;
    unsigned int i;
    while (fgets(line, LINE_LENGTH, stdin)) {
        for (i = 0; line[i] != '\0'; i++) {
            if (line[i] >= 'A' && line[i] <= 'Z') {
                line[i] = (char) (line[i] + 32);
            }
            else if (line[i] < 'a' || line[i] > 'z') {
                break;
            }
        }
        char *final_output = calloc(i + 1, sizeof(char)); 
        strncpy(final_output, line, i);
        output = table_lookup(hash_table, final_output);
        printf("%s\n", final_output);
        for (unsigned long i = 0; i < array_size(output); i++) {
            printf("%s", "* ");
            printf("%d\n", array_get(output, i));
        }
        printf("\n");
        free(final_output);
    }   
    free(line);
}

void timed_construction(char *filename) {
    /* Here you can edit the hash table testing parameters: Starting size,
     * maximum load factor and hash function used, and see the the effect
     * on the time it takes to build the table.
     * You can edit the tested values in the 3 arrays below. If you change
     * the number of elements in the array, change the defined constants
     * at the top of the file too, to change the size of the arrays. */
    unsigned long start_sizes[START_TESTS] = {2, 16, 64, 256, 1024, 65536};
    double max_loads[MAX_TESTS] = {0.2, 0.5, 0.8, 1.0};
    unsigned long (*hash_funcs[HASH_TESTS])(unsigned char *) = {hash_too_simple, 
                                                                hash_djb2, 
                                                                hash_loselose};

    for (int i = 0; i < START_TESTS; i++) {
        for (int j = 0; j < MAX_TESTS; j++) {
            for (int k = 0 ; k < HASH_TESTS; k++) { 
                clock_t start = clock();
                struct table *hash_table = create_from_file(filename,
                        start_sizes[i], max_loads[j], hash_funcs[k]);
                clock_t end = clock();

                printf("Start: %ld\tMax: %.1f\tHash: %d\t -> Time: %ld microsecs\n",
                        start_sizes[i], max_loads[j], k, end - start);
                table_cleanup(hash_table);
            }
        }
    }
}


int main(int argc, char *argv[])
{
    if (argc < 2)
        return 1;
    
    if (argc == 3 && !strcmp(argv[2], "-t")) {
        timed_construction(argv[1]);
    } else {
        struct table *hash_table = create_from_file(argv[1], TABLE_START_SIZE,
                MAX_LOAD_FACTOR, HASH_FUNCTION);  
        
        stdin_lookup(hash_table);
        table_cleanup(hash_table);
    }

    return 0;
}

