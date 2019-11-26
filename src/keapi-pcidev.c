/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* PCI devices information. */

#include "shell_tools.h"

const char pcidev_usage[] = "\
    * GetPciDeviceCount\n\
    * GetPciDeviceList \n";

int pcidev_call(const char *FunctName, int argc, const char *argv[])
{
	uint32_t retvalue;

	if (strncasecmp(FunctName, "GetPciDeviceCount", KEAPI_MAX_STR) == 0) {
		int32_t device_count = 0;
		CHECK_PARAMS(0);

		retvalue = KEApiGetPciDeviceCount(&device_count);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("PCI Device Count: %d\n", device_count);
	} else if (strncasecmp(FunctName, "GetPciDeviceList", KEAPI_MAX_STR) == 0) {
		int32_t pci_devices = 0;
		PKEAPI_PCI_DEVICE pci_device_list = NULL;
		CHECK_PARAMS(0);

		if ((retvalue = KEApiGetPciDeviceCount(&pci_devices)) == KEAPI_RET_SUCCESS) {
			if (pci_devices == 0)
				printf("No PCI devices present.\n");
			else {
				pci_device_list = (PKEAPI_PCI_DEVICE)malloc(
					sizeof(KEAPI_PCI_DEVICE) * pci_devices);

				if (pci_device_list == NULL) {
					fprintf(stderr, "ktool: data allocation error\n");
					return -1;
				}

				retvalue = KEApiGetPciDeviceList(pci_device_list, pci_devices);
				if (retvalue == KEAPI_RET_SUCCESS ||
				    retvalue == KEAPI_RET_PARTIAL_SUCCESS) {
					int32_t i;
					for (i = 0; i < pci_devices; i++) {
						printf("Domain     : %d\n",
						       pci_device_list[i].domain);
						printf("Bus        : %d\n",
						       pci_device_list[i].bus);
						printf("Slot       : %d\n",
						       pci_device_list[i].slot);
						printf("Function   : %d\n",
						       pci_device_list[i].funct);
						printf("Device ID  : 0x%X\n",
						       pci_device_list[i].deviceId);
						printf("Vendor ID  : 0x%X\n",
						       pci_device_list[i].vendorId);
						printf("Class ID   : 0x%X\n",
						       pci_device_list[i].classId);
						printf("Device name: %s\n",
						       pci_device_list[i].deviceName);
						printf("Vendor name: %s\n",
						       pci_device_list[i].vendorName);
						printf("Class name : %s\n\n",
						       pci_device_list[i].className);
					}
				}
				free(pci_device_list);
			}
		} else
			keapi_perror("KEApiGetPciDeviceCount", retvalue);
	} else {
		fprintf(stderr, "unknown api call: %s\n", FunctName);
		return -1;
	}

	if (retvalue)
		keapi_perror("returned", retvalue);
	else
		keapi_success("returned", retvalue);

	return 0;
}
