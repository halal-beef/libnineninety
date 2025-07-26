#include <stdbool.h>
#include <stdint.h>

#include <libnineninety/exynos9830/sfr.h>

#include <libnineninety/timer/delay.h>

#include <libnineninety/subsystems/speedy.h>
#include <libnineninety/subsystems/s2mps_19_22.h>

void pmic_init(void)
{
	uint8_t reg;

	speedy_init(SPEEDY0_BASE);
	speedy_init(SPEEDY1_BASE);

	/* Disable Manual Reset */
	speedy_read(SPEEDY0_BASE, S2MPS19_PM_ADDR, S2MPS19_PM_CTRL1, &reg);
	reg &= ~MRSTB_EN;
	speedy_write(SPEEDY0_BASE, S2MPS19_PM_ADDR, S2MPS19_PM_CTRL1, reg);

	/* Enable Warm Reset */
	speedy_read(SPEEDY0_BASE, S2MPS19_PM_ADDR, S2MPS19_PM_CTRL3, &reg);
	reg |= WRSTEN;
	speedy_write(SPEEDY0_BASE, S2MPS19_PM_ADDR, S2MPS19_PM_CTRL3, reg);

	/* LCD power */
	speedy_read(SPEEDY0_BASE, S2MPS19_PM_ADDR, S2MPS19_PM_LDO3M_CTRL, &reg);
	reg |= S2MPS_OUTPUT_ON_NORMAL;
	speedy_write(SPEEDY0_BASE, S2MPS19_PM_ADDR, S2MPS19_PM_LDO3M_CTRL, reg);

	speedy_read(SPEEDY0_BASE, S2MPS19_PM_ADDR, S2MPS19_PM_LDO4M_CTRL, &reg);
	reg |= S2MPS_OUTPUT_ON_NORMAL;
	speedy_write(SPEEDY0_BASE, S2MPS19_PM_ADDR, S2MPS19_PM_LDO4M_CTRL, reg);

	speedy_read(SPEEDY0_BASE, S2MPS19_PM_ADDR, S2MPS19_PM_LDO9M_CTRL, &reg);
	reg |= S2MPS_OUTPUT_ON_NORMAL;
	speedy_write(SPEEDY0_BASE, S2MPS19_PM_ADDR, S2MPS19_PM_LDO9M_CTRL, reg);

	speedy_read(SPEEDY1_BASE, S2MPS22_PM_ADDR, S2MPS22_PM_LDO4S_CTRL, &reg);
	reg |= S2MPS_OUTPUT_ON_NORMAL;
	speedy_write(SPEEDY1_BASE, S2MPS22_PM_ADDR, S2MPS22_PM_LDO4S_CTRL, reg);

	/* ICEN enable for PB02 & PB03 */
	speedy_read(SPEEDY0_BASE, S2MPS19_PM_ADDR, S2MPS19_PM_LDO16M_CTRL, &reg);
	reg |= S2MPS_OUTPUT_ON_NORMAL;
	speedy_write(SPEEDY0_BASE, S2MPS19_PM_ADDR, S2MPS19_PM_LDO16M_CTRL, reg);

	/* LDO 10, 11, 12 On for USB */
	speedy_read(SPEEDY0_BASE, S2MPS19_PM_ADDR, S2MPS19_PM_LDO10M_CTRL, &reg);
	reg |= S2MPS_OUTPUT_ON_TCXO;
	speedy_write(SPEEDY0_BASE, S2MPS19_PM_ADDR, S2MPS19_PM_LDO10M_CTRL, reg);

	speedy_read(SPEEDY0_BASE, S2MPS19_PM_ADDR, S2MPS19_PM_LDO11M_CTRL, &reg);
	reg |= S2MPS_OUTPUT_ON_TCXO;
	speedy_write(SPEEDY0_BASE, S2MPS19_PM_ADDR, S2MPS19_PM_LDO11M_CTRL, reg);

	speedy_read(SPEEDY0_BASE, S2MPS19_PM_ADDR, S2MPS19_PM_LDO12M_CTRL, &reg);
	reg |= S2MPS_OUTPUT_ON_TCXO;
	speedy_write(SPEEDY0_BASE, S2MPS19_PM_ADDR, S2MPS19_PM_LDO12M_CTRL, reg);

	/* Enable WTSR */
	speedy_read(SPEEDY0_BASE, S2MPS19_RTC_ADDR, S2MPS19_RTC_WTSR_SMPL, &reg);
	reg = (reg & 0xB8) | 0x43;
	speedy_write(SPEEDY0_BASE, S2MPS19_RTC_ADDR, S2MPS19_RTC_WTSR_SMPL, reg);
}

rtc_time_t get_pmic_rtc_time(void)
{
	rtc_time_t time = {};
	uint8_t tmp = 0;

	/* Trigger RTC update */
	speedy_read(SPEEDY0_BASE, S2MPS19_RTC_ADDR, S2MPS19_RTC_UPDATE, &tmp);
	tmp |= 1;
	speedy_write(SPEEDY0_BASE, S2MPS19_RTC_ADDR, S2MPS19_RTC_UPDATE, tmp);

	/* Wait for update */
	udelay(40);

	speedy_read(SPEEDY0_BASE, S2MPS19_RTC_ADDR, S2MPS19_RTC_SEC, &time.seconds);
	speedy_read(SPEEDY0_BASE, S2MPS19_RTC_ADDR, S2MPS19_RTC_MIN, &time.minutes);
	speedy_read(SPEEDY0_BASE, S2MPS19_RTC_ADDR, S2MPS19_RTC_HOUR, &time.hours);
	speedy_read(SPEEDY0_BASE, S2MPS19_RTC_ADDR, S2MPS19_RTC_WEEK, &time.weeks);
	speedy_read(SPEEDY0_BASE, S2MPS19_RTC_ADDR, S2MPS19_RTC_DAY, &time.day);
	speedy_read(SPEEDY0_BASE, S2MPS19_RTC_ADDR, S2MPS19_RTC_MONTH, &time.month);
	speedy_read(SPEEDY0_BASE, S2MPS19_RTC_ADDR, S2MPS19_RTC_YEAR, &tmp);

	time.year = tmp + 2000;

	time.is_pm = !!(time.hours & (1 << 6));

	time.hours &= 0x1F;

	return time;
}

void pmic_disable_wtsr(void)
{
	unsigned char reg;

	speedy_read(SPEEDY0_BASE, S2MPS19_RTC_ADDR, S2MPS19_RTC_WTSR_SMPL, &reg);

	reg &= ~(1 << 6); // Clear bit 6 (WTSR)

	speedy_write(SPEEDY0_BASE, S2MPS19_RTC_ADDR, S2MPS19_RTC_WTSR_SMPL, reg);
}

void pmic_disable_smpl(void)
{
	unsigned char reg;

	speedy_read(SPEEDY0_BASE, S2MPS19_RTC_ADDR, S2MPS19_RTC_WTSR_SMPL, &reg);

	reg &= ~(1 << 7); // Clear bit 7 (SMPL)
	reg &= ~(1 << 8); // Clear bit 8 (SUB_SMPL)

	speedy_write(SPEEDY0_BASE, S2MPS19_RTC_ADDR, S2MPS19_RTC_WTSR_SMPL, reg);
}

void pmic_shutdown(void)
{
	unsigned char reg;

	speedy_read(SPEEDY1_BASE, S2MPS22_PM_ADDR, S2MPS22_PM_CTRL1, &reg);

	reg |= (1 << 7);

	speedy_write(SPEEDY1_BASE, S2MPS22_PM_ADDR, S2MPS22_PM_CTRL1, reg);
}
