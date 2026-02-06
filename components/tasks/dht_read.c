#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "dht_read.h"
#include "helper.h"
#include "cJSON.h"
#include "esp_log.h"

float temperature = 0;
volatile uint16_t high_temp_threshold = 0;

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
    uint32_t cmd;  //notification buffer, it can store up to 32 different commands (32bits)

    float humidity;
    

    while (1)
    {   

      /* if (xTaskNotifyWait(0, ULONG_MAX, &cmd , 0) == pdTRUE)      //0, ULONG_MAX = delete all bits after reading, resetting the message
        {
            // 00000011 & 00000001 = 00000001 check if both bytes have same value, only the LSB have 1+1
            if (cmd & DHT_CMD_RESET)            
            {
                printf("DHT reset\n");
            }

            if (cmd & DHT_CMD_UPDATE)
            {
                printf("Threshold updated: %d\n", high_temp_threshold);
            }
        }*/

        UBaseType_t free = uxTaskGetStackHighWaterMark(NULL);
        //printf("STACK FREE: %u\n", free);
        //read temperature and humidity
        if (dht_read_float_data(SENSOR_TYPE, CONFIG_EXAMPLE_DATA_GPIO, &humidity, &temperature) == ESP_OK)
            //printf("Humidity: %0.f%% Temp: %0.fC\n", humidity, temperature);
            {}
        else
            printf("Could not read data from sensor\n");
           
        
        high_temp_alarm(temperature, high_temp_threshold);
        //send temperature to frontend with websocket
        send_JSON_num(temperature);
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
};