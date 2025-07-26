#pragma once

#define ALIGN(x) (((x) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))
#define BLOCK_SIZE ALIGN(sizeof(block_t))

#define HEAP_SIZE 2 * 1024 * 1024	// 2MiB heap, should be enough for a good bit of stuff
#define ALIGNMENT 8			// 8-byte alignment

typedef struct block
{
	size_t sz;
	bool free;
	struct block *next_block;
} block_t;
