/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* CPU information. */

#include "shell_tools.h"

const char cpu_usage[] = "\
    * GetCpuFreq <core_number> <frequency_type>\n\
    \t** frequency_types:\n\
    \t0 - KEAPI_CPU_FREQUENCY_MIN\n\
    \t1 - KEAPI_CPU_FREQUENCY_MAX\n\
    \t2 - KEAPI_CPU_FREQUENCY_CURRENT\n\
    \t3 - KEAPI_CPU_FREQUENCY_TURBO\n\
    * GetCpuInfo\n\
    * GetCpuPerformance <core_number>\n";

int cpu_call(const char *FunctName, int argc, const char *argv[])
{
	uint32_t retvalue;

	if (strncasecmp(FunctName, "GetCpuFreq", KEAPI_MAX_STR) == 0) {
		int32_t frequency, coreNr;
		int8_t frequencyType;
		CHECK_PARAMS(2);

		coreNr = atoi(argv[0]);
		frequencyType = (int8_t)atoi(argv[1]);

		retvalue = KEApiGetCpuFreq(coreNr, frequencyType, &frequency);

		if (retvalue == KEAPI_RET_SUCCESS)
			printf("Core number: %d\nFrequency type: %d\nFrequency: %d MHz\n", coreNr,
			       frequencyType, frequency / 1000);

	} else if (strncasecmp(FunctName, "GetCpuInfo", KEAPI_MAX_STR) == 0) {
		KEAPI_CPU_INFO cpu_info;
		CHECK_PARAMS(0);

		retvalue = KEApiGetCpuInfo(&cpu_info);
		if (retvalue == KEAPI_RET_SUCCESS || retvalue == KEAPI_RET_PARTIAL_SUCCESS)
			printf("CPU name: %s\nCPU count: %d\nCore count: %d\nThread count: %d\n",
			       cpu_info.cpuName,
			       cpu_info.cpuCount,
			       cpu_info.cpuCoreCount,
			       cpu_info.cpuThreadCount);

	} else if (strncasecmp(FunctName, "GetCpuPerformance", KEAPI_MAX_STR) == 0) {
		int8_t performancePercentage = 0;
		int32_t core_nr;
		CHECK_PARAMS(1);

		core_nr = atoi(argv[0]);
		retvalue = KEApiGetCpuPerformance(core_nr, &performancePercentage);

		if (retvalue == KEAPI_RET_SUCCESS)
			printf("CPU performance is %d.\n", performancePercentage);
		else
			printf("KEApiGetCpuPerformance - Error in function execution.\n");
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
