#include <stdint.h>

#include <libnineninety/memory_operations.h>

#include <libnineninety/subsystems/pmu.h>

// Seriously, please do not call this.
void self_destruct(void)
{
	writel(0x6626CCCA, 0x80000000);
	reboot(NORMAL);
}
