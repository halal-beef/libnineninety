#pragma once

#include <stdbool.h>

typedef struct
{
	bool initialised;
	int  dram_size;
	int  board_revision;
	char board_codename[8];
} board_info_t;

board_info_t get_board_info(void);
