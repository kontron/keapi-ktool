/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron Europe GmbH */

/* Handling EEPROM storage. */

#include "shell_tools.h"
#include "limits.h"
#include "errno.h"

const char eeprom_usage[] = "\
    * GetStorageCount\n\
    * GetStorageSize <storage_number>\n\
    * StorageRead <storage_number> <offset> <data_length>\n\
    * StorageWrite <storage_number> <offset> <data>\n\
    <storage_number> can be transmitted as Dec(123) number.\n\
    <offset> can be transmitted as Dec(123) or Hex(0x123) number.\n\
    <data> can be transmitted as:\n\
    \t- text data in quotes and prefix 's' (s\"text data\"),\n\
    \t- Hex and Dec numbers in quotes (\"0x30 30 0x56 10\").\n\n\
    Examples: \n\
    Write string 'ABCDE' to EEPROM 0, offset 0x18:\n\
    \tStorageWrite 0 0x18 \"0x41 0x42 0x43 0x44 0x45\"\n\
    Read 5 bytes from EEPROM 0, offset 0x18:\n\
    \tStorageRead 0 0x18 5\n\n\
	\n";

/* return 0 if converted successfully */
static int convert_int32(const char *arg, int32_t *value)
{
	char *end;
/* WEC has no errno, alas... */
#ifdef UNDER_CE
	*value = strtol(arg, &end, 0);
	if (end == arg || *end != '\0') {
		return -1;
	}
#else
	errno = 0;
	*value = strtol(arg, &end, 0);
	if ((errno == ERANGE && (*value == LONG_MAX || *value == LONG_MIN)) ||
	    (errno != 0 && *value == 0) || (end == arg) || (*end != '\0')) {
		return -1;
	}
#endif
	return 0;
}

int eeprom_call(const char *FunctName, int argc, const char *argv[])
{
	uint32_t retvalue;

	if (strncasecmp(FunctName, "GetStorageCount", KEAPI_MAX_STR) == 0) {
		int32_t StorageCount;
		CHECK_PARAMS(0);

		retvalue = KEApiGetStorageCount(&StorageCount);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("Storage count: %d\n", StorageCount);
	} else if (strncasecmp(FunctName, "GetStorageSize", KEAPI_MAX_STR) == 0) {
		int32_t storageNr = 0;
		int32_t storageSize;
		CHECK_PARAMS(1);

		storageNr = atol(argv[0]);
		retvalue = KEApiGetStorageSize(storageNr, &storageSize);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("Storage size: %d\n", storageSize);
	} else if (strncasecmp(FunctName, "StorageRead", KEAPI_MAX_STR) == 0) {
		int32_t storageNr, offset, dataLength, i;
		uint8_t *pData;
		CHECK_PARAMS(3);

		storageNr = atol(argv[0]);

		if (convert_int32(argv[1], &offset)) {
			fprintf(stderr, "Error: Wrong offset parameter '%s'\n", argv[1]);
			return -1;
		}

		dataLength = atol(argv[2]);

		pData = (uint8_t *)malloc(dataLength * sizeof(uint8_t));

		if (pData == NULL) {
			fprintf(stderr, "ktool: data allocation error\n");
			return -1;
		}

		retvalue = KEApiStorageRead(storageNr, offset, pData, dataLength);

		if (retvalue == KEAPI_RET_SUCCESS) {
			for (i = 0; i < dataLength; i++)
				printf("'%c'(0x%02X) ", pData[i] < 32 ? '.' : pData[i],
				       (unsigned int)(pData[i]));
		}

		printf("\n\n");
		free(pData);
	} else if (strncasecmp(FunctName, "StorageWrite", KEAPI_MAX_STR) == 0) {
		int32_t storageNr, offset, dataLength = 0;
		uint8_t *pData;
		uint8_t *dataInput;
		uint8_t *dataSave;
		char *pch;
		int i = 0;
		CHECK_PARAMS(3);

		storageNr = atol(argv[0]);

		if (convert_int32(argv[1], &offset)) {
			fprintf(stderr, "Error: Wrong offset parameter '%s'\n", argv[1]);
			return -1;
		}

		/* plain text in quotes: s"data" */
		if (argv[2][0] == 's') {
			dataLength = (int32_t)strlen(argv[2]) - 1;
			if (dataLength < 1) {
				fprintf(stderr, "Error: Wrong data\n");
				return -1;
			}

			pData = (uint8_t *)malloc(dataLength);

			if (pData == NULL) {
				fprintf(stderr, "ktool: data allocation error\n");
				return -1;
			}

			memcpy(pData, &argv[2][1], dataLength);

			retvalue = KEApiStorageWrite(storageNr, offset, pData, dataLength);

			/* otherwise Hex or Dec numbers in quotes: "0x30 48 50 0x31" */
		} else {
			int32_t bufVar, buflen;

			buflen = (int32_t)strlen(argv[2]);
			dataInput = (uint8_t *)malloc(buflen);
			if (dataInput == NULL) {
				fprintf(stderr, "ktool: data allocation error\n");
				return -1;
			}
			dataSave = (uint8_t *)malloc(buflen);
			if (dataSave == NULL) {
				fprintf(stderr, "ktool: data allocation error\n");
				free(dataInput);
				return -1;
			}

			strncpy((char *)dataInput, argv[2], buflen);
			strncpy((char *)dataSave, argv[2], buflen);

			pch = strtok((char *)dataInput, " ");
			while (pch != NULL) {
				pch = strtok(NULL, " ");
				dataLength++;
			}

			pData = (uint8_t *)malloc(dataLength);

			if (pData == NULL) {
				fprintf(stderr, "ktool: data allocation error\n");
				free(dataInput);
				free(dataSave);
				return -1;
			}

			strncpy((char *)dataInput, (char *)dataSave, buflen);
			pch = strtok((char *)dataInput, " ");

			while (pch != NULL) {
				if (convert_int32(pch, &bufVar) || bufVar < 0 || bufVar > 0xFF) {
					fprintf(stderr, "Error: Error: Wrong data '%s'\n", pch);
					free(pData);
					free(dataInput);
					free(dataSave);
					return -1;
				}
				pData[i] = (uint8_t)bufVar;
				pch = strtok(NULL, " ");
				i++;
			}

			retvalue = KEApiStorageWrite(storageNr, offset, pData, i);
			free(dataInput);
			free(dataSave);
		}

		free(pData);

		if (retvalue == KEAPI_RET_SUCCESS)
			printf("Written.\n");
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
