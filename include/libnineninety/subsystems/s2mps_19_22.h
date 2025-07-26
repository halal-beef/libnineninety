#pragma once

/* S2MPS19 slave address */
#define S2MPS19_PM_ADDR		0x1
#define S2MPS19_RTC_ADDR	0x2

/* S2MPS22 slave address */
#define S2MPS22_PM_ADDR		0x1

/* S2MPS19 Register Address */
#define S2MPS19_PM_INT1		0x000
#define S2MPS19_PM_INT2		0x001
#define S2MPS19_PM_INT3		0x002
#define S2MPS19_PM_INT4		0x003
#define S2MPS19_PM_INT5		0x004
#define S2MPS19_PM_PWRONSRC	0x010
#define S2MPS19_PM_OFFSRC	0x011
#define S2MPS19_PM_RTC_BUF	0x013
#define S2MPS19_PM_CTRL1	0x014
#define S2MPS19_PM_CTRL3	0x016
#define S2MPS19_PM_ETC_OTP	0x018
#define S2MPS19_PM_RTC_BUF	0x013

#define S2MPS19_PM_LDO2M_CTRL	0x047
#define S2MPS19_PM_LDO3M_CTRL	0x048
#define S2MPS19_PM_LDO4M_CTRL	0x049
#define S2MPS19_PM_LDO9M_CTRL	0x04E
#define S2MPS19_PM_LDO15M_CTRL	0x054
#define S2MPS19_PM_LDO10M_CTRL	0x04F
#define S2MPS19_PM_LDO11M_CTRL	0x050
#define S2MPS19_PM_LDO12M_CTRL	0x051
#define S2MPS19_PM_LDO16M_CTRL	0x055
#define S2MPS19_PM_LDO17M_CTRL	0x056

#define S2MPS19_RTC_WTSR_SMPL	0x001
#define S2MPU19_RTC_CAP_SEL	0x003

/* S2MPS22 Register Address */
#define S2MPS22_PM_CTRL1	0x013
#define S2MPS22_PM_LDO4S_CTRL	0x02D

/*
 * CTRL1
 */
#define MRSTB_EN		(1 << 4)

/*
 * CTRL3
 */
#define WRSTBIEN		(1 << 6)
#define MRSEL			(1 << 5)
#define WRSTEN			(1 << 4)

/*
 * LDOx_CTRL
 */
#define S2MPS_OUTPUT_ON_NORMAL	(3 << 6)
#define S2MPS_OUTPUT_ON_TCXO	(1 << 6)


#define S2MPS19_RTC_UPDATE	0x2
#define S2MPS19_RTC_MSEC	0x4
#define S2MPS19_RTC_SEC		0x5
#define S2MPS19_RTC_MIN		0x6
#define S2MPS19_RTC_HOUR	0x7
#define S2MPS19_RTC_WEEK	0x8
#define S2MPS19_RTC_DAY		0x9
#define S2MPS19_RTC_MONTH	0xA
#define S2MPS19_RTC_YEAR	0xB

/* RTC time struct */
typedef struct
{
	uint8_t  seconds;
	uint8_t  minutes;
	uint8_t  hours;

	uint8_t  weeks;

	uint8_t  day;
	uint8_t  month;
	uint16_t year;

	bool	is_pm;
} rtc_time_t;

/*
 * RTC_BUF
 */
#define _32KHZAP_EN	(0x1 << 0)
#define _32KHZPERI_EN	(0x1 << 2)

void pmic_init(void);
rtc_time_t get_pmic_rtc_time(void);
void pmic_disable_wtsr(void);
void pmic_disable_smpl(void);
void pmic_shutdown(void);
