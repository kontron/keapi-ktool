/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron Europe GmbH */

/* SMBus communications. */

#include "shell_tools.h"

const char smbus_usage[] = "\
    * GetSmbusCount\n\
    * SmbusQuickCommand <SMBus_number> <SMBus_address> <operation>\n\
     - operation: -w (write) or -r(read)\n\
    * SmbusSendByte <SMBus_number> <SMBus_address> <byte>\n\
    * SmbusReceiveByte <SMBus_number> <SMBus_address>\n\
    * SmbusWriteByte <SMBus_number> <SMBus_address> <command> <byte>\n\
    * SmbusReadByte <SMBus_number> <SMBus_address> <command>\n\
    * SmbusWriteWord <SMBus_number> <SMBus_address> <command> <word>\n\
    * SmbusReadWord <SMBus_number> <SMBus_address> <command>\n\
    * SmbusWriteBlock <SMBus_number> <SMBus_address> <command> <block_of_data>\n\
    * SmbusReadBlock <SMBus_number> <SMBus_address> <command> \n\n\
    Examples: \n\
    - SmbusSendByte 0 0x54 0x4B\n\
    - SmbusWriteByte 0 0x54 0 0x7F\n\
    - SmbusWriteWord 0 0x54 0x45 0xABCD\n\
    - Write string 'ABCDE' (5 byte length), address = 0x54, command = 0x12:\n\
    - SmbusWriteBlock 0 0x54 0x12 \"0x41 0x42 0x43 0x44 0x45\"\n";

int smbus_call(const char *FunctName, int argc, const char *argv[])
{
	uint32_t retvalue;

	if (strncasecmp(FunctName, "GetSmbusCount", KEAPI_MAX_STR) == 0) {
		int32_t smbus_count;
		CHECK_PARAMS(0);

		retvalue = KEApiGetSmbusCount(&smbus_count);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("SMBus count = %d\n", smbus_count);
	} else if (strncasecmp(FunctName, "SmbusQuickCommand", KEAPI_MAX_STR) == 0) {
		int32_t smbusNr = 0;
		uint8_t smbusAddress = 0;
		uint8_t operation =
			KEAPI_SMBUS_READ_OP; // KEAPI_SMBUS_READ_OP = 1, KEAPI_SMBUS_WRITE_OP = 0
		CHECK_PARAMS(3);

		smbusNr = atoi(argv[0]);
		smbusAddress = (uint8_t)strtol(argv[1], NULL, 16);

		if (strncasecmp(argv[2], "-w", KEAPI_MAX_STR) == 0)
			operation = 0;
		else if (strncasecmp(argv[2], "-r", KEAPI_MAX_STR) == 0)
			operation = 1;
		else {
			fprintf(stderr,
				"Wrong parameter for operation. Set either -r for 'read', or -w for 'write'");
			return -1;
		}

		retvalue = KEApiSmbusQuickCommand(smbusNr, smbusAddress, operation);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("SMBus number: %d\nSMBus address: 0x%02X\nOperation: %s\n", smbusNr,
			       smbusAddress, argv[2]);
	} else if (strncasecmp(FunctName, "SmbusSendByte", KEAPI_MAX_STR) == 0) {
		int32_t smbusNr;
		uint8_t smbusAddress;
		uint8_t byte;
		CHECK_PARAMS(3);

		smbusNr = atoi(argv[0]);
		smbusAddress = (uint8_t)strtol(argv[1], NULL, 16);
		byte = (uint8_t)strtol(argv[2], NULL, 16);

		retvalue = KEApiSmbusSendByte(smbusNr, smbusAddress, byte);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("Byte was sent.\nSMBus number: %d\nSMBus address: 0x%02X\nByte: 0x%02X\n",
			       smbusNr, smbusAddress, byte);
	} else if (strncasecmp(FunctName, "SmbusReceiveByte", KEAPI_MAX_STR) == 0) {
		int32_t smbusNr;
		uint8_t smbusAddress;
		uint8_t pByte;
		CHECK_PARAMS(2);

		smbusNr = atoi(argv[0]);
		smbusAddress = (uint8_t)strtol(argv[1], NULL, 16);
		retvalue = KEApiSmbusReceiveByte(smbusNr, smbusAddress, &pByte);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("SMBus number: %d\nSMBus address: 0x%02X\nReceived byte: 0x%02X\n",
			       smbusNr, smbusAddress, pByte);
	} else if (strncasecmp(FunctName, "SmbusWriteByte", KEAPI_MAX_STR) == 0) {
		int32_t smbusNr;
		uint8_t smbusAddress;
		uint8_t command;
		uint8_t byte;
		CHECK_PARAMS(4);

		smbusNr = atoi(argv[0]);
		smbusAddress = (uint8_t)strtol(argv[1], NULL, 16);
		command = (uint8_t)strtol(argv[2], NULL, 16);
		byte = (uint8_t)strtol(argv[3], NULL, 16);

		retvalue = KEApiSmbusWriteByte(smbusNr, smbusAddress, command, byte);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("SMBus number: %d\nSMBus address: 0x%02X\nCommand: 0x%02X\nWritten byte: 0x%02X\n",
			       smbusNr, smbusAddress, command, byte);
	} else if (strncasecmp(FunctName, "SmbusReadByte", KEAPI_MAX_STR) == 0) {
		int32_t smbusNr;
		uint8_t smbusAddress;
		uint8_t command;
		uint8_t pByte;
		CHECK_PARAMS(3);

		smbusNr = atoi(argv[0]);
		smbusAddress = (uint8_t)strtol(argv[1], NULL, 16);
		command = (uint8_t)strtol(argv[2], NULL, 16);

		retvalue = KEApiSmbusReadByte(smbusNr, smbusAddress, command, &pByte);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("SMBus number: %d\nSMBus address: 0x%02X\nCommand: 0x%02X\nByte read: 0x%02X\n",
			       smbusNr, smbusAddress, command, pByte);
	} else if (strncasecmp(FunctName, "SmbusWriteWord", KEAPI_MAX_STR) == 0) {
		int32_t smbusNr;
		uint8_t smbusAddress;
		uint8_t command;
		uint16_t word;
		CHECK_PARAMS(4);

		smbusNr = atoi(argv[0]);
		smbusAddress = (uint8_t)strtol(argv[1], NULL, 16);
		command = (uint8_t)strtol(argv[2], NULL, 16);
		word = (uint16_t)strtol(argv[3], NULL, 16);

		retvalue = KEApiSmbusWriteWord(smbusNr, smbusAddress, command, word);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("Requested word was written.\nSMBus number: %d\nSMBus address: 0x%02X\nCommand: 0x%02X\nWritten word: 0x%04X\n",
			       smbusNr, smbusAddress, command, word);
	} else if (strncasecmp(FunctName, "SmbusReadWord", KEAPI_MAX_STR) == 0) {
		int32_t smbusNr;
		uint8_t smbusAddress;
		uint8_t command;
		uint16_t pWord;
		CHECK_PARAMS(3);

		smbusNr = atoi(argv[0]);
		smbusAddress = (uint8_t)strtol(argv[1], NULL, 16);
		command = (uint8_t)strtol(argv[2], NULL, 16);

		retvalue = KEApiSmbusReadWord(smbusNr, smbusAddress, command, &pWord);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("SMBus number: %d\nSMBus address: 0x%02X\nCommand: 0x%02X\nRead word: 0x%04X\n",
			       smbusNr, smbusAddress, command, pWord);
	} else if (strncasecmp(FunctName, "SmbusWriteBlock", KEAPI_MAX_STR) == 0) {
		int32_t smbusNr;
		uint8_t smbusAddress;
		uint8_t command;
		uint8_t *pData;
		uint8_t dataInput[DATA_BUFFER_SIZE], dataSave[DATA_BUFFER_SIZE];
		uint8_t dataLength = 0;
		char *pch;
		int i = 0;
		CHECK_PARAMS(4);

		smbusNr = atoi(argv[0]);
		smbusAddress = (uint8_t)strtol(argv[1], NULL, 16);
		command = (uint8_t)strtol(argv[2], NULL, 16);
		strncpy((char *)dataInput, argv[3], DATA_BUFFER_SIZE);
		strncpy((char *)dataSave, argv[3], DATA_BUFFER_SIZE);

		pch = strtok((char *)dataInput, " ");
		while (pch != NULL) {
			strtol(pch, NULL, 16);
			pch = strtok(NULL, " ");
			dataLength++;
		}

		pData = (uint8_t *)malloc(dataLength);

		if (pData == NULL) {
			fprintf(stderr, "ktool: data allocation error\n");
			return -1;
		}

		strncpy((char *)dataInput, (char *)dataSave, DATA_BUFFER_SIZE);
		pch = strtok((char *)dataInput, " ");

		while (pch != NULL) {
			pData[i] = (uint8_t)strtol(pch, NULL, 16);
			pch = strtok(NULL, " ");
			i++;
		}

		retvalue = KEApiSmbusWriteBlock(smbusNr, smbusAddress, command, pData, i);
		if (retvalue == KEAPI_RET_SUCCESS) {
			printf("SMBus number: %d\nSMBus address: 0x%02X\nCommand: 0x%02X\nRequested data to be written: \n",
			       smbusNr, smbusAddress, command);

			for (i = 0; i < dataLength; i++)
				printf("0x%02X ", pData[i]);

			printf("\nData length: %d\n", dataLength);
		}
		free(pData);
	} else if (strncasecmp(FunctName, "SmbusReadBlock", KEAPI_MAX_STR) == 0) {
		int32_t smbusNr;
		uint8_t smbusAddress;
		uint8_t command;
		uint8_t *pData;
		int i;
		// A Block Read or Write is allowed to transfer a maximum of 32 data bytes.
		int8_t dataLength = 32;

		CHECK_PARAMS(3);

		smbusNr = atoi(argv[0]);
		smbusAddress = (uint8_t)strtol(argv[1], NULL, 16);
		command = (uint8_t)strtol(argv[2], NULL, 16);

		pData = (uint8_t *)malloc(dataLength * sizeof(uint8_t));

		if (pData == NULL) {
			fprintf(stderr, "ktool: data allocation error\n");
			return -1;
		}

		retvalue = KEApiSmbusReadBlock(smbusNr, smbusAddress, command, pData, &dataLength);

		if (retvalue == KEAPI_RET_SUCCESS) {
			printf("SMBus number: %d\nSMBus address: 0x%02X\nCommand: 0x%02X\nData read: \n",
			       smbusNr, smbusAddress, command);

			for (i = 0; i < dataLength; i++)
				printf("'%c'(0x%02X) ", pData[i] < 32 ? '.' : pData[i],
				       (unsigned int)(pData[i]));

			printf("\nData length: %d\n", dataLength);
		}
		free(pData);
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
