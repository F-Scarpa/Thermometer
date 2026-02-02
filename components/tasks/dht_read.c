#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "dht_read.h"
#include "pinsSetup.h"

void high_temp_alarm(int16_t measure, uint8_t threshold)
{
    if(measure > threshold){
        printf("fake call\n");
    }
}

void dht_test(void *pvParameters)
{
    float temperature, humidity;
    uint8_t debug_aug = 10;

    while (1)
    {
        if (dht_read_float_data(SENSOR_TYPE, CONFIG_EXAMPLE_DATA_GPIO, &humidity, &temperature) == ESP_OK)
            printf("Humidity: %0.f%% Temp: %0.fC\n", humidity, temperature);
        else
            printf("Could not read data from sensor\n");
           
        high_temp_alarm(temperature + debug_aug, 20);

        // If you read the sensor data too often, it will heat up
        // http://www.kandrsmith.org/RJS/Misc/Hygrometers/dht_sht_how_fast.html
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
};