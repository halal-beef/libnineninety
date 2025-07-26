#pragma once

#include <stdint.h>

#define MHz(x)	(x * 1000000)

#define CNTFRQ_EL0 MHz(26)

void udelay(uint32_t us);
#define mdelay(ms) udelay(ms * 1000);
