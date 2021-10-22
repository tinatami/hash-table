/* Tina Tami
 *
 * This file contains the data structure described
 * in the hash_table.h file. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hash_table.h"
#include "array.h"

struct table {
    // The (simple) array used to index the table
    struct node **array;
    // The function used for computing the hash values in this table
    unsigned long (*hash_func)(unsigned char *);
    // Maximum load factor after which the table array should be resized
    double max_load;
    // Capacity of the array used to index the table
    unsigned long capacity;
    // Current number of elements stored in the table
    unsigned long load;
};

/* Note: This struct should be a *strong* hint to a specific type of hash table
 * You may implement other options, if you can build them in such a way they
 * pass all tests. However, the other options are generally harder to code. */
struct node {
    // The string of characters that is the key for this node
    char *key;
    // A resizing array, containing the all the integer values for this key
    struct array *value;
    // Next pointer
    struct node *next;
};

unsigned long table_index(struct table *t, char *key);
struct node *node_init(char *key, int value);

/* Returns the index corresponding to a key in the hash table. */
unsigned long table_index(struct table *t, char *key) {
    return t -> hash_func((unsigned char *) key) % t -> capacity;
}

/* Initializes a new node for a new key-value tuple. */
struct node *node_init(char *key, int value) {
    struct node *new_node = malloc(sizeof (struct node));
    new_node -> key = malloc(sizeof (char) * strlen(key) + 1);
    new_node -> next = NULL;
    strcpy(new_node -> key, key);
    struct array *new_array = array_init(2);
    array_append(new_array, value);
    new_node -> value = new_array;
    return new_node;
}

/* This inializes a hash table and returns a pointer to it,
 * or NULL on failure. */
struct table *table_init(unsigned long capacity, double max_load,
                            unsigned long (*hash_func)(unsigned char *)) {
    struct table* t = malloc(sizeof (struct table));
    if (t == NULL) {
        return NULL;
    }
    t -> array = calloc(capacity, sizeof (struct node));
    t -> max_load = max_load;
    t -> capacity = capacity;
    t -> load = 0;
    t -> hash_func = hash_func;
    return t;
}

/* Copies and inserts an array of characters as a key into the hash table,
 * together with the value. If the key is
 * already present in the table, the value is appended to the existing array
 * instead. */
int table_insert(struct table *t, char *key, int value) {
    unsigned long index = table_index(t, key);
    struct node *new_key = t -> array[index];
    if (new_key == NULL) {
        struct node *new_node = node_init(key, value);
        t -> array[index] = new_node;
        t -> load++;
        return 0;
    }
    while (new_key != NULL) {
        if (strcmp(new_key -> key, key) == 0) {
            array_append(new_key -> value, value);
            return 0;
        }
        if (new_key -> next == NULL) {
            struct node *new_node = node_init(key, value);
            new_key -> next = new_node;
            t -> load++;
            return 0;
        }
        new_key = new_key -> next;
    }
    return 1;
}

/* Returns the array of all inserted integer values for the specified key. */
struct array *table_lookup(struct table *t, char *key) {
    unsigned long index = table_index(t, key);
    struct node *key_lookup = t -> array[index];
    while (key_lookup != NULL) {
        if (strcmp(key_lookup -> key, key) == 0) {
            return key_lookup -> value;
        }
        key_lookup = key_lookup -> next;
    }
    return NULL; 
}

/* Removes the specified key and associated value from the hash table. */
int table_delete(struct table *t, char *key) {
    unsigned long index = table_index(t, key);
    struct node *to_delete = t -> array[index];
    struct node* previous_node = NULL;
    while (to_delete != NULL) {
        if (strcmp(to_delete -> key, key) == 0) {
            if (previous_node == NULL) {
                t -> array[index] = to_delete -> next;
            }
            else {
                previous_node -> next = to_delete -> next;
            }
            array_cleanup(to_delete -> value);
            free(to_delete -> key);
            free(to_delete);
            t -> load--;
            return 0;
        }
        previous_node = to_delete;
        to_delete = to_delete -> next;
    }
    return 1;
}

/* Cleans the hash table data structure. */
void table_cleanup(struct table *t) {
    unsigned int i = 0;
    struct node* n1;
    struct node* n2;
    while (i < t -> capacity) {
        n1 = t -> array[i];
        while(n1 != NULL) {
            n2 = n1;
            n1 = n1 -> next;
            array_cleanup(n2 -> value);
            free(n2 -> key);
            free(n2);
        }
        i++;
    }
    free(t -> array);
    free(t);
}
