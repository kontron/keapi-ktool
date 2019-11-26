/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* SPI communications. */

#include "shell_tools.h"

const char spi_usage[] = "\
    * GetSpiBusCount\n\
    * SpiXfer <SPI_number> <SPI_devId> <command> <command_size_in_bytes> <data_to_be_written> <io_data_length>\n\n\
    Examples: \n\
    Send \"write enable\" command to SPI flash on SPI1, chip select 0, command 0x6: \n\
    \tSpiXfer 1 0 0x06 1 \"\" 0\n\
    Read status register from SPI flash on SPI1, chip select 0, command 0x5: \n\
    \tSpiXfer 1 0 0x05 1 \"\" 1\n\
    Write string 'ABCDE' to SPI flash on SPI1, chip select  0, command 0x02000400 (offset=0x400):\n\
    \tSpiXfer 1 0 0x02000400 4 \"0x41 0x42 0x43 0x44 0x45\" 5\n\
    Read 5 bytes from SPI flash on SPI1, chip select  0, command 0x03000400 (offset=0x400):\n\
    \tSpiXfer 1 0 0x03000400 4 \"\" 5\n\
    \n";

int spi_call(const char *FunctName, int argc, const char *argv[])
{
	uint32_t retvalue;

	uint8_t spiNr = 0;
	uint16_t deviceId = 0;
	uint32_t command = 0;
	uint8_t commandSize = 0;
	uint32_t numTransfers = 0;

	uint8_t *pWriteData = NULL, *pWr = NULL;
	int32_t writeLength = 0;

	uint8_t *pReadData = NULL, *pRd = NULL;

	uint32_t i = 0;
	char *pch;
	uint8_t dataIn[KEAPI_MAX_STR], dataSave[KEAPI_MAX_STR];

	if (strncasecmp(FunctName, "GetSpiBusCount", KEAPI_MAX_STR) == 0) {
		int32_t spi_count;
		CHECK_PARAMS(0);

		retvalue = KEApiGetSpiBusCount(&spi_count);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("SPI bus count = %d\n", spi_count);

	} else if (strncasecmp(FunctName, "SpiXfer", KEAPI_MAX_STR) == 0) {
		CHECK_PARAMS(6);
		spiNr = atoi(argv[0]);
		deviceId = (uint16_t)atoi(argv[1]);
		command = (uint32_t)strtoul(argv[2], NULL, 16);
		commandSize = atoi(argv[3]);

		strncpy((char *)dataIn, argv[4], KEAPI_MAX_STR);
		strncpy((char *)dataSave, argv[4], KEAPI_MAX_STR);

		numTransfers = atoi(argv[5]);

		pch = strtok((char *)dataIn, " ");
		while (pch != NULL) {
			strtol(pch, NULL, 16);
			pch = strtok(NULL, " ");
			writeLength++;
		}

		if (writeLength) {
			pWr = pWriteData = (uint8_t *)calloc(numTransfers, 1);
			if (pWriteData == NULL) {
				keapi_perror("returned", KEAPI_RET_MALLOC_ERROR);
				return -1;
			}

			strncpy((char *)dataIn, (char *)dataSave, KEAPI_MAX_STR);
			pch = strtok((char *)dataIn, " ");

			while (pch != NULL && i < numTransfers) {
				pWriteData[i] = (uint8_t)strtol(pch, NULL, 16);
				pch = strtok(NULL, " ");
				i++;
			}
		}

		pRd = pReadData = (uint8_t *)calloc(numTransfers, 1);
		if (pReadData == NULL) {
			free(pWr);
			keapi_perror("returned", KEAPI_RET_MALLOC_ERROR);
			return -1;
		}

		retvalue = KEApiSpiXfer(spiNr, deviceId, command, commandSize, numTransfers,
					pWriteData, pReadData);

		if (retvalue == KEAPI_RET_SUCCESS) {
			printf(" SPI number: %d\n chip select: %d \n command 0x%X\n", spiNr,
			       deviceId, command);

			if (numTransfers) {
				printf("\nData was written:\n");

				for (i = 0; i < numTransfers; i++) {
					if (pWriteData)
						printf("'%c'(0x%02X) ", pWriteData[i],
						       (unsigned int)(pWriteData[i]));
					else
						printf("'--'(--) ");
				}

				printf("\nData was read:\n");

				for (i = 0; i < numTransfers; i++) {
					printf("'%c'(0x%02X) ", pReadData[i],
					       (unsigned int)(pReadData[i]));
				}
			}
			printf("\n");
		}

		free(pRd);
		if (pWr != NULL)
			free(pWr);

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
