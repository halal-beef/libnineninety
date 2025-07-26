#pragma once

#define REG64(addr) ((volatile uint64_t *)(uintptr_t)(addr))
#define REG32(addr) ((volatile uint32_t *)(uintptr_t)(addr))
#define REG16(addr) ((volatile uint16_t *)(uintptr_t)(addr))
#define REG8(addr) ((volatile uint8_t *)(uintptr_t)(addr))

#define writeq(v, a) (*REG64(a) = (v))
#define readq(a) (*REG64(a))
#define writel(v, a) (*REG32(a) = (v))
#define readl(a) (*REG32(a))
#define writeb(v, a) (*REG8(a) = (v))
#define readb(a) (*REG8(a))

#define be16_to_cpu(x) \
		((((x) & 0xff00) >> 8) | \
		 (((x) & 0x00ff) << 8))
#define cpu_to_be16(x) be16_to_cpu(x)

#define be32_to_cpu(x) \
		((((x) & 0xff000000) >> 24) | \
		 (((x) & 0x00ff0000) >>  8) | \
		 (((x) & 0x0000ff00) <<  8) | \
		 (((x) & 0x000000ff) << 24))
#define cpu_to_be32(x) be32_to_cpu(x)

#define be64_to_cpu(x) \
		((((x) & 0x00000000000000ff) << 56) | \
		 (((x) & 0x000000000000ff00) << 40) | \
		 (((x) & 0x0000000000ff0000) << 24) | \
		 (((x) & 0x00000000ff000000) << 8) | \
		 (((x) & 0x000000ff00000000) >> 8) | \
		 (((x) & 0x0000ff0000000000) >> 24) | \
		 (((x) & 0x00ff000000000000) >> 40) | \
		 (((x) & 0xff00000000000000) >> 56))
#define cpu_to_be64(x) be64_to_cpu(x)
