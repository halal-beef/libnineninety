#include <libnineninety/timer/delay.h>

void udelay(uint32_t us)
{
	uint64_t start, now;

	__asm__ volatile("mrs %0, cntpct_el0" : "=r" (start));

	uint64_t ticks = (CNTFRQ_EL0 / 1000000) * us;

	do {
		__asm__ volatile("mrs %0, cntpct_el0" : "=r" (now));
	} while ((now - start) < ticks);
}
