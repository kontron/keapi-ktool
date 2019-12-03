/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron Europe GmbH */

/* Network information. */

#include "shell_tools.h"

const char netdev_usage[] = "\
    * GetNetworkDeviceCount\n\
    * GetNetworkDeviceList\n";

int netdev_call(const char *FunctName, int argc, const char *argv[])
{
	uint32_t retvalue;

	if (strncasecmp(FunctName, "GetNetworkDeviceCount", KEAPI_MAX_STR) == 0) {
		int32_t device_count = 0;
		CHECK_PARAMS(0);

		retvalue = KEApiGetNetworkDeviceCount(&device_count);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("Device Count: %d\n", device_count);
	} else if (strncasecmp(FunctName, "GetNetworkDeviceList", KEAPI_MAX_STR) == 0) {
		int32_t network_devices_count = 0;
		PKEAPI_NETWORK_DEVICE network_device_list = NULL;
		CHECK_PARAMS(0);

		if ((retvalue = KEApiGetNetworkDeviceCount(&network_devices_count)) ==
		    KEAPI_RET_SUCCESS) {
			if (network_devices_count == 0)
				printf("No network device present.\n");
			else {
				int32_t i;
				network_device_list = (PKEAPI_NETWORK_DEVICE)malloc(
					sizeof(KEAPI_NETWORK_DEVICE) * network_devices_count);

				if (network_device_list == NULL) {
					fprintf(stderr, "ktool: data allocation error\n");
					return -1;
				}

				retvalue = KEApiGetNetworkDeviceList(network_device_list,
								     network_devices_count);
				if (retvalue == KEAPI_RET_SUCCESS ||
				    retvalue == KEAPI_RET_PARTIAL_SUCCESS) {
					for (i = 0; i < network_devices_count; i++) {
						printf("Device Name: %s\n",
						       network_device_list[i].deviceName);
						printf("IP         : %s\n",
						       network_device_list[i].ip);
						printf("MAC        : %s\n",
						       network_device_list[i].mac);
						printf("Speed      : %d MB/s\n\n",
						       network_device_list[i].speed);
					}
				}
				free(network_device_list);
			}
		} else
			keapi_perror("KEApiGetNetworkDeviceCount failed", retvalue);
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
