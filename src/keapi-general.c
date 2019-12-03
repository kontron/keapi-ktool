/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron Europe GmbH */

/* Basic system information. */

#include "shell_tools.h"
#include "time.h"

const char general_usage[] = "\
    * GetLibVersion\n\
    * GetBoardInfo\n\
    * GetBootCounter\n\
    * SystemUpTime\n\
    * GetIntruderStatus\n\
    * ResetIntruderStatus\n\
    * GetPBITResult\n\
    * ClearPBITResult\n";

int general_call(const char *FunctName, int argc, const char *argv[])
{
	uint32_t retvalue;
	struct tm md, lr, fw;
	time_t buf = 0;

	if (strncasecmp(FunctName, "GetLibVersion", KEAPI_MAX_STR) == 0) {
		KEAPI_VERSION_DATA ver = { 0 };
		CHECK_PARAMS(0);

		retvalue = KEApiGetLibVersion(&ver);

		if (retvalue == KEAPI_RET_SUCCESS) {
			printf("Version: 0x%x\n", ver.version);
			printf("Text:    %s\n", ver.versionText);
		} else
			printf("KEApiGetLibVersion - Error in function execution.\n");
	} else if (strncasecmp(FunctName, "GetBoardInfo", KEAPI_MAX_STR) == 0) {
		KEAPI_BOARD_INFO board_info;
		CHECK_PARAMS(0);

		retvalue = KEApiGetBoardInfo(&board_info);

		if (retvalue == KEAPI_RET_SUCCESS || retvalue == KEAPI_RET_PARTIAL_SUCCESS) {
			memset(&md, 0, sizeof(struct tm));
			memset(&lr, 0, sizeof(struct tm));
			memset(&fw, 0, sizeof(struct tm));

			buf = (time_t)board_info.firmwareDate;
			gmtime_r(&buf, &fw);

			buf = (time_t)board_info.manufacturingDate;
			gmtime_r(&buf, &md);

			buf = (time_t)board_info.lastRepairDate;
			gmtime_r(&buf, &lr);

			printf("Board manufacturer: %s\n"
			       "Board name: %s\n"
			       "Board SN: %s\n"
			       "Carrier Info: %s\n"
			       "Hardware Version: %s\n"
			       "Manufacturing Date: %d/%d/%d\n"
			       "Last RepairDate: %d/%d/%d\n"
			       "Firmware date: %d/%d/%d\n"
			       "Firmware version: %s\n",
			       board_info.boardManufacturer,
			       board_info.boardName,
			       board_info.boardSerialNumber,
			       board_info.carrierInfo,
			       board_info.hardwareVersion,
			       md.tm_mon + 1, md.tm_mday, md.tm_year + 1900,
			       lr.tm_mon + 1, lr.tm_mday, lr.tm_year + 1900,
			       fw.tm_mon + 1, fw.tm_mday, fw.tm_year + 1900,
			       board_info.firmwareVersion);

		} else
			printf("KEApiGetBoardInfo - Error in function execution.\n");

	} else if (strncasecmp(FunctName, "GetBootCounter", KEAPI_MAX_STR) == 0) {
		int32_t boots = 0;
		CHECK_PARAMS(0);

		retvalue = KEApiGetBootCounter(&boots);

		if (retvalue == KEAPI_RET_SUCCESS)
			printf("Number of boots: %d\n", boots);
		else
			printf("KEApiGetBootCounter - Error in function execution.\n");
	} else if (strncasecmp(FunctName, "SystemUpTime", KEAPI_MAX_STR) == 0) {
		int32_t SystemUpTime = 0;
		CHECK_PARAMS(0);

		retvalue = KEApiSystemUpTime(&SystemUpTime);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("System uptime: %d seconds\n", SystemUpTime);
		else
			printf("KEApiSystemUpTime - Error in function execution.\n");

	} else if (strncasecmp(FunctName, "GetIntruderStatus", KEAPI_MAX_STR) == 0) {
		int32_t IntruderStatus = 0;
		CHECK_PARAMS(0);

		retvalue = KEApiGetIntruderStatus(&IntruderStatus);
		if (retvalue == KEAPI_RET_SUCCESS) {
			printf("IntruderStatus = %d, ", IntruderStatus);
			if (IntruderStatus == 0)
				printf("that means the case was not opened.\n");
			else if (IntruderStatus == 1)
				printf("that means the case was opened.\n");
		} else
			printf("KEApiGetIntruderStatus - Error in function execution.\n");

	} else if (strncasecmp(FunctName, "ResetIntruderStatus", KEAPI_MAX_STR) == 0) {
		CHECK_PARAMS(0);

		retvalue = KEApiResetIntruderStatus();
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("The intruder status was set to 'closed'.\n ");
		else
			printf("KEApiResetIntruderStatus - Error in function execution.\n");

	} else if (strncasecmp(FunctName, "GetPBITResult", KEAPI_MAX_STR) == 0) {
		uint32_t result = 0;
		uint32_t cumulativeResult = 0;
		CHECK_PARAMS(0);

		retvalue = KEApiGetPBITResult(&result, &cumulativeResult);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("Latest status = %d\nCumulative status = %d\n", result,
			       cumulativeResult);
		else
			printf("KEApiGetPBITResult - Error in function execution.\n");

	} else if (strncasecmp(FunctName, "ClearPBITResult", KEAPI_MAX_STR) == 0) {
		CHECK_PARAMS(0);

		retvalue = KEApiClearPBITResult();
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("Latest result of Power-on built-in test (PBIT) was reset.\n");
		else
			printf("KEApiClearPBITResult - Error in function execution.\n");

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
