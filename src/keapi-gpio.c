/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron Europe GmbH */

/* GPIO pins control. */

#include "shell_tools.h"

const char gpio_usage[] = "\
    * GetGpioPortCount\n\
    * GetGpioPortDirectionCaps <port_number>\n\
    * GetGpioPortDirections <port_number>\n\
    * SetGpioPortDirections <port_number> <directions>\n\
    \t** direction:\n\
    \t- 1 - IN\n\
    \t- 0 - OUT\n\
    * GetGpioPortLevels <port_number>\n\
    * SetGpioPortLevels <port_number> <levels>\n\
    \t** level:\n\
    \t- 0 - LOW\n\
    \t- 1 - HIGH\n\n\
    Examples: \n\
    Set GPIO port 0 pins 1 and 2 to \"OUT\":\n\
    \tSetGpioPortDirections 0 0xFC\n\
    Set GPIO port 0 pins 1, 5 and 8 to \"HIGH\":\n\
    \tSetGpioPortLevels 0 0x91\n\
    \n";

int gpio_call(const char *FunctName, int argc, const char *argv[])
{
	uint32_t retvalue;

	if (strncasecmp(FunctName, "GetGpioPortCount", KEAPI_MAX_STR) == 0) {
		int32_t port_count;
		CHECK_PARAMS(0);

		retvalue = KEApiGetGpioPortCount(&port_count);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("GPIO port count = %d\n", port_count);
	} else if (strncasecmp(FunctName, "GetGpioPortDirectionCaps", KEAPI_MAX_STR) == 0) {
		int32_t gpioNr = 0;
		uint32_t IsInput;
		uint32_t IsOutput;
		CHECK_PARAMS(1);

		gpioNr = atoi(argv[0]);
		retvalue = KEApiGetGpioPortDirectionCaps(gpioNr, &IsInput, &IsOutput);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("GPIO port number: %d\nIs input: 0x%x\nIs output: 0x%x\n", gpioNr,
			       IsInput, IsOutput);
	} else if (strncasecmp(FunctName, "GetGpioPortLevels", KEAPI_MAX_STR) == 0) {
		int32_t gpioNr = 0;
		uint32_t levels;
		CHECK_PARAMS(1);

		gpioNr = atoi(argv[0]);
		retvalue = KEApiGetGpioPortLevels(gpioNr, &levels);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("GPIO port number: %d\nLevels = 0x%x\n", gpioNr, levels);
	} else if (strncasecmp(FunctName, "SetGpioPortLevels", KEAPI_MAX_STR) == 0) {
		int32_t gpioNr = 0;
		uint32_t levels = 1;
		CHECK_PARAMS(2);

		gpioNr = atoi(argv[0]);
		levels = (uint32_t)strtol(argv[1], NULL, 16);

		retvalue = KEApiSetGpioPortLevels(gpioNr, levels);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("GPIO port number: %d\nLevels = 0x%x\n", gpioNr, levels);
	} else if (strncasecmp(FunctName, "GetGpioPortDirections", KEAPI_MAX_STR) == 0) {
		int32_t gpioNr;
		uint32_t directions;
		CHECK_PARAMS(1);

		gpioNr = atoi(argv[0]);

		retvalue = KEApiGetGpioPortDirections(gpioNr, &directions);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("GPIO port number: %d\nDirections: 0x%x\n", gpioNr, directions);
	} else if (strncasecmp(FunctName, "SetGpioPortDirections", KEAPI_MAX_STR) == 0) {
		int32_t gpioNr = 0;
		uint32_t directions = 1;
		CHECK_PARAMS(2);

		gpioNr = atoi(argv[0]);
		directions = (uint32_t)strtol(argv[1], NULL, 16);

		retvalue = KEApiSetGpioPortDirections(gpioNr, directions);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("GPIO port number: %d\nDirections: 0x%x\n", gpioNr, directions);
	} else {
		fprintf(stderr, "unknown api call: %s\n", FunctName);
		return -1;
	}

	if (retvalue) {
		keapi_perror("returned", retvalue);
	} else
		keapi_success("returned", retvalue);

	return 0;
}
