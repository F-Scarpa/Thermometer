#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "dht_read.h"
#include "helper.h"     //helper import the send_ws_message
#include "cJSON.h"
#include "esp_log.h"

float temperature = 0;
volatile uint16_t high_temp_threshold = 35;

void send_JSON_num(int num)     //send data as JSON
{
    cJSON *payload = cJSON_CreateObject();                                          
    cJSON_AddNumberToObject(payload,"temperature",num);                
    char *message = cJSON_Print(payload);       
    //printf("message : %s\n", message);
    //send message to websocket
    send_ws_message(message);               
    //free heap memory
    cJSON_Delete(payload);     
    free(message);    
}

//function to set high alarm threshold
static void high_temp_alarm(int16_t measure, uint8_t threshold)
{
    if(measure > threshold){
        printf("temp high\n");
    }
}



//file task
void dht_test(void *pvParameters)
{
   

    float humidity;
    

    while (1)
    {   



        UBaseType_t free = uxTaskGetStackHighWaterMark(NULL);
        printf("STACK FREE: %u\n", free);
        //read temperature and humidity
        if (dht_read_float_data(SENSOR_TYPE, CONFIG_EXAMPLE_DATA_GPIO, &humidity, &temperature) == ESP_OK)
            //printf("Humidity: %0.f%% Temp: %0.fC\n", humidity, temperature);
            //printf( "%0.f\n", temperature); 
            {}
        else
            printf("Could not read data from sensor\n");
           
        
        high_temp_alarm(temperature, high_temp_threshold);
        //send temperature to frontend with websocket
        send_JSON_num(temperature);
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
};