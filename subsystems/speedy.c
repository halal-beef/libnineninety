#include <stdint.h>

#include <libnineninety/debug/debug.h>
#include <libnineninety/memory_operations.h>
#include <libnineninety/timer/delay.h>

#include <libnineninety/subsystems/speedy.h>

static void speedy_set_enable(uint32_t SPEEDY_BASE)
{
	uint32_t speedy_ctl = readl((SPEEDY_BASE + SPEEDY_CTRL));

	speedy_ctl |= SPEEDY_ENABLE;
	writel(speedy_ctl, SPEEDY_BASE + SPEEDY_CTRL);
}

static void speedy_fifo_reset(uint32_t SPEEDY_BASE)
{
	uint32_t speedy_fifo_ctl = readl(SPEEDY_BASE + SPEEDY_FIFO_CTRL);

	speedy_fifo_ctl |= SPEEDY_FIFO_RESET;
	writel(speedy_fifo_ctl, SPEEDY_BASE + SPEEDY_FIFO_CTRL);
	/* Delay for SPEEDY fifo reset */
	udelay(10);
}

static void speedy_set_cmd(uint32_t SPEEDY_BASE, int direction, uint16_t address, int random, int burst_length)
{
	uint32_t speedy_fifo_ctl = 0;
	uint32_t speedy_int_en = 0;
	uint32_t speedy_command = 0;

	speedy_fifo_reset(SPEEDY_BASE);
	speedy_fifo_ctl = readl(SPEEDY_BASE + SPEEDY_FIFO_CTRL);

	speedy_command |= SPEEDY_ADDRESS(address);

	switch (random) {
	case ACCESS_RANDOM:
		speedy_command |= SPEEDY_ACCESS_RANDOM;
		speedy_fifo_ctl |= (SPEEDY_RX_TRIGGER_LEVEL(1) |
		                    SPEEDY_TX_TRIGGER_LEVEL(1));
		break;
	case ACCESS_BURST:
		speedy_command |= (SPEEDY_ACCESS_BURST | SPEEDY_BURST_INCR |
		                   SPEEDY_BURST_LENGTH(burst_length - 1));
		speedy_fifo_ctl |= (
			SPEEDY_RX_TRIGGER_LEVEL(burst_length) |
			SPEEDY_TX_TRIGGER_LEVEL(1)
			);
		break;

	default:
		printf("[SPEEDY] Can not support this ACCESS mode\n");
		break;
	}

	/* configure speedy fifio trigger level */
	writel(speedy_fifo_ctl, SPEEDY_BASE + SPEEDY_FIFO_CTRL);

	speedy_int_en |= (SPEEDY_TIMEOUT_CMD_EN | SPEEDY_TIMEOUT_STANDBY_EN |
	                  SPEEDY_TIMEOUT_DATA_EN);

	switch (direction) {
	case DIRECTION_READ:
		speedy_command |= SPEEDY_DIRECTION_READ;
		speedy_int_en |= (SPEEDY_FIFO_RX_ALMOST_FULL_EN |
		                  SPEEDY_RX_FIFO_INT_TRAILER_EN |
		                  SPEEDY_RX_MODEBIT_ERR_EN |
		                  SPEEDY_RX_GLITCH_ERR_EN |
		                  SPEEDY_RX_ENDBIT_ERR_EN |
		                  SPEEDY_REMOTE_RESET_REQ_EN);
		break;

	case DIRECTION_WRITE:
		speedy_command |= SPEEDY_DIRECTION_WRITE;
		speedy_int_en |= (SPEEDY_TRANSFER_DONE_EN |
		                  SPEEDY_FIFO_TX_ALMOST_EMPTY_EN |
		                  SPEEDY_TX_LINE_BUSY_ERR_EN |
		                  SPEEDY_TX_STOPBIT_ERR_EN |
		                  SPEEDY_REMOTE_RESET_REQ_EN);
		break;

	default:
		printf("[SPEEDY] Can not support this DIRECTION mode\n");
		break;
	}

	/* Clear SPEEDY interrupt status */
	writel(0xFFFFFFFF, SPEEDY_BASE + SPEEDY_INT_STATUS);

	/* SPEEDY interrupt enable */
	writel(speedy_int_en, SPEEDY_BASE + SPEEDY_INT_ENABLE);

	/* Configure SPEEDY command */
	writel(speedy_command, SPEEDY_BASE + SPEEDY_CMD);
}

static int speedy_wait_transfer_done(uint32_t SPEEDY_BASE)
{
	uint32_t speedy_int_status;
	unsigned long timeout = 1000;
	int ret = 0;

	while (timeout-- > 0) {
		speedy_int_status = readl(SPEEDY_BASE + SPEEDY_INT_STATUS);

		if (speedy_int_status & SPEEDY_TRANSFER_DONE) {
			ret = 0;
			break;
		}
	}

	if (timeout == 0)
		ret = -1;

	writel(0xFFFFFFFF, SPEEDY_BASE + SPEEDY_INT_STATUS);

	return ret;
}

static void speedy_swreset(uint32_t SPEEDY_BASE)
{
	uint32_t speedy_ctl = readl(SPEEDY_BASE + SPEEDY_CTRL);

	speedy_ctl |= SPEEDY_SW_RST;
	writel(speedy_ctl, SPEEDY_BASE + SPEEDY_CTRL);
	/* Delay for SPEEDY software reset */
	udelay(10);
}

void speedy_init(uint32_t SPEEDY_BASE)
{
	/* Clear SPEEDY interrupt status */
	writel(0xFFFFFFFF, SPEEDY_BASE + SPEEDY_INT_STATUS);

	/* Reset SPEEDY ctrl SFR. It may be used by bootloader */
	speedy_swreset(SPEEDY_BASE);
	speedy_set_enable(SPEEDY_BASE);
}

void speedy_read(uint32_t SPEEDY_BASE, uint8_t slave, uint8_t addr, uint8_t *data)
{
	int result;
	uint16_t address = addr + ((slave & 0xF) << 8);

	speedy_set_cmd(SPEEDY_BASE, DIRECTION_READ, address, ACCESS_RANDOM, 1);

	result = speedy_wait_transfer_done(SPEEDY_BASE);

	if (result == 0)
		*data = (uint8_t)readl(SPEEDY_BASE + SPEEDY_RX_DATA);
	else
		printf("[SPEEDY] timeout at read\n");
}

void speedy_read_burst(uint32_t SPEEDY_BASE, uint8_t slave, uint8_t addr,
                       uint8_t *data, uint8_t count)
{
	int result;
	uint16_t address = addr + ((slave & 0xF) << 8);
	int i;

	speedy_set_cmd(SPEEDY_BASE, DIRECTION_READ, address, ACCESS_BURST, count);

	result = speedy_wait_transfer_done(SPEEDY_BASE);

	if (result == 0)
		for (i = 0; i < count; i++)
			data[i] = (uint8_t)readl(SPEEDY_BASE + SPEEDY_RX_DATA);
	else
		printf("[SPEEDY] timeout at burst_read\n");
}

void speedy_write(uint32_t SPEEDY_BASE, uint8_t slave, uint8_t addr, uint8_t data)
{
	int result;
	uint16_t address = addr + ((slave & 0xF) << 8);

	speedy_set_cmd(SPEEDY_BASE, DIRECTION_WRITE, address, ACCESS_RANDOM, 1);
	writel(data, SPEEDY_BASE + SPEEDY_TX_DATA);

	result = speedy_wait_transfer_done(SPEEDY_BASE);

	if (result == 0)
		return;
	else
		printf("[SPEEDY] timeout at write\n");
}

void speedy_write_burst(uint32_t SPEEDY_BASE, uint8_t slave, uint8_t addr,
                        uint8_t *data, uint8_t count)
{
	int result;
	uint16_t address = addr + ((slave & 0xF) << 8);
	int i;

	speedy_set_cmd(SPEEDY_BASE, DIRECTION_WRITE, address, ACCESS_BURST, count);

	for (i = 0; i < count; i++)
		writel(data[i], SPEEDY_BASE + SPEEDY_TX_DATA);

	result = speedy_wait_transfer_done(SPEEDY_BASE);
	if (result == 0)
		return;
	else
		printf("[SPEEDY] timeout at burst_write\n");
}
