/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron Europe GmbH */

/* Common include header for shell tools. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _WIN32
#include <strings.h>
#else
#include <tchar.h>
#include <stdint.h>
#define strncasecmp _strnicmp
#define gmtime_r(arg1, arg2) gmtime_s(arg2, arg1);
#endif

#include <keapi.h>

#ifndef __shell_tools_h
#define __shell_tools_h

#define DATA_BUFFER_SIZE 4096

enum ApiClass {
	CT_BAT = 0,
	CT_CPU,
	CT_DISK,
	CT_DISP,
	CT_EEP,
	CT_FAN,
	CT_GEN,
	CT_GPIO,
	CT_I2C,
	CT_MEM,
	CT_NET,
	CT_PCI,
	CT_PERF,
	CT_SMBUS,
	CT_SPI,
	CT_TEMP,
	CT_VOLT,
	CT_WDT
};

struct cmd_struct {
	enum ApiClass apiClass;
	const char *apiClassStr;
	int (*call)(const char *, int, const char **);
	const char *usage;
};

void keapi_perror(const char *msg, uint32_t retvalue);
void keapi_success(const char *msg, uint32_t retvalue);
void check_params(int argc, int expected, const char *api_name);
void print_string(const char *msg, char *string);

extern const char battery_usage[];
extern const char cpu_usage[];
extern const char disk_usage[];
extern const char display_usage[];
extern const char eeprom_usage[];
extern const char fan_usage[];
extern const char general_usage[];
extern const char gpio_usage[];
extern const char i2c_usage[];
extern const char memory_usage[];
extern const char netdev_usage[];
extern const char pcidev_usage[];
extern const char performance_usage[];
extern const char smbus_usage[];
extern const char spi_usage[];
extern const char temperature_usage[];
extern const char voltage_usage[];
extern const char watchdog_usage[];

extern int battery_call(const char *FunctName, int argc, const char *argv[]);
extern int cpu_call(const char *FunctName, int argc, const char *argv[]);
extern int disk_call(const char *FunctName, int argc, const char *argv[]);
extern int display_call(const char *FunctName, int argc, const char *argv[]);
extern int eeprom_call(const char *FunctName, int argc, const char *argv[]);
extern int fan_call(const char *FunctName, int argc, const char *argv[]);
extern int general_call(const char *FunctName, int argc, const char *argv[]);
extern int gpio_call(const char *FunctName, int argc, const char *argv[]);
extern int i2c_call(const char *FunctName, int argc, const char *argv[]);
extern int memory_call(const char *FunctName, int argc, const char *argv[]);
extern int netdev_call(const char *FunctName, int argc, const char *argv[]);
extern int pcidev_call(const char *FunctName, int argc, const char *argv[]);
extern int performance_call(const char *FunctName, int argc, const char *argv[]);
extern int smbus_call(const char *FunctName, int argc, const char *argv[]);
extern int spi_call(const char *FunctName, int argc, const char *argv[]);
extern int temperature_call(const char *FunctName, int argc, const char *argv[]);
extern int voltage_call(const char *FunctName, int argc, const char *argv[]);
extern int watchdog_call(const char *FunctName, int argc, const char *argv[]);

#define CHECK_PARAMS(exp) check_params(argc, exp, FunctName);

#endif /* __shell_tools_h */
