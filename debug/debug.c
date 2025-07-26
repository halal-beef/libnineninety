#include <stdarg.h>

#define NANOPRINTF_IMPLEMENTATION
#define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS	1
#define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS	1
#define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS		0
#define NANOPRINTF_USE_SMALL_FORMAT_SPECIFIERS		1
#define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS		1
#define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS		1
#define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS	1
#define NANOPRINTF_USE_ALT_FORM_FLAG			1
#include <libnineninety/nanoprintf.h>

#include <libnineninety/debug/debug.h>
#include <libnineninety/libc/string.h>
#include <libnineninety/simplefb/simplefb.h>

void fb_putc(int c, void *ctx)
{
	(void)ctx;
	char ch[2] = {(char)c, '\0'};

	__raw_print_fb(ch, (color_t){255, 255, 255, 255}, (color_t){0, 0, 0, 0});
}

int printf(volatile const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	int ret = npf_vpprintf(&fb_putc, NULL, (char *)fmt, args);

	va_end(args);
	return ret;
}
