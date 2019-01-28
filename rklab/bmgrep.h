#ifndef __BMGREP_H_
#define __BMGREP_H_
int *init_delta1(const char *pattern);
int find_rpr(const char *pattern, int j);
int *init_delta2(const char *pattern);
int boyer_moore(const char *pattern, const char *doc, int *n_aligns);

#endif

