#pragma once
#include <stdint.h>

#define INTGR0                  (0x0008)
#define INTCR1                  (0x0020)
#define INTSR1                  (0x0028)

#define INTR_FLAG_OFFSET	(16)

#define EXYNOS_MESSAGE_APM	(0x203C200)
#define SR0                     (0x0)
#define SR1                     (0x4)
#define SR2                     (0x8)
#define SR3                     (0xC)

//void acpm_ipc_send_data_async(uint32_t ipc_channel, uint32_t cmd_0, uint32_t cmd_1);
//void acpm_ipc_send_data_wait(uint32_t ipc_channel, uint32_t *cmd_0, uint32_t *cmd_1);
