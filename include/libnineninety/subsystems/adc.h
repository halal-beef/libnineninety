#include <libnineninety/exynos9830/sfr.h>
#include <stdint.h>

#define MAX_CHANNEL		11

enum adc_reg
{
	ADC_CON1 = 0x0,
	ADC_CON2 = 0x4,
	ADC_DAT = 0x8,
	ADC_SUM_DAT = 0xC,
	ADC_INT_EN = 0x10,
	ADC_INT_STATUS = 0x14,
	ADC_DEBUG_DATA = 0x1C
};

#define ADC_BASE_ADDR		EXYNOS_ADC_BASE

#define ADC_SOFT_RESET		(1 << 2)
#define ADC_NON_SOFT_RESET	(1 << 1)
#define ADC_CON2_C_TIME(x)	(((x) & 7) << 4)
#define ADC_CON2_ACH_MASK	(0xF)
#define ADC_CON2_ACH_SEL(x)	(((x) & 0xF) << 0)
#define ADC_CON_EN_START	(1 << 0)
#define ADC_DAT_MASK		(0xFFF)
#define EN_INT			(1)

int exynos_adc_read_raw(uint32_t channel);
