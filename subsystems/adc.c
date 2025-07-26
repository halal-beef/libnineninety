#include <libnineninety/memory_operations.h>
#include <libnineninety/timer/delay.h>
#include <libnineninety/subsystems/adc.h>

static void init_adc(void)
{
	uint32_t con1, con2;

	con1 = ADC_SOFT_RESET;
	writel(con1, ADC_BASE_ADDR + ADC_CON1);

	con1 = ADC_NON_SOFT_RESET;
	writel(con1, ADC_BASE_ADDR + ADC_CON1);

	con2 = ADC_CON2_C_TIME(6);
	writel(con2, ADC_BASE_ADDR + ADC_CON2);

	writel(EN_INT, ADC_BASE_ADDR + ADC_INT_EN);
}

static void deinit_adc(void)
{
	uint32_t con2;

	con2 = readl(ADC_BASE_ADDR + ADC_CON2);
	con2 &= ~ADC_CON2_C_TIME(7);
	writel(con2, ADC_BASE_ADDR + ADC_CON2);

	writel(!EN_INT, ADC_BASE_ADDR + ADC_INT_EN);
}

static void adc_start_conv(uint32_t channel)
{
	uint32_t con1, con2;

	con2 = readl(ADC_BASE_ADDR + ADC_CON2);
	con2 &= ~ADC_CON2_ACH_MASK;
	con2 |= ADC_CON2_ACH_SEL(channel);
	writel(con2, ADC_BASE_ADDR + ADC_CON2);

	con1 = readl(ADC_BASE_ADDR + ADC_CON1);
	writel(con1 | ADC_CON_EN_START, ADC_BASE_ADDR + ADC_CON1);
}

int exynos_adc_read_raw(uint32_t channel)
{
	int val = -1;

	init_adc();
	adc_start_conv(channel);

	mdelay(100);

	val = readl(ADC_BASE_ADDR + ADC_DAT) & ADC_DAT_MASK;

	deinit_adc();
	return val;
}
