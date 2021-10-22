
/* Do not edit this function, as it used in testing too
 * Add you own hash functions with different headers instead. */
unsigned long hash_too_simple(unsigned char *str) {
    return (unsigned long) *str;
}

/* DJB2 hash function.
 * Source: http://www.cse.yorku.ca/~oz/hash.html
 * Author: Dan Bernstein. */
unsigned long hash_djb2(unsigned char *str) {
	unsigned long hash = 5381;
	int c;
	while ((c = *str++)) {
    	hash = ((hash << 5) + hash) + (unsigned long) c; 
	}
    return hash;
} 

/* Lose lose hash function.
 * Source: http://www.cse.yorku.ca/~oz/hash.html
 * Author: Dan Bernstein. */
unsigned long hash_loselose(unsigned char *str) {
	unsigned int hash = 0;
	int c;
	while ((c = *str++)) {
		hash += (unsigned int) c;
	}
	return hash;
} 