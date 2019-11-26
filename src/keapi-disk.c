/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* Information about the disk system attached to the board. */

#include "shell_tools.h"

const char disk_usage[] = "\
  * GetDiskDriveCount\n\
  * GetDiskDriveList\n\
  * GetMountPointCount\n\
  * GetMountPointList\n\
  * GetDiskDriveSMARTAttrCount <disk_number>\n\
  * GetDiskDriveSMARTAttrs <disk_number>\n";

int disk_call(const char *FunctName, int argc, const char *argv[])
{
	uint32_t retvalue;

	if (strncasecmp(FunctName, "GetDiskDriveCount", KEAPI_MAX_STR) == 0) {
		int32_t hdd_count = 0;
		CHECK_PARAMS(0);

		retvalue = KEApiGetDiskDriveCount(&hdd_count);

		if (retvalue == KEAPI_RET_SUCCESS)
			printf("Disk drive count: %d\n", hdd_count);
	} else if (strncasecmp(FunctName, "GetDiskDriveList", KEAPI_MAX_STR) == 0) {
		int32_t hdds = 0;
		PKEAPI_DISK_DRIVE pDiskDrives = NULL;
		CHECK_PARAMS(0);

		retvalue = KEApiGetDiskDriveCount(&hdds);
		if (retvalue == KEAPI_RET_SUCCESS) {
			if (hdds <= 0) {
				printf("no disks\n");
				return 0;
			}

			pDiskDrives = (PKEAPI_DISK_DRIVE)malloc(sizeof(KEAPI_DISK_DRIVE) * hdds);

			if (pDiskDrives == NULL) {
				fprintf(stderr, "ktool: data allocation error\n");
				return -1;
			}

			retvalue = KEApiGetDiskDriveList(pDiskDrives, hdds);
			if (retvalue == KEAPI_RET_SUCCESS) {
				int32_t i;
				for (i = 0; i < hdds; i++) {
					print_string("Disk name:", pDiskDrives[i].name);
					printf("Disk size: %d MB\n", pDiskDrives[i].size);
					print_string("Model    :", pDiskDrives[i].model);
					print_string("Serial nr:", pDiskDrives[i].diskSerialNumber);
					printf("\n");
				}
			}

			free(pDiskDrives);
		} else {
			keapi_perror("KEApiGetDiskDriveCount failed", retvalue);
		}
	} else if (strncasecmp(FunctName, "GetMountPointCount", KEAPI_MAX_STR) == 0) {
		int32_t hdd_partition_count = 0;
		CHECK_PARAMS(0);

		retvalue = KEApiGetMountPointCount(&hdd_partition_count);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("Mount point count: %d\n", hdd_partition_count);
		else
			keapi_perror("KEApiGetMountPointCount failed", retvalue);

	} else if (strncasecmp(FunctName, "GetMountPointList", KEAPI_MAX_STR) == 0) {
		int32_t partitions = 0;
		PKEAPI_MOUNT_POINT hdd_mpoint_list = NULL;
		CHECK_PARAMS(0);

		if ((retvalue = KEApiGetMountPointCount(&partitions)) == KEAPI_RET_SUCCESS) {
			if (partitions <= 0) {
				printf("no mount points\n");
				return 0;
			}
			hdd_mpoint_list =
				(PKEAPI_MOUNT_POINT)malloc(sizeof(KEAPI_MOUNT_POINT) * partitions);

			if (hdd_mpoint_list == NULL) {
				fprintf(stderr, "ktool: data allocation error\n");
				return -1;
			}

			if ((retvalue = KEApiGetMountPointList(hdd_mpoint_list, partitions)) ==
			    KEAPI_RET_SUCCESS) {
				int32_t i;
				for (i = 0; i < partitions; i++) {
					print_string("Mount point name:", hdd_mpoint_list[i].name);
					printf("Mount point size: %d MB\n",
					       hdd_mpoint_list[i].size);
					printf("Free space      : %d MB\n",
					       hdd_mpoint_list[i].freeSpace);
					print_string("Filesystem type :",
						     hdd_mpoint_list[i].fsType);
					printf("\n");
				}
			}
			free(hdd_mpoint_list);
		}
	} else if (strncasecmp(FunctName, "GetDiskDriveSMARTAttrCount", KEAPI_MAX_STR) == 0) {
		int32_t diskNr;
		int32_t attrCount = 0;
		CHECK_PARAMS(1);
		diskNr = atoi(argv[0]);

		if ((retvalue = KEApiGetDiskDriveSMARTAttrCount(diskNr, &attrCount)) ==
		    KEAPI_RET_SUCCESS) {
			printf("Disk SMART attributes count: %d\n", attrCount);
		}
	} else if (strncasecmp(FunctName, "GetDiskDriveSMARTAttrs", KEAPI_MAX_STR) == 0) {
		int32_t diskNr;
		int32_t attrCount = 0;
		PKEAPI_SMART_ATTR pAttrs;
		CHECK_PARAMS(1);
		diskNr = atoi(argv[0]);

		if ((retvalue = KEApiGetDiskDriveSMARTAttrCount(diskNr, &attrCount)) ==
		    KEAPI_RET_SUCCESS) {
			if (attrCount <= 0) {
				printf("no attrs\n");
			} else {
				pAttrs = (PKEAPI_SMART_ATTR)malloc(sizeof(KEAPI_SMART_ATTR) * attrCount);

				if (pAttrs == NULL) {
					fprintf(stderr, "ktool: data allocation error\n");
					return -1;
				}

				if ((retvalue = KEApiGetDiskDriveSMARTAttrs(diskNr, pAttrs, attrCount)) == KEAPI_RET_SUCCESS) {
					int32_t i;
					for (i = 0; i < attrCount; i++) {
						printf("Attribute ID   : %u\n", pAttrs[i].attrID);
						printf("Status Flags   : 0x%x\n",
						       pAttrs[i].statusFlags);
						printf("Attribute Value: %u\n",
						       pAttrs[i].attrValue);
						printf("Worst Value    : %u\n",
						       pAttrs[i].worstValue);
						printf("Raw Value      : 0x%02x%02x%02x%02x%02x%02x\n\n",
						       pAttrs[i].rawValue[0], pAttrs[i].rawValue[1],
						       pAttrs[i].rawValue[2], pAttrs[i].rawValue[3],
						       pAttrs[i].rawValue[4],
						       pAttrs[i].rawValue[5]);
					}
				}
				free(pAttrs);
			}
		} else {
			keapi_perror("KEApiGetDiskDriveSMARTAttrCount failed", retvalue);
		}
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
