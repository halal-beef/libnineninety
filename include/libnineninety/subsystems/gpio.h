#pragma once

#include <stdbool.h>
#include <stdint.h>

#define CON_MASK(x)	(0xf << ((x) << 2))
#define CON_SFR(x, v)	((v) << ((x) << 2))

#define DAT_MASK(x)	(0x1 << (x))
#define DAT_SET(x)	(0x1 << (x))

#define PULL_MASK(x)	(0xf << ((x) << 2))
#define PULL_MODE(x, v)	((v) << ((x) << 2))

#define DRIVE_MASK(x)	(0xf << ((x) << 2))
#define DRIVE_SET(x, m)	((m) << ((x) << 2))

#define RATE_MASK(x)	(0x1 << (x + 16))
#define RATE_SET(x)	(0x1 << (x + 16))

enum pull_level
{
	NONE = 0,
	DOWN,
	UP
};

enum drive_speed {
	FAST = 0,
	SLOW
};

enum direction {
	INPUT = 0,
	OUTPUT = 1
};

enum levels {
	LOW = 0,
	HIGH
};

typedef struct
{
	uint32_t con;
	uint32_t dat;
	uint32_t pull;
	uint32_t drive;
	uint32_t pdn_con;
	uint32_t pdn_pull;
	uint64_t reserved;
} gpio_bank_t;

void configure_pin(gpio_bank_t *bank, uint32_t offset, int pin, int config);
void set_pin_input(gpio_bank_t *bank, uint32_t offset, int pin);
void set_pin_output(gpio_bank_t *bank, uint32_t offset, int pin, bool enable);
int get_pin_state(gpio_bank_t *bank, uint32_t offset, int pin);
void set_pin_state(gpio_bank_t *bank, uint32_t offset, int pin, bool enable);
void set_pin_pull_mode(gpio_bank_t *bank, uint32_t offset, int pin, int mode);
void set_pin_drive(gpio_bank_t *bank, uint32_t offset, int pin, int drive);
void set_pin_rate(gpio_bank_t *bank, uint32_t offset, int pin, int rate);
