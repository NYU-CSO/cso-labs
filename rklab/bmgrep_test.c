/* NYU Computer Systems Organization Lab 2 (Bonus part)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "bmgrep.h"

void
basic_test()
{
	
	{
		char *p =   "abcxxxabc";
		int pn = strlen(p);
		int *d1 = init_delta1(p);
		printf("d1[a] %d\n", d1['a']);
		assert(d1['a'] == 2);
		assert(d1['b'] == 1);
		assert(d1['c'] == 0);
		assert(d1['x'] == 3);
		for (unsigned char c = 1; c < 1<<7; c++) {
			if (c != 'a' && c != 'b' && c!= 'c' && c != 'x') 
				assert(d1[c] == pn);
		}
	}

	{
		char *p =   "abcdefghi";
		int pn = strlen(p);
		int *d1 = init_delta1(p);
		for (unsigned char c = 1; c < 1<<7; c++) {
			if (c >= 'a' && c <= 'i') {
				int correct = pn - 1 - (c-'a');
				assert(d1[c] == correct);
			} else {
			       	assert(d1[c] == pn);
			}
		}
	}

	{
		char *p =   "abcxxxabc";
		int pn = strlen(p);
		int *d2 = init_delta2(p);
		int d2_paper[9] = {14, 13, 12, 11, 10, 9, 11, 10, 1};
		for (int j = 0; j < pn; j++) {
			assert(d2_paper[j] == d2[j]);
		}
	}

	{
		char *p =   "abyxcdeyx";
		int *d2 = init_delta2(p);
		int pn = strlen(p);
		int d2_paper[9] = {17, 16, 15, 14, 13, 12, 7, 10, 1};
		for (int j = 0; j < pn; j++) {
			assert(d2_paper[j] == d2[j]);
		}
	}

	{
		char *p =   "abcdefghi";
		int pn = strlen(p);
		int *d2 = init_delta2(p);
		int d2_correct[9] = {17, 16, 15, 14, 13, 12, 11, 10, 1};
		for (int j = 0; j < pn; j++) {
			assert(d2_correct[j] == d2[j]);
		
		}
	}

	{
		char *p =   "babb";
		int pn = strlen(p);
		int *d2 = init_delta2(p);
		int d2_correct[4] = {6, 5, 2, 1};
		for (int j = 0; j < pn; j++) {
			assert(d2_correct[j] == d2[j]);
		
		}
	}

	{
		char *p = "at-that";
		char *doc = "which-finally-halts.--at-that-point";
		int pos_book = 22, pos;
		int n_aligns_book = 5, n_aligns;
	       	pos = boyer_moore(p, doc, &n_aligns);
		assert(pos == pos_book);
		assert(n_aligns == n_aligns_book);

	}

	{
		char *p =   "abcdef";
		char *doc = "ghijklmnopqrstuvwzyz1234";
		int pos_correct= -1, pos;
		int n_aligns_correct= 4, n_aligns;
	       	pos = boyer_moore(p, doc, &n_aligns);
		assert(pos == pos_correct);
		assert(n_aligns == n_aligns_correct);

	}

	{
		char *p =   "abcxxxabc";
		char *doc = "abcabcxxxxxxabcabcxxxabc";
		int pos_correct= 15, pos;
		int n_aligns_correct= 5, n_aligns;
	       	pos = boyer_moore(p, doc, &n_aligns);
		assert(pos == pos_correct);
		assert(n_aligns == n_aligns_correct);

	}


}

char *
random_string(int nchar, int n)
{
	char *s = malloc(sizeof(char)*(n+1));
	for (int i = 0; i < n; i++) {
		s[i] = 'a' + (rand() % nchar);
	}
	s[n] = '\0';
	return s;
}

void
random_test()
{
	int pn = 4, dn = 100;
	char *pattern = random_string(pn/2, pn);
	char *doc = random_string(pn, dn);
	char *found = strstr(doc, pattern);
	int n_align;
	int pos = boyer_moore(pattern, doc, &n_align);
	if (found == NULL) {
		assert(pos == -1);
		char *newdoc = malloc(sizeof(char)*(pn+dn+1));
		int matching_loc = rand() % dn - pn;
		if (matching_loc < 0) {
			matching_loc = 0;
		}
		memcpy(newdoc, doc, matching_loc);
		memcpy(newdoc+matching_loc, pattern, pn);
		memcpy(newdoc+matching_loc+pn, doc+matching_loc, dn-matching_loc+1);
		assert(newdoc[pn+dn] == '\0');
		found = strstr(newdoc, pattern);
		assert(found);
		int pos = boyer_moore(pattern, newdoc, &n_align);
		assert(pos == (found-newdoc));
		free(newdoc);
	} else {
		assert(pos == (found-doc));
	}
	free(pattern);
	free(doc);
}

int
main(int argc, char **argv)
{

	basic_test();
	printf("-- basic_test: OK --\n");
	for (int i = 0; i < 1000; i++) {
	       	random_test();
	}
	printf("-- random_test: OK --\n");

	printf("PASSED\n");
}

