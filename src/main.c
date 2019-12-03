/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron Europe GmbH */

/* Main entrance point. */

#include "shell_tools.h"

static int cmdApiIdx = -1;

static struct cmd_struct commands[] = {
	{ CT_BAT, "bat", battery_call, battery_usage },
	{ CT_CPU, "cpu", cpu_call, cpu_usage },
	{ CT_DISK, "disk", disk_call, disk_usage },
	{ CT_DISP, "disp", display_call, display_usage },
	{ CT_EEP, "eep", eeprom_call, eeprom_usage },
	{ CT_FAN, "fan", fan_call, fan_usage },
	{ CT_GEN, "gen", general_call, general_usage },
	{ CT_GPIO, "gpio", gpio_call, gpio_usage },
	{ CT_I2C, "i2c", i2c_call, i2c_usage },
	{ CT_MEM, "mem", memory_call, memory_usage },
	{ CT_NET, "net", netdev_call, netdev_usage },
	{ CT_PCI, "pci", pcidev_call, pcidev_usage },
	{ CT_PERF, "perf", performance_call, performance_usage },
	{ CT_SMBUS, "smbus", smbus_call, smbus_usage },
	{ CT_SPI, "spi", spi_call, spi_usage },
	{ CT_TEMP, "temp", temperature_call, temperature_usage },
	{ CT_VOLT, "volt", voltage_call, voltage_usage },
	{ CT_WDT, "wdt", watchdog_call, watchdog_usage },
};

static const char main_usage[] = "\n"
"Usage: ktool <API class> <API name> [<command_type_args>] [-h]\n\
\tAvailable API classes:\n\
    \t\tbat - battery functions;\n\
    \t\tcpu - cpu functions;\n\
    \t\tdisk - disk functions;\n\
    \t\tdisp - display functions;\n\
    \t\teep - eeprom functions;\n\
    \t\tfan - fan functions;\n\
    \t\tgen - general functions;\n\
    \t\tgpio - gpio functions;\n\
    \t\ti2c - i2c functions;\n\
    \t\tmem - memory functions;\n\
    \t\tnet - network info functions;\n\
    \t\tpci - pci info functions;\n\
    \t\tperf - performance functions;\n\
    \t\tsmbus - smbus functions;\n\
    \t\tspi - spi functions;\n\
    \t\ttemp - temperature sensors functions;\n\
    \t\tvolt - voltage sensors functions;\n\
    \t\twdt - watchdog functions\n\
\nTo learn more about API class args:\n\
\t\t$ ktool <API class> -h\n";

static const char cl_usage[] = "\n"
"Usage: ktool %s <API call> [<command_type_args>] [-h]\n\
\nAvailable API calls:\n%s\n";

void keapi_perror(const char *msg, uint32_t retvalue)
{
	char err[KEAPI_MAX_STR];
	err[0] = '\0';
	KEApiGetErrorString(retvalue, err, KEAPI_MAX_STR);
	fprintf(stderr, "%s: %s (0x%04X)\n", msg, err, retvalue);
}

void keapi_success(const char *msg, uint32_t retvalue)
{
	fprintf(stdout, "%s: success (0x%04X)\n", msg, retvalue);
}

void print_string(const char *msg, char *string)
{
	if (string[0] == '\0')
		fprintf(stdout, "%s none\n", msg);
	else
		fprintf(stdout, "%s %s\n", msg, string);
}

void check_params(int argc, int expected, const char *api_name)
{
	if (argc != expected) {
		fprintf(stderr, "API class: %s\nAPI call %s expected %d parameters:\n%s\n",
			commands[cmdApiIdx].apiClassStr,
			api_name,
			expected,
			commands[cmdApiIdx].usage);
		exit(1);
	}
}

void initial_parse_args(int argc, const char *argv[])
{
	uint32_t i;
	int32_t j;

	if (argc < 2 || strncasecmp(argv[1], "-h", KEAPI_MAX_STR) == 0) {
		fprintf(stderr, main_usage);
		exit(1);
	}

	for (i = 0; i < sizeof(commands) / sizeof(struct cmd_struct); i++) {
		if (strncasecmp(argv[1], commands[i].apiClassStr, KEAPI_MAX_STR) == 0) {
			cmdApiIdx = i;

			if (argc < 3) {
				fprintf(stderr, cl_usage, argv[1], commands[cmdApiIdx].usage);
				exit(1);
			}

			for (j = 2; j < argc; j++)
				if (strncasecmp(argv[j], "-h", KEAPI_MAX_STR) == 0) {
					fprintf(stderr, cl_usage, argv[1],
						commands[cmdApiIdx].usage);
					exit(0);
				}

			return;
		}
	}

	fprintf(stderr, main_usage);
	exit(1);
}

int main(int argc, const char *argv[])
{
	uint32_t retvalue = 0;
	int ret;

	initial_parse_args(argc, argv);

	if ((retvalue = KEApiLibInitialize()) != KEAPI_RET_SUCCESS) {
		keapi_perror("KEApiLibInitialize failed", retvalue);
		exit(retvalue);
	}

	ret = commands[cmdApiIdx].call(argv[2], argc - 3, argv + 3);

	if (ret) {
		fprintf(stderr, cl_usage, argv[1], commands[cmdApiIdx].usage);
		KEApiLibUnInitialize();
		exit(ret);
	}

	if ((retvalue = KEApiLibUnInitialize()) != KEAPI_RET_SUCCESS) {
		keapi_perror("KEApiLibUnInitialize failed", retvalue);
		exit(retvalue);
	}

	return 0;
}
