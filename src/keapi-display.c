/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* Display brightness control. */

#include "shell_tools.h"

const char display_usage[] = "\
    * GetDisplayCount\n\
    * GetBacklightValue <display_number>\n\
    * SetBacklightValue <display_number> <backlight (0..255)>\n";

int display_call(const char *FunctName, int argc, const char *argv[])
{
	uint32_t retvalue;

	if (strncasecmp(FunctName, "GetDisplayCount", KEAPI_MAX_STR) == 0) {
		int32_t DisplayCount = 0;
		CHECK_PARAMS(0);

		retvalue = KEApiGetDisplayCount(&DisplayCount);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("Number of displays: %d\n", DisplayCount);
	} else if (strncasecmp(FunctName, "GetBacklightValue", KEAPI_MAX_STR) == 0) {
		int32_t displayNr = 0;
		int32_t backlight;
		CHECK_PARAMS(1);

		displayNr = atoi(argv[0]);
		retvalue = KEApiGetBacklightValue(displayNr, &backlight);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("Backlight: %d\n", backlight);
	} else if (strncasecmp(FunctName, "SetBacklightValue", KEAPI_MAX_STR) == 0) {
		int32_t displayNr = 0;
		int32_t backlight = 66;
		CHECK_PARAMS(2);

		displayNr = atoi(argv[0]);
		backlight = atoi(argv[1]);
		retvalue = KEApiSetBacklightValue(displayNr, backlight);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("Display number: %d\nwas set to backlight: %d\n", displayNr,
			       backlight);
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
