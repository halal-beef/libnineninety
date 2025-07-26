#pragma once

#define SEC_DEBUG_MAGIC_INFORM          0x0808
#define SEC_DEBUG_PANIC_INFORM          0x080C

#define SEC_REBOOT_END_OFFSET           (16)

#define SEC_RESET_REASON_PREFIX         0x12345600

#define SEC_REBOOT_NORMAL               0x4E

typedef enum
{
	NORMAL = 0,
	DOWNLOAD = 1,
	RECOVERY = 4,
	BOOTLOADER = 13,
	FASTBOOTD,
} reboot_mode;

void power_off(void);
void reboot(reboot_mode mode);
