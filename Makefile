CROSS ?= aarch64-linux-gnu
CC = $(CROSS)-gcc
CXX = $(CROSS)-g++
LD = $(CROSS)-ld
AS = $(CROSS)-as
STRIP = $(CROSS)-strip

INCLUDES = -I include

CFLAGS = -fno-builtin-printf -Os $(INCLUDES)

LIBOBJ =

LIBOBJ += libc/memcpy.o
LIBOBJ += libc/string.o
LIBOBJ += libc/stdlib.o

LIBOBJ += libfdt/fdt.o \
	  libfdt/fdt_ro.o \
	  libfdt/fdt_wip.o \
	  libfdt/fdt_sw.o \
	  libfdt/fdt_rw.o \
	  libfdt/fdt_strerror.o \
	  libfdt/fdt_empty_tree.o \
	  libfdt/fdt_addresses.o \
	  libfdt/fdt_overlay.o \
	  libfdt/fdt_check.o

LIBOBJ += board/board_detection.o

LIBOBJ += subsystems/adc.o
LIBOBJ += subsystems/gpio.o
LIBOBJ += subsystems/speedy.o
LIBOBJ += subsystems/s2mps_19_22.o
LIBOBJ += subsystems/pmu.o
LIBOBJ += DONOTTOUCH/self_destruct.o

LIBOBJ += timer/delay.o

LIBOBJ += simplefb/simplefb.o

LIBOBJ += debug/debug.o

all: libnineninety.a

clean:
	rm -rf $(LIBOBJ) libnineninety.a

.c.o:
	@echo [$(notdir $<)]
	@$(CC) -c $(CFLAGS)  $< -o $@

libnineninety.a: $(LIBOBJ)
	@ar rc $@ $(LIBOBJ)

install: libnineninety.a
	mkdir -p /usr/lib/
	mkdir -p /usr/include/
	cp -r include/* /usr/include/
	install -m 0664 libnineninety.a /usr/lib/
