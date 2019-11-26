/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* Memory information. */

#include "shell_tools.h"

const char memory_usage[] = "\
    * GetMemoryInfo\n";

int memory_call(const char *FunctName, int argc, const char *argv[])
{
	uint32_t retvalue;

	if (strncasecmp(FunctName, "GetMemoryInfo", KEAPI_MAX_STR) == 0) {
		KEAPI_MEMORY_INFO memory_info;
		CHECK_PARAMS(0);

		retvalue = KEApiGetMemoryInfo(&memory_info);

		if (retvalue == KEAPI_RET_SUCCESS || retvalue == KEAPI_RET_PARTIAL_SUCCESS)
			printf("Total memory: %d MB\nFree memory: %d MB\nMemory speed: %d MHz\nMemory type: %s\n",
			       memory_info.memTotal,
			       memory_info.memFree,
			       memory_info.memSpeed,
			       memory_info.memType);
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
