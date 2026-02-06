#pragma once
#include "dht.h"

#define SENSOR_TYPE DHT_TYPE_DHT11
#define CONFIG_EXAMPLE_DATA_GPIO 4

#define DHT_CMD_RESET   (1 << 0)        //00000001
#define DHT_CMD_UPDATE  (1 << 1)        //00000010

void dht_test(void *pvParameters);

