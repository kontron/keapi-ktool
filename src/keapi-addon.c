/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron Europe GmbH */

/* Led addon. */

#include "shell_tools.h"

const char addon_usage[] = "\
    * LedGetCount \n\
    * LedSetStatus <no> <status>\n\
    \t<no> index of LED (start: 0)\n\
    \t<status> 0: LED OFF 1: LED on\n\
    * LedGetStatus <no> \n\
    * LedGetConfig <no> \n\
    * LedSetConfig <no> <Colour> <Light> <Mode> <TOn> <TOff>\n\
    \t the parameters should fit with the LED-configuation\n\
    \n\
    * ( LedGetCaps <no> )\n\
    ";
    
int addon_call(const char *FunctName, int argc, const char *argv[])
{
	uint32_t retvalue;

   	if (strncasecmp(FunctName, "LedGetCount", KEAPI_MAX_STR) == 0) {
		int32_t ledCount;
		CHECK_PARAMS(0);

		retvalue = KEApiLedGetCount(&ledCount);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("LedCount count: %d\n", ledCount);
	} else if (strncasecmp(FunctName, "LedGetStatus", KEAPI_MAX_STR) == 0) {
		int32_t ledNr = 0;
		int32_t status;
		CHECK_PARAMS(1);
    
		ledNr = atol(argv[0]);
		retvalue = KEApiLedGetStatus(ledNr, &status);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("status: %d\n", status);
	} else if (strncasecmp(FunctName, "LedSetStatus", KEAPI_MAX_STR) == 0) {
		int32_t ledNr = 0;
		int32_t status = 0;
		CHECK_PARAMS(2);
    
		ledNr = atol(argv[0]);
		status = atol(argv[1]);
		retvalue = KEApiLedSetStatus(ledNr, status);

	} else if (strncasecmp(FunctName, "LedGetConfig", KEAPI_MAX_STR) == 0) {
		int32_t ledNr = 0;
		KEAPI_LED_CONFIG ledConfig;
		CHECK_PARAMS(1);
    
		ledNr = atol(argv[0]);
		retvalue = KEApiLedGetConfig(ledNr, &ledConfig);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("LedConfig: Colour:0x%X Light:0x%X mode:%d TOn:%d TOff:%d\n", 
                   ledConfig.Colour, ledConfig.Light, ledConfig.Mode, ledConfig.TOn, ledConfig.TOff);    
            
    } else if (strncasecmp(FunctName, "LedSetConfig", KEAPI_MAX_STR) == 0) {
		int32_t ledNr = 0;
		KEAPI_LED_CONFIG ledConfig;
		CHECK_PARAMS(6);
    
		ledNr = atol(argv[0]);
		ledConfig.Colour = atol(argv[1]);
		ledConfig.Light = atol(argv[2]);
		ledConfig.Mode = atol(argv[3]);
		ledConfig.TOn = atol(argv[4]);
		ledConfig.TOff = atol(argv[5]);
               
		retvalue = KEApiLedSetConfig (ledNr, ledConfig);
        
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
