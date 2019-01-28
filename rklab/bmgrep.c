#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "bmgrep.h"

/* allocate, initialize and return the delta1 array according to the BM paper. 
 * The delta1 array should have 128 integer elements, one for each valid ASCII character.
 * delta1[c] is set the distance from the right where c is found in the pattern.
 * if c is not found in the pattern, x = strlen(pattern).
 */
int*
init_delta1(const char *pattern) {
	// Your code here. Do not return NULL
	return NULL; 
}

/* Let plen = strlen(pattern).
 * Return the largest position where
 * the terminal substring pattern[j+1..plen] re-occurs and is not 
 * preceded by the character pattern[j].
 *
 * The BM paper describes how to calculate this value, aka rpr(j),  
 * in Sec 4 (the last two paragraphs).
 * Note that in your implementation, positions start from 0. 
 * By contrast, in the BM paper, positions start from 1.
 */
int find_rpr(const char *pattern, int j) {
	// Your code here. Do not return strlen(pattern), which is never a valid return value.
	return strlen(pattern);
}

/* Allocate, initialize and return the delta2 array according to the BM paper (page 765).
 * The delta2 array should contain strlen(pattern) integer elements, 
 * one corresponding to each position in the pattern. 
 * Let plen = strlen(pattern).
 * delta2[j] = plen - find_rpr(pattern,j) 
 * Note that the above formula uses "plen" instead of "patlen+1" as in the BM paper.
 * This is because string positions range from 1 to patlen in the BM paper,
 * however, in C, string positions range from 0 to strlen(pattern)-1.
 * Also note that delta2[plen-1] should always be 1.
 */
int*
init_delta2(const char *pattern) {
	// Your code here. Do not return NULL
	return NULL;
}


/* Return the position of the first occurance of the pattern in doc using Boyer-Moore algorithm.
 * If the pattern is not found, return -1.
 * You need to use helper functions (init_delta1, init_delta2) and follow the algorithm in Sec 4.
 * Note: do not use any goto statements in your code!
 *
 * Additionally, return the number of alignments you've done during the matching process by writing to
 * the deferenced pointer n_aligns.
 * The number of alignments is the number of times you've advanced the "i" pointer to the right, 
 * including its initialization.
 * For example, the number of alignments done for the example on page 764 is 5.
 */
int 
boyer_moore(const char *pattern, const char *doc, int *n_aligns) {
	return -1;
}

