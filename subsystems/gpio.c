#include <libnineninety/subsystems/gpio.h>
#include <libnineninety/memory_operations.h>

void configure_pin(gpio_bank_t *bank, uint32_t offset, int pin, int config)
{
	uint32_t value;

	value = readl(REG32(&bank->con + offset));
	value &= ~CON_MASK(pin);
	value |= CON_SFR(pin, config);
}

void set_pin_output(gpio_bank_t *bank, uint32_t offset, int pin, bool enable)
{
	uint32_t value;

	configure_pin(bank, offset, pin, OUTPUT);

	value = readl(REG32(&bank->dat + offset));
	value &= ~DAT_MASK(pin);

	if (enable)
	{
		value |= DAT_SET(pin);
	}

	writel(value, REG32(&bank->dat + offset));
}

int get_pin_state(gpio_bank_t *bank, uint32_t offset, int pin)
{
	uint32_t value;

	value = readl(REG32(&bank->dat + offset));

	return !!(value & DAT_MASK(pin));
}

void set_pin_state(gpio_bank_t *bank, uint32_t offset, int pin, bool enable)
{
	uint32_t value;

	value = readl(REG32(&bank->dat + offset));
	value &= ~DAT_MASK(pin);

	if (enable)
		value |= DAT_SET(pin);

	writel(value, REG32(&bank->dat + offset));
}

void set_pin_pull_mode(gpio_bank_t *bank, uint32_t offset, int pin, int mode)
{
	uint32_t value;

	value = readl(REG32(&bank->pull + offset));
	value &= ~PULL_MASK(pin);

	value |= PULL_MODE(pin, mode);

	writel(value, REG32(&bank->pull + offset));
}

void set_pin_drive(gpio_bank_t *bank, uint32_t offset, int pin, int drive)
{
	uint32_t value;

	value = readl(REG32(&bank->drive + offset));
	value &= ~DRIVE_MASK(pin);
	value |= DRIVE_SET(pin, drive);

	writel(value, REG32(&bank->drive + offset));
}

void set_pin_rate(gpio_bank_t *bank, uint32_t offset, int pin, int rate)
{
	uint32_t value;

	value = readl(REG32(&bank->drive + offset));
	value &= ~RATE_MASK(pin);

	value |= RATE_SET(pin);

	writel(value, REG32(&bank->drive + offset));
}
