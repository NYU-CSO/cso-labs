#ifndef MM_COMMON_H
#define MM_COMMON_H

#include <stdio.h>
#include <stdbool.h>
typedef struct {
	size_t num_allocated_chunks; //number of allocated chunks
	size_t num_free_chunks; // number of free chunks
	size_t allocated_size; //sum of sizes of all allocated chunks
	size_t free_size; //sum of sizes of all free chunks
} heap_info_t;

int mm_init (void);
void *mm_malloc (size_t size);
void mm_free (void *ptr);
void *mm_realloc(void *ptr, size_t size);
heap_info_t mm_checkheap(bool verbose);

#endif
