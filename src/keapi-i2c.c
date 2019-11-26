/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* I2C communication. */

#include "shell_tools.h"

const char i2c_usage[] = "\
    * GetI2cBusCount\n\
    * I2cProbe <I2C_number> <I2C_address> <memory_address> <memory_address_used>\n\
    * I2cXfer <I2C_number> <I2C_address> <data_to_be_written> <read_data_length>\n\n\
    Examples:\n\
    please note, these are general examples, not related to any board \n\
    all addresses and offsets are provided as an example \n\
    \tProbe device 0x50 on I2C-0 without memory_address_used:\n\
    \t\tI2cProbe 0 0x50 0 0\n\
    \tProbe device 0x50 on I2C-0 with memory_address_used, offset 0x23:\n\
    \t\tI2cProbe 0 0x50 0x23 1\n\
    \tRead 1 byte from device 0x60 on I2C-0:\n\
    \t\tI2cXfer 0 0x60 \"\" 1\n\
    \tWrite 1 byte \"0xAB\" to device 0x60 on I2C-0:\n\
    \t\tI2cXfer 0 0x60 0xAB 0\n\
    \tRead 5 bytes from EEPROM on I2C-0, address 0x57,\n\
    \toffset 0x38 (8-bit offset):\n\
    \t\tI2cXfer 0 0x57 0x38 5\n\
    \tWrite string 'ABCDE' to EEPROM on I2C-0, address 0x57,\n\
    \toffset 0x12 (8-bit offset):\n\
    \t\tI2cXfer 0 0x57 \"0x12 0x41 0x42 0x43 0x44 0x45\" 0\n\
    \tRead 12 bytes from EEPROM on I2C-0, address 0x54,\n\
    \toffset 0x130 (16-bit offset):\n\
    \t\tI2cXfer 0 0x54 \"0x01 0x30\" 12\n\
    \tWrite string 'ABC' to EEPROM on I2C-0, address 0x54,\n\
    \toffset 0x1525 (16-bit offset):\n\
    \t\tI2cXfer 0 0x54 \"0x15 0x25 0x41 0x42 0x43\" 0\n\
    \n";

int i2c_call(const char *FunctName, int argc, const char *argv[])
{
	uint32_t retvalue;

	uint8_t i2cNr = 0;
	uint8_t i2cAddress = 0;
	uint8_t memoryAddress = 0;
	int8_t memoryAddressUsed = 0;

	uint8_t *pWriteData = NULL;
	int32_t writeLength = 0;

	uint8_t *pReadData = NULL;
	int32_t readLength = 0;

	int32_t i = 0;
	char *pch;
	uint8_t dataIn[DATA_BUFFER_SIZE], dataSave[DATA_BUFFER_SIZE];

	if (strncasecmp(FunctName, "GetI2cBusCount", KEAPI_MAX_STR) == 0) {
		int32_t i2c_count;
		CHECK_PARAMS(0);

		retvalue = KEApiGetI2cBusCount(&i2c_count);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("I2C bus count = %d\n", i2c_count);

	} else if (strncasecmp(FunctName, "I2cXfer", KEAPI_MAX_STR) == 0) {
		CHECK_PARAMS(4);
		i2cNr = atoi(argv[0]);
		i2cAddress = (uint8_t)strtol(argv[1], NULL, 16);
		strncpy((char *)dataIn, argv[2], DATA_BUFFER_SIZE);
		strncpy((char *)dataSave, argv[2], DATA_BUFFER_SIZE);

		pch = strtok((char *)dataIn, " ");
		while (pch != NULL) {
			strtol(pch, NULL, 16);
			pch = strtok(NULL, " ");
			writeLength++;
		}

		if (writeLength != 0) {
			pWriteData = (uint8_t *)malloc(writeLength);

			if (pWriteData == NULL) {
				fprintf(stderr, "ktool: data allocation error\n");
				return -1;
			}

			strncpy((char *)dataIn, (char *)dataSave, DATA_BUFFER_SIZE);
			pch = strtok((char *)dataIn, " ");

			while (pch != NULL) {
				pWriteData[i] = (uint8_t)strtol(pch, NULL, 16);
				pch = strtok(NULL, " ");
				i++;
			}
		} else
			pWriteData = NULL;

		readLength = atoi(argv[3]);

		if (readLength != 0) {
			pReadData = (uint8_t *)malloc(readLength);

			if (pReadData == NULL) {
				fprintf(stderr, "ktool: data allocation error\n");

				if (pWriteData != NULL)
					free(pWriteData);

				return -1;
			}

			memset(pReadData, '\0', readLength);
		} else
			pReadData = NULL;

		retvalue = KEApiI2cXfer(i2cNr, i2cAddress, pWriteData, writeLength, pReadData,
					&readLength);

		if (retvalue == KEAPI_RET_SUCCESS) {
			printf("I2C number = %d, I2C address = 0x%X", i2cNr, i2cAddress);

			if (writeLength) {
				printf("\nData was written:\n");
				for (i = 0; i < writeLength; i++)
					printf("'%c'(0x%02X) ", pWriteData[i], pWriteData[i]);
			}

			if (readLength) {
				printf("\nData was read:\n");
				for (i = 0; i < readLength; i++)
					printf("'%c'(0x%02X) ", pReadData[i], pReadData[i]);
			}
		}

		printf("\n\n");

		if (pWriteData != NULL)
			free(pWriteData);

		if (pReadData != NULL)
			free(pReadData);

	} else if (strncasecmp(FunctName, "I2cProbe", KEAPI_MAX_STR) == 0) {
		CHECK_PARAMS(4);

		i2cNr = atoi(argv[0]);
		i2cAddress = (uint8_t)strtol(argv[1], NULL, 16);
		memoryAddress = (uint8_t)strtol(argv[2], NULL, 16);
		memoryAddressUsed = atoi(argv[3]);

		retvalue = KEApiI2cProbe(i2cNr, i2cAddress, memoryAddress, memoryAddressUsed);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("I2C number: %d\nI2C address: 0x%X\n Device was found!\n", i2cNr,
			       i2cAddress);

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
