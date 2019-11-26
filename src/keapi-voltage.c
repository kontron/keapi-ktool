/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2019 Kontron AG */

/* Voltage sensors information. */

#include "shell_tools.h"

const char voltage_usage[] = "\
    * GetVoltageSensorCount\n\
    * GetVoltageSensorValue <voltage_sensor_number>\n\
    * GetVoltageSensorValueList\n\
    * GetVoltageSensorInfo <voltage_sensor_number>\n\
    \t** Description of sensor types:\n\
    \t- 1 - VCORE\n\
    \t- 2 - 1V8\n\
    \t- 3 - 2V5\n\
    \t- 4 - 3V3\n\
    \t- 5 - VBAT\n\
    \t- 6 - 5V\n\
    \t- 7 - 5VSB\n\
    \t- 8 - 12V\n\
    \t- 9 - AC\n\
    \t- 10 - OTHER\n";

int voltage_call(const char *FunctName, int argc, const char *argv[])
{
	uint32_t retvalue;

	if (strncasecmp(FunctName, "GetVoltageSensorCount", KEAPI_MAX_STR) == 0) {
		int32_t voltage_sensor_count = 0;
		CHECK_PARAMS(0);

		retvalue = KEApiGetVoltageSensorCount(&voltage_sensor_count);
		if (retvalue == KEAPI_RET_SUCCESS)
			printf("Voltage sensor count: %d\n", voltage_sensor_count);

	} else if (strncasecmp(FunctName, "GetVoltageSensorValue", KEAPI_MAX_STR) == 0) {
		KEAPI_SENSOR_VALUE sensor_value = { 0 };
		int32_t sensor_number = 0;
		CHECK_PARAMS(1);

		sensor_number = atoi(argv[0]);
		retvalue = KEApiGetVoltageSensorValue(sensor_number, &sensor_value);
		if (retvalue == KEAPI_RET_SUCCESS) {
			printf("Sensor number:%d\nSensor value: %d\n", sensor_number,
			       sensor_value.value);

			printf("Sensor status: %d\n", sensor_value.status);

			if (sensor_value.status >
			    (KEAPI_SENSOR_STATUS_ACTIVE | KEAPI_SENSOR_STATUS_ALARM |
			     KEAPI_SENSOR_STATUS_BROKEN | KEAPI_SENSOR_STATUS_SHORTCIRCUIT))
				printf("Sensor status: error! Unknown sensor status\n\n");
			else {
				if (sensor_value.status & KEAPI_SENSOR_STATUS_ACTIVE)
					printf("(KEAPI_SENSOR_STATUS_ACTIVE)\n");

				if (sensor_value.status & KEAPI_SENSOR_STATUS_ALARM)
					printf("(KEAPI_SENSOR_STATUS_ALARM)\n");

				if (sensor_value.status & KEAPI_SENSOR_STATUS_BROKEN)
					printf("(KEAPI_SENSOR_STATUS_BROKEN)\n");

				if (sensor_value.status & KEAPI_SENSOR_STATUS_SHORTCIRCUIT)
					printf("(KEAPI_SENSOR_STATUS_SHORTCIRCUIT)\n");

				printf("\n");
			}
		}

	} else if (strncasecmp(FunctName, "GetVoltageSensorValueList", KEAPI_MAX_STR) == 0) {
		PKEAPI_SENSOR_VALUE psensor_values = NULL;
		int32_t sensor_count = 0;
		CHECK_PARAMS(0);

		if ((retvalue = KEApiGetVoltageSensorCount(&sensor_count)) == KEAPI_RET_SUCCESS) {
			psensor_values = (PKEAPI_SENSOR_VALUE)malloc(sizeof(KEAPI_SENSOR_VALUE) *
								     sensor_count);

			if (psensor_values == NULL) {
				fprintf(stderr, "ktool: data allocation error\n");
				return -1;
			}

			if ((retvalue = KEApiGetVoltageSensorValueList(
				     psensor_values, sensor_count)) == KEAPI_RET_SUCCESS) {
				int32_t i;
				for (i = 0; i < sensor_count; i++) {
					printf("Sensor number:%d\nSensor value: %d\n", i,
					       psensor_values[i].value);

					printf("Sensor status: %d\n", psensor_values[i].status);

					if (psensor_values[i].status >
					    (KEAPI_SENSOR_STATUS_ACTIVE |
					     KEAPI_SENSOR_STATUS_ALARM |
					     KEAPI_SENSOR_STATUS_BROKEN |
					     KEAPI_SENSOR_STATUS_SHORTCIRCUIT))
						printf("Sensor status: error! Unknown sensor status\n\n");
					else {
						if (psensor_values[i].status &
						    KEAPI_SENSOR_STATUS_ACTIVE)
							printf("(KEAPI_SENSOR_STATUS_ACTIVE)\n");

						if (psensor_values[i].status &
						    KEAPI_SENSOR_STATUS_ALARM)
							printf("(KEAPI_SENSOR_STATUS_ALARM)\n");

						if (psensor_values[i].status &
						    KEAPI_SENSOR_STATUS_BROKEN)
							printf("(KEAPI_SENSOR_STATUS_BROKEN)\n");

						if (psensor_values[i].status &
						    KEAPI_SENSOR_STATUS_SHORTCIRCUIT)
							printf("(KEAPI_SENSOR_STATUS_SHORTCIRCUIT)\n");

						printf("\n");
					}
				}
			}

			free(psensor_values);
		} else
			keapi_perror("KEApiGetVoltageSensorCount failed", retvalue);

	} else if (strncasecmp(FunctName, "GetVoltageSensorInfo", KEAPI_MAX_STR) == 0) {
		KEAPI_SENSOR_INFO sensor_info = { { 0 } };
		int32_t sensor_number = 0;
		CHECK_PARAMS(1);

		sensor_number = atoi(argv[0]);
		retvalue = KEApiGetVoltageSensorInfo(sensor_number, &sensor_info);
		if (retvalue == KEAPI_RET_SUCCESS) {
			printf("Sensor number: %d\n", sensor_number);

			printf("name: %s\n", sensor_info.name);

			if (sensor_info.type == KEAPI_SENSOR_INFO_UNKNOWN)
				printf("type: unknown (%d)\n", sensor_info.type);
			else
				printf("type: %d\n", sensor_info.type);

			if (sensor_info.max == KEAPI_SENSOR_INFO_UNKNOWN)
				printf("max: unknown (%d)\n", sensor_info.max);
			else
				printf("max: %d\n", sensor_info.max);

			if (sensor_info.min == KEAPI_SENSOR_INFO_UNKNOWN)
				printf("min: unknown (%d)\n", sensor_info.min);
			else
				printf("min: %d\n", sensor_info.min);

			if (sensor_info.alarmHi == KEAPI_SENSOR_INFO_UNKNOWN)
				printf("alarmHi: unknown (%d)\n", sensor_info.alarmHi);
			else
				printf("alarmHi: %d\n", sensor_info.alarmHi);

			if (sensor_info.alarmLo == KEAPI_SENSOR_INFO_UNKNOWN)
				printf("alarmLo: unknown (%d)\n", sensor_info.alarmLo);
			else
				printf("alarmLo: %d\n", sensor_info.alarmLo);

			if (sensor_info.hystHi == KEAPI_SENSOR_INFO_UNKNOWN)
				printf("hystHi: unknown (%d)\n", sensor_info.hystHi);
			else
				printf("hystHi: %d\n", sensor_info.hystHi);

			if (sensor_info.hystLo == KEAPI_SENSOR_INFO_UNKNOWN)
				printf("hystLo: unknown (%d)\n", sensor_info.hystLo);
			else
				printf("hystLo: %d\n", sensor_info.hystLo);
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
