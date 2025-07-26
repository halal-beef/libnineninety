#pragma once

#include <stdint.h>

typedef struct
{
	uint32_t address;
	int x_res;
	int y_res;
	int bpp;
} simplefb_params_t;

typedef struct __attribute__((packed))
{
	uint8_t b;
	uint8_t g;
	uint8_t r;
	uint8_t a;
} color_t;

void fill_fb(color_t color);
void __raw_print_fb(volatile const char *text, color_t font_color, color_t background_color);
void init_simplefb(void);
