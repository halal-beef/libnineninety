#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <libnineninety/libc/string.h>

#include "malloc_stuff.h"

static uint8_t heap[HEAP_SIZE];
static block_t *free_blocks = (block_t *)heap;

static void initialise_heap(void)
{
	free_blocks->sz = HEAP_SIZE - BLOCK_SIZE;
	free_blocks->free = true;
	free_blocks->next_block = NULL;
}

void *malloc(size_t sz)
{
	if(free_blocks->sz == 0)
		initialise_heap();

	sz = ALIGN(sz);
	block_t *current_block = free_blocks;

	while(current_block)
	{
		if(current_block->free && current_block->sz >= sz)
		{
			if(current_block->sz >= sz + BLOCK_SIZE + ALIGNMENT)
			{
				block_t *new_block = (block_t *)((uint8_t *)current_block + BLOCK_SIZE + sz);
				new_block->sz = current_block->sz - sz - BLOCK_SIZE;
				new_block->free = true;
				new_block->next_block = current_block->next_block;

				current_block->sz = sz;
				current_block->next_block = new_block;
			}

			current_block->free = false;
			return (uint8_t *)current_block + BLOCK_SIZE;
		}

		current_block = current_block->next_block;
	}
	return NULL; // No mem
}

void free(void *allocated_block)
{
	if(!allocated_block) return;

	block_t *block = (block_t *)((uint8_t *)allocated_block - BLOCK_SIZE);
	block->free = true;

	block_t *current_block = free_blocks;
	while (current_block && current_block->next_block)
	{
		if(current_block->free && current_block->next_block->free)
		{
			current_block->sz += BLOCK_SIZE + current_block->next_block->sz;
			current_block->next_block = current_block->next_block->next_block;
		}
		else
			current_block = current_block->next_block;
	}
}

void* memalign(size_t alignment, size_t sz) {
	if (alignment < sizeof(void*) || (alignment & (alignment - 1)) != 0) {
		return NULL;
	}

	size_t total_sz = sz + alignment - 1 + sizeof(void*);
	void* raw = malloc(total_sz);
	if (!raw) return NULL;

	uintptr_t raw_addr = (uintptr_t)raw + sizeof(void*);
	uintptr_t aligned_addr = (raw_addr + alignment - 1) & ~(alignment - 1);

	((void**)aligned_addr)[-1] = raw;

	return (void*)aligned_addr;
}

void aligned_free(void* ptr) {
    if (!ptr) return;

    void* raw = ((void**)ptr)[-1];

    free(raw);
}
