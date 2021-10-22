/* Tina Tami
 *
 * This file contains the data structure described
 * in the array.h file. */

#include <stdlib.h>

#include "array.h"

/* Handles array data structure. */
struct array {
    unsigned long size;
    unsigned long capacity;
    int* data;
};

/* Initialises an array and returns a pointer to it. */
struct array* array_init(unsigned long initial_capacity) {
    struct array* a = malloc(sizeof (struct array));
    a -> size = 0;
    a -> capacity = initial_capacity;
    a -> data = malloc((sizeof (int) * initial_capacity));
    if (a == NULL) {
        return NULL;
    }
    return a;
}

/* Cleans array data structure. */
void array_cleanup(struct array* a) {
    free(a -> data);
    free(a);
}

/* Returns the element at the index position in the array, or
 * -1 if the index is not a valid position. */
int array_get(struct array *a, unsigned long index) {
    if (!a) {
        return -1;
    }
    if (index >= a -> size) {
    	return -1;
    }
    return a -> data[index];
}

/* Adds the element at the end of the array. */
int array_append(struct array *a, int elem) {
	if (a == NULL) {
		return 1;
	}
    if (a -> size >= a -> capacity) {
    	a -> capacity *= 2;
    	a -> data = realloc(a -> data, sizeof (int) * a -> capacity);
    }
    a -> data[a -> size++] = elem;
    return 0;
}

/* Returns the number of elements in the array. */
unsigned long array_size(struct array *a) {
    if (a == NULL) {
        return 0;
    }
    return a -> size;
}
