#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "dht_read.h"
#include "helper.h"
#include "cJSON.h"

void send_JSON_num(int num)     //send data as JSON
{
    cJSON *payload = cJSON_CreateObject();                                          
    cJSON_AddNumberToObject(payload,"temperature",num);                
    char *message = cJSON_Print(payload);       
    printf("message : %s\n", message);
    //send message to websocket
    send_ws_message(message);               
    //free heap memory
    cJSON_Delete(payload);     
    free(message); 
    
}

//function to set high alarm threshold
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
        //read temperature and humidity
        if (dht_read_float_data(SENSOR_TYPE, CONFIG_EXAMPLE_DATA_GPIO, &humidity, &temperature) == ESP_OK)
            printf("Humidity: %0.f%% Temp: %0.fC\n", humidity, temperature);
        else
            printf("Could not read data from sensor\n");
           
        
        high_temp_alarm(temperature + debug_aug, 20);
        //send temperature to frontend with websocket
        send_JSON_num(temperature);
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
};