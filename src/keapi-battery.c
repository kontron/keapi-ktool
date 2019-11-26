/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* Information about battery. */

#include "shell_tools.h"

const char battery_usage[] = "\
  * GetBatteryCount\n\
  * GetBatteryInfo [battery_number]\n\
  * GetBatteryState [battery_number]\n";

int battery_call(const char *FunctName, int argc, const char *argv[])
{
	uint32_t retvalue;

	if (strncasecmp(FunctName, "GetBatteryCount", KEAPI_MAX_STR) == 0) {
		int32_t battery_count = 0;
		CHECK_PARAMS(0);

		retvalue = KEApiGetBatteryCount(&battery_count);

		if (retvalue == KEAPI_RET_SUCCESS)
			printf("Battery count: %d\n", battery_count);
		else
			printf("KEApiGetBatteryCount - Error in function execution.\n");
	} else if (strncasecmp(FunctName, "GetBatteryInfo", KEAPI_MAX_STR) == 0) {
		uint8_t battery_nr;
		KEAPI_BATTERY_INFO battery_info;
		CHECK_PARAMS(1);

		battery_nr = atoi(argv[0]);

		retvalue = KEApiGetBatteryInfo(battery_nr, &battery_info);
		if (retvalue == KEAPI_RET_SUCCESS || retvalue == KEAPI_RET_PARTIAL_SUCCESS)
			printf("Battery number: %d\nDevice name: %s\nBattery type: %s\nBattery SN: %s\nDesigned voltage: %d\nDesigned capacity: %d\nFully charged capacity: %d\nCycle Count: %d\n\n",
			       battery_nr,
			       battery_info.deviceName,
			       battery_info.type,
			       battery_info.serialNumber,
			       battery_info.designedVoltage,
			       battery_info.designedCapacity,
			       battery_info.fullyChargedCapacity,
			       battery_info.cycleCount);
		else
			printf("KEApiGetBatteryInfo - Error in function execution.\n");
	} else if (strncasecmp(FunctName, "GetBatteryState", KEAPI_MAX_STR) == 0) {
		uint8_t battery_nr;
		KEAPI_BATTERY_STATE battery_state;
		CHECK_PARAMS(1);

		battery_nr = atoi(argv[0]);
		retvalue = KEApiGetBatteryState(battery_nr, &battery_state);
		if (retvalue == KEAPI_RET_SUCCESS || retvalue == KEAPI_RET_PARTIAL_SUCCESS)
			printf("Battery number: %d\nPower state: %d\nFull battery remaining time: %d\nRemaining time: %d\nRemaining capacity: %d\nCurrent voltage: %d\nCharge rate: %d\nCharge state: %d\n",
			       battery_nr,
			       battery_state.powerState,
			       battery_state.fullBatteryRemainingTime,
			       battery_state.remainingTime,
			       battery_state.remainingCapacity,
			       battery_state.currentVoltage,
			       battery_state.rate,
			       battery_state.chargeState);
		else
			printf("KEApiGetBatteryState - Error in function execution.\n");
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
