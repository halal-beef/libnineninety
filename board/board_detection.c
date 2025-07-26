#include <stdint.h>

#include <libnineninety/board/board_detection.h>
#include <libnineninety/debug/debug.h>
#include <libnineninety/exynos9830/sfr.h>
#include <libnineninety/libc/string.h>
#include <libnineninety/libfdt/libfdt.h>
#include <libnineninety/memory_operations.h>

static board_info_t board_info = {0};

board_info_t get_board_info(void)
{
	void *bootloader_set_fdt = (void *)(DT_BASE);
	int ret, len, offset;
	char *model_node;

	if (board_info.initialised)
		return board_info;

	ret = fdt_check_header(bootloader_set_fdt);
	if (ret)
	{
		printf("Invalid FDT header: %s\n", fdt_strerror(ret));
	}

	offset = fdt_path_offset(bootloader_set_fdt, "/");
	if (ret < 0)
	{
		printf("Failed to find root node: %s\n", fdt_strerror(ret));
	}

	model_node = (char *)fdt_getprop(bootloader_set_fdt, offset, "model", &len);
	if (len <= 0)
	{
		printf("Failed to get 'model' property\n");
	}

	char *candidate = strtok(model_node, " ");

	/*
	 * The model node is structured as:
	 *   [0]      [1]     [2]    [3]   [4]
	 * Samsung Codename EUR/KOR OPEN Revision
	 */

	for (int i = 0; candidate != NULL && i < 5; i++)
	{
		switch (i)
		{
			case 1:
				strncpy(board_info.board_codename, candidate, 7);
				board_info.board_codename[7] = '\0';
				break;
			case 4:
				board_info.board_revision = atoi(candidate);
				break;
		}

		candidate = strtok(NULL, " ");
	}

	board_info.dram_size = readq(DRAM_SIZE_INFO) >> 20;

	board_info.initialised = true;

	return board_info;
}
