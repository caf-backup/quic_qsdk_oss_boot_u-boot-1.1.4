/*
 * Copyright (c) 2018, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <common.h>
#include <command.h>

#if defined(CONFIG_MACH_QCN550x) && (FLASH_SIZE == 32)
#define KERNEL_DDR_LOAD_ADDR	0x83000000
#define FLASH_SIZE_32M		0x2000000
#define FLASH_SIZE_16M		0x1000000

int do_bootqca(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	flash_info_t *info;
	char runcmd[256];
	ulong addr;
	ulong len;

	if (argc < 3) {
		printf("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	addr = simple_strtoul(argv[1], NULL, 16);
	len = simple_strtoul(argv[2], NULL, 16);

	if ((addr < CFG_FLASH_BASE) ||
	    ((addr + len) > (CFG_FLASH_BASE + FLASH_SIZE_32M))) {
		printf("Bad Flash Address");
		return 1;
	} else if ((addr + len) > (FLASH_SIZE_16M + CFG_FLASH_BASE)) {
		info = addr2info(addr);
		read_buff(info, (uchar *)KERNEL_DDR_LOAD_ADDR, addr, len);
		addr = KERNEL_DDR_LOAD_ADDR;
	}

	snprintf(runcmd, sizeof(runcmd), "bootm 0x%x\n", addr);

	if (run_command(runcmd, 0) != 0)
		return 1;

	return 0;
}

U_BOOT_CMD(
	bootqca,    3,    1,    do_bootqca,
	"bootqca - boot the kernel\n",
"bootqca <kernel_addr> <kernel_size>\n\n"
"kernel_addr and kernel_size values should be in hex.\n\n"
"If the given address greater than 16MB, SPI driver will read the\n"
"data from flash into DDR and boot from loaded DDR address.\n"
"    - bootqca\n"
);
#endif
