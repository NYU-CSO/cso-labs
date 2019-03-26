typedef struct {
	size_t size;  //8-byte size of the chunk including header
	size_t allocated; // 8-byte
} header_t; //header_t is 16-byte aligned

extern size_t hdr_size;

//helper functions of mm-implicit.c
void init_chunk(header_t *h, size_t csz, bool allocated);
header_t *next_chunk(header_t *h);
header_t *first_fit(size_t csz);
void split(header_t *original, size_t csz);
header_t *ask_os_for_chunk(size_t csz);
header_t *payload2header(void *p);
void coalesce(header_t *h);
