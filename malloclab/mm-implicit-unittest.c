#include <stdio.h>
#include <assert.h>
#include "memlib.h"
#include "mm-common.h"
#include "mm-implicit.h"

void
test_first_fit()
{
	printf("start %s\n", __func__);
	mem_reset_brk();
	mm_init();

	//make a test heap 
	//first chunk is 256 bytes and allocated
	init_chunk((header_t *)mem_sbrk(256), 256, true); 
	//second chunk is 256 bytes and free
	header_t *fh = (header_t *)mem_sbrk(256);
	init_chunk(fh, 256, false);
	//third chunk is 256 bytes and allocated
	init_chunk((header_t *)mem_sbrk(256), 256, true); 
	
	header_t *h;
        h = first_fit(64);
	assert(h == fh);
	return;
}

void
test_mm_checkheap()
{
	printf("start %s\n", __func__);
	mem_reset_brk();
	mm_init();

	//make a test heap 
	//first chunk is 128 bytes and allocated
	init_chunk((header_t *)mem_sbrk(128), 128, true); 
	//second chunk is 256 bytes and free
	init_chunk((header_t *)mem_sbrk(256), 256, false);
	//third chunk is 512 bytes and allocated
	init_chunk((header_t *)mem_sbrk(512), 512, true); 
	//4th chunk is 1024 bytes and free
	init_chunk((header_t *)mem_sbrk(1024), 1024, false); 

	heap_info_t info;
	info = mm_checkheap(false);	
	assert(info.num_allocated_chunks == 2);
	assert(info.num_free_chunks == 2);
	assert(info.allocated_size == 640);
	assert(info.free_size == 1280);
}


void
test_split()
{
	printf("start %s\n", __func__);
	mem_reset_brk();
	mm_init();

	//allocate a 256-byte chunk
	header_t *p;
	p = (header_t *)mem_sbrk(256);
	p->size = 256;
	p->allocated = false;

	//split it
	split(p, 64);

	assert(p->size == 64);
	p->allocated = true;

	
	header_t *np;
	np = (header_t *)((char *)p + p->size);
	assert(np->size == 192);

	heap_info_t info = mm_checkheap(false);
	assert(info.num_allocated_chunks == 1);
	assert(info.num_free_chunks == 1);
	assert(info.allocated_size == 64);
	assert(info.free_size == 192);

}

void 
test_ask_os_for_chunk()
{
	printf("start %s\n", __func__);
	mem_reset_brk();
	mm_init();

	header_t *p;
	p = ask_os_for_chunk(256);
	assert(p->size == 256);
	assert(p->allocated == false);
	assert(mem_heapsize() == 256);
}



void
test_mm_malloc()
{
	printf("start %s\n", __func__);
	mem_reset_brk();
	mm_init();

	//make a test heap 
	//first chunk is 256 bytes and allocated
	init_chunk((header_t *)mem_sbrk(256), 256, true); 
	//second chunk is 256 bytes and free
	header_t *fp;
	fp = (header_t *)mem_sbrk(256);
	init_chunk(fp, 256, false);

	//check that allocation is done in the beginning of 
	//second chunk
	char *p = mm_malloc(60);
	assert(p == (char *)fp + hdr_size);

	heap_info_t info = mm_checkheap(false);
	assert(info.num_free_chunks == 1);
	assert(info.num_allocated_chunks == 2);

	//malloc again, check that payload is properly aligned
	p = mm_malloc(60);
	assert(is_aligned(p));

	info = mm_checkheap(false);
	assert(info.num_free_chunks == 1);
	assert(info.num_allocated_chunks == 3);
	assert((info.free_size + info.allocated_size) == 512);
}

void
test_payload2header()
{
	printf("start %s\n", __func__);
	mem_reset_brk();
	mm_init();

	header_t *h = (header_t *)mem_sbrk(128);	
	char *p = (char *)h + hdr_size;
	assert(payload2header(p) == h);
}

void
test_next_chunk()
{
	printf("start %s\n", __func__);
	mem_reset_brk();
	mm_init();

	assert(next_chunk(NULL) == NULL);

	//make a test heap of 2 chunks
	header_t *p[2];
	for (int i = 0; i < 2; i++) {
		p[i] = (header_t *)mem_sbrk(256);
		init_chunk(p[i], 256, true);
	}

	assert(next_chunk(NULL) == p[0]);
	assert(next_chunk(p[0]) == p[1]);
	assert(next_chunk(p[1]) == NULL);

}

void
test_coalesce()
{
	printf("start %s\n", __func__);
	mem_reset_brk();
	mm_init();

	//make a test heap of 3 free chunks
	header_t *p[3];
	for (int i = 0; i < 3; i++) {
		p[i] = (header_t *)mem_sbrk(256);
		init_chunk(p[i], 256, false);
	}
	coalesce(p[0]);
	heap_info_t info = mm_checkheap(false);
	assert(info.num_free_chunks = 1);
	assert(info.free_size = 256*3);
}


void
test_mm_free()
{
	printf("start %s\n", __func__);
	mem_reset_brk();
	mm_init();

	//make a test heap of 3 allocated chunks
	header_t *h[3];
	for (int i = 0; i < 3; i++) {
		h[i] = (header_t *)mem_sbrk(256);
		init_chunk(h[i], 256, true);
	}
	char *p;
        p = (char *)h[1] + hdr_size;
	mm_free(p);
	heap_info_t info = mm_checkheap(false);
	assert(info.num_free_chunks == 1);
	assert(info.num_allocated_chunks == 2);

        p = (char *)h[2] + hdr_size;
	mm_free(p);
	//no coalesce is happening with the previous chunk
	info = mm_checkheap(false);
	assert(info.num_free_chunks == 2);
	assert(info.num_allocated_chunks == 1);

	p = (char *)h[0] + hdr_size;
	mm_free(p);
	//coalesce merges 3 chunks together
	info = mm_checkheap(false);
	assert(info.num_free_chunks == 1);
	assert(info.num_allocated_chunks == 0);
	assert(info.free_size == 256*3);

}

void
test_mm_realloc()
{
	printf("start %s\n", __func__);
	mem_reset_brk();
	mm_init();

	//make a test heap of 3 chunks 
	//first one is free, second one is allocated
	//3rd one is free.
	header_t *h[3];
	for (int i = 0; i < 3; i++) {
		h[i] = (header_t *)mem_sbrk(256);
		init_chunk(h[i], 256, false);
	}
	//mark 2nd chunk as allocated
	h[1]->allocated = true;

	heap_info_t info = mm_checkheap(false);
	assert(info.num_free_chunks == 2);
	assert(info.num_allocated_chunks == 1);

	char *p = mm_realloc((char *)h[1] + hdr_size, 512-hdr_size);
	assert(p == (char *)h[1] + hdr_size);
	info = mm_checkheap(false);
	assert(info.num_free_chunks == 1);
	assert(info.num_allocated_chunks == 1);
	assert(info.allocated_size == 512);
}

int
main(int argc, char **argv)
{
	mem_init();
	test_next_chunk();
	test_mm_checkheap(false);

       	test_ask_os_for_chunk();
	test_split();
	test_first_fit();
	test_mm_malloc();

	test_payload2header();
	test_coalesce();
	test_mm_free();

	test_mm_realloc();
	printf("all unit tests passed...\n");

}
