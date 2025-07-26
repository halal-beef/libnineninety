#include <libnineninety/simplefb/simplefb.h>
#include <libnineninety/simplefb/font.h>

#include <libnineninety/memory_operations.h>
#include <libnineninety/exynos9830/sfr.h>
#include <libnineninety/libc/string.h>
#include <libnineninety/debug/debug.h>

#include <stdbool.h>

static simplefb_params_t framebuffer_params;
static int line_count = 1;
static int textX = 0;
static bool fb_inited = false;

void fill_fb(color_t color)
{
	if(!fb_inited) return;

	for (int i = 0; i < (framebuffer_params.x_res * framebuffer_params.y_res * framebuffer_params.bpp); i += framebuffer_params.bpp)
	{
		*(color_t *)(framebuffer_params.address + i) = color;
	}
}

static void draw_pixel(int x, int y, color_t color)
{
	uint64_t location = (x * framebuffer_params.bpp) + (y * framebuffer_params.x_res * framebuffer_params.bpp);

	*(color_t *)(framebuffer_params.address + location) = color;
}

void __raw_print_fb(volatile const char *text, color_t font_color, color_t background_color)
{
	int max_lines = ((framebuffer_params.y_res) / (FONTH * SCALE_FACTOR));
	int max_x = framebuffer_params.x_res - (FONTW * SCALE_FACTOR);
	int len = strlen((char *)text);
	bool textX_set = false;

	for (int i = 0; i < len; i++)
	{
		if (text[i] < 32 && text[i] != '\n')
			continue;

		if (text[i] == '\n')
		{
			textX = 0;
			textX_set = true;
			line_count++;
			continue;
		}

		if (textX >= max_x)
		{
			textX = 0;
			line_count++;
		}

		if (line_count >= max_lines)
		{
			fill_fb((color_t){0, 0, 0, 0});
			line_count = 1;
			textX = 0;
			textX_set = true;
		}

		int textY = (100 + (line_count * FONTH * SCALE_FACTOR));

		int index = font_index(text[i]);
		uint8_t *img = letters[index];

		for (int y = 0; y < FONTH; y++)
		{
			unsigned char b = img[y];

			for (int x = 0; x < FONTW; x++)
			{
				if (((b << x) & 0b10000000) > 0)
				{
					for (int dy = 0; dy < SCALE_FACTOR; dy++)
					{
						for (int dx = 0; dx < SCALE_FACTOR; dx++)
						{
							draw_pixel(textX + i * FONTW * SCALE_FACTOR + x * SCALE_FACTOR + dx, textY + y * SCALE_FACTOR + dy, font_color);
						}
					}
				}
				else
				{
					for (int dy = 0; dy < SCALE_FACTOR; dy++)
					{
						for (int dx = 0; dx < SCALE_FACTOR; dx++)
						{
							draw_pixel(textX + i * FONTW * SCALE_FACTOR + x * SCALE_FACTOR + dx, textY + y * SCALE_FACTOR + dy, background_color);
						}
					}
				}
			}
		}
	}

	if(!textX_set)
		textX += len * FONTW * SCALE_FACTOR;
}

void init_simplefb(void)
{
	framebuffer_params.address = FRAMEBUFFER_BASE;
	framebuffer_params.x_res = (readl(DECON_F_BASE + 0x0200)) & 0x3FFF;
	framebuffer_params.y_res = (readl(DECON_F_BASE + 0x0200) >> 16) & 0x3FFF;
	framebuffer_params.bpp = 4;

	writel(0x1281, DECON_F_BASE + 0x0070);

	fb_inited = true;

	fill_fb((color_t){0, 0, 0, 0});

	printf("Framebuffer at 0x%08X initialised with params %dx%dx%d\n", framebuffer_params.address, framebuffer_params.x_res, framebuffer_params.y_res, framebuffer_params.bpp);
}
