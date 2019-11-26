/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* Watchdog functions. */

#include "shell_tools.h"

const char watchdog_usage[] = "\
    * WatchdogGetCaps\n\
    * WatchdogSetup <stage_to_be_set_up> <timeout_in_ms> <stage_mode>\n\
    \t** stage_modes:\n\
    \t 0 - KEAPI_WD_MODE_RESET\n\
    \t 1 - KEAPI_WD_MODE_INTERRUPT\n\
    \t 2 - KEAPI_WD_MODE_TIMER_ONLY\n\
    \t-1 - KEAPI_WD_MODE_DISABLE\n\
    * WatchdogEnable\n\
    * WatchdogTrigger\n\
    * WatchdogDisable\n\
    * WatchdogGetExpired\n\
    * WatchdogClearExpired\n\
    * WatchdogWaitUntilExpired\n";

int watchdog_call(const char *FunctName, int argc, const char *argv[])
{
	uint32_t retvalue;

	if (strncasecmp(FunctName, "WatchdogGetCaps", KEAPI_MAX_STR) == 0) {
		int32_t maxTimeout;
		int32_t minTimeout;
		int32_t stagesNr;
		CHECK_PARAMS(0);

		retvalue = KEApiWatchdogGetCaps(&maxTimeout, &minTimeout, &stagesNr);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("Watchdog maximum timeout: %d mS\nWatchdog minimum timeout: %d mS\n"
			       "Watchdog number of stages: %d\n\n",
			       maxTimeout, minTimeout, stagesNr);

	} else if (strncasecmp(FunctName, "WatchdogSetup", KEAPI_MAX_STR) == 0) {
		int32_t stage;
		int32_t timeout;
		int32_t mode;
		CHECK_PARAMS(3);

		stage = atoi(argv[0]);
		timeout = atoi(argv[1]);
		mode = atoi(argv[2]);
		retvalue = KEApiWatchdogSetup(stage, timeout, mode);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("Watchdog Setup:\nStage: %d\nTimeout: %d\nMode: %d\n", stage,
			       timeout, mode);

	} else if (strncasecmp(FunctName, "WatchdogEnable", KEAPI_MAX_STR) == 0) {
		CHECK_PARAMS(0);

		retvalue = KEApiWatchdogEnable();
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("Watchdog enabled.\n");

	} else if (strncasecmp(FunctName, "WatchdogTrigger", KEAPI_MAX_STR) == 0) {
		CHECK_PARAMS(0);

		retvalue = KEApiWatchdogTrigger();
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("Watchdog triggered.\n");

	} else if (strncasecmp(FunctName, "WatchdogDisable", KEAPI_MAX_STR) == 0) {
		CHECK_PARAMS(0);

		retvalue = KEApiWatchdogDisable();
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("Watchdog disabled.\n");

	} else if (strncasecmp(FunctName, "WatchdogGetExpired", KEAPI_MAX_STR) == 0) {
		int32_t wte;
		CHECK_PARAMS(0);

		retvalue = KEApiWatchdogGetExpired(&wte);
		if (retvalue == KEAPI_RET_SUCCESS) {
			if (wte == 0)
				printf("WTE = %d, Watchdog timer is not expired.\n", wte);
			else
				printf("WTE = %d, Watchdog timer has been expired.\n", wte);
		}

	} else if (strncasecmp(FunctName, "WatchdogClearExpired", KEAPI_MAX_STR) == 0) {
		CHECK_PARAMS(0);

		retvalue = KEApiWatchdogClearExpired();
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("Watchdog Timer Expired status was cleared.\n");

	} else if (strncasecmp(FunctName, "WatchdogWaitUntilExpired", KEAPI_MAX_STR) == 0) {
		CHECK_PARAMS(0);

		retvalue = KEApiWatchdogWaitUntilExpired();
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("Waiting until watchdog expired.\n");

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
