#include <libnineninety/exynos9830/sfr.h>

#include <libnineninety/memory_operations.h>

#include <libnineninety/timer/delay.h>

#include <libnineninety/subsystems/pmu.h>
#include <libnineninety/subsystems/s2mps_19_22.h>

static void sec_set_reboot_magic(int magic, int offset, int mask)
{
	uint32_t tmp = 0;

	tmp = readl(EXYNOS_POWER_INFORM2);

	mask <<= offset;
	tmp &= ~mask;
	tmp |= magic << offset;

	writel(tmp, EXYNOS_POWER_INFORM2);
}

void power_off(void)
{
	uint32_t ps_hold = readl(EXYNOS_POWER_PS_HOLD_CONTROL);
	ps_hold &= 0xFFFFFEFF; // Hold PS_HOLD low

	pmic_disable_wtsr();
	pmic_disable_smpl();
	pmic_shutdown();

	writel(ps_hold, EXYNOS_POWER_PS_HOLD_CONTROL);
}

void reboot(reboot_mode mode)
{
	int reason = SEC_RESET_REASON_PREFIX;

	switch (mode)
	{
		case NORMAL:
			goto reboot;
			break;

		case FASTBOOTD:
			writel(0xFA, EXYNOS_POWER_SYSIP_DAT0);
			reason |= RECOVERY;
			break;

		default:
			reason |= mode;
			break;
	}

	writel(reason, EXYNOS_POWER_INFORM3);

reboot:
	sec_set_reboot_magic(SEC_REBOOT_NORMAL, SEC_REBOOT_END_OFFSET, 0xFF);
	writel(0, EXYNOS_POWER_RST_STAT);
	writel(readl(EXYNOS_POWER_SYSTEM_CONFIGURATION) | 0x2, EXYNOS_POWER_SYSTEM_CONFIGURATION);
}
