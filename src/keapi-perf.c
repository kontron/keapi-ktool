/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron Europe GmbH */

/* System performance functions. */

#include "shell_tools.h"

const char performance_usage[] = "\
    * PerformanceStateCaps\n\
    * GetPerformanceStateDescription <state_number>\n\
    * GetPerformanceState\n\
    * SetPerformanceState <state_number>\n";

int performance_call(const char *FunctName, int argc, const char *argv[])
{
	uint32_t retvalue;

	if (strncasecmp(FunctName, "PerformanceStateCaps", KEAPI_MAX_STR) == 0) {
		uint32_t stateMask;
		int8_t b = 1;
		CHECK_PARAMS(0);

		retvalue = KEApiPerformanceStateCaps(&stateMask);

		if (retvalue == KEAPI_RET_SUCCESS) {
			printf("State caps mask: 0x%x\n", stateMask);
			printf("Available states: 0");
			while (stateMask) {
				if (stateMask & 0x01)
					printf(", %d", b);
				stateMask >>= 1;
				b++;
			}
			printf("\n");
		} else
			keapi_perror("KEApiPerformanceStateCaps - Error", retvalue);

	} else if (strncasecmp(FunctName, "GetPerformanceStateDescription", KEAPI_MAX_STR) == 0) {
		char desc[KEAPI_MAX_STR];
		uint32_t state, state_number;

		CHECK_PARAMS(1);

		state = state_number = atoi(argv[0]);
		if (state_number > 1)
			state = 1 << (state_number - 1);

		retvalue = KEApiGetPerformanceStateDescription(state, desc);

		if (retvalue == KEAPI_RET_SUCCESS) {
			printf("State: %d (mask 0x%x)\nDescription: %s\n", state_number, state,
			       desc);
		} else
			keapi_perror("KEApiGetPerformanceStateDescription - Error", retvalue);

	} else if (strncasecmp(FunctName, "GetPerformanceState", KEAPI_MAX_STR) == 0) {
		uint32_t state, buf_state, state_number = 0;
		CHECK_PARAMS(0);

		retvalue = KEApiGetPerformanceState(&state);

		buf_state = state;
		while (buf_state) {
			state_number++;
			buf_state >>= 1;
		}

		if (retvalue == KEAPI_RET_SUCCESS) {
			printf("Current state: %d (mask 0x%x)\n", state_number, state);
		} else
			keapi_perror("KEApiGetPerformanceState - Error", retvalue);

	} else if (strncasecmp(FunctName, "SetPerformanceState", KEAPI_MAX_STR) == 0) {
		uint32_t state, state_number;
		CHECK_PARAMS(1);

		state = state_number = atoi(argv[0]);
		if (state_number > 1)
			state = 1 << (state_number - 1);

		retvalue = KEApiSetPerformanceState(state);

		if (retvalue == KEAPI_RET_SUCCESS) {
			printf("State %d (mask 0x%x) have been successfully setup\n", state_number,
			       state);
		} else
			keapi_perror("KEApiSetPerformanceState - Error", retvalue);

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
