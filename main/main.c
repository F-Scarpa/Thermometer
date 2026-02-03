#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h" 
#include "freertos/semphr.h"       

#include "nvs_flash.h"   
#include "secrets.h"
#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "esp_err.h"
#include "mymdns.h"
#include "server_init.h"
#include "wifi_connect.h"

//from tasks
#include "traffic_light.h"      
#include "pedestrian_call.h"   
#include "pinsSetup.h"
#include "dht_read.h"
#include "motor_control.h"
#include "urls.h"



//volatile int mode = 0;
//volatile int vehicle_tra_lig_state = 0;

//task handlers
//TaskHandle_t traffic_handle = NULL;
TaskHandle_t dht_test_handle = NULL;
TaskHandle_t motor_control_handle = NULL;

//queue handlers
QueueHandle_t motor_c_data = NULL;      //global rtos resouces (queue)


/*
if (traffic_handle)             //if task has been created delete it and reset value
    {
        vTaskDelete(traffic_handle);            //kill task
        traffic_handle = NULL;                  //remove old values from task
    }


if(traffic_handle == NULL)              //only create task if doesn't exist
    {
        xTaskCreate(&traffic_light_cycle,"traffic_light_cycle",2048, NULL, 2, &traffic_handle);     //this will call the defined task using &object as arguments
    }   
*/



void app_main(void)
{   
    
    //initializers
    pinsInit();             //pins
    pwm_init();             //pwm
    int_init();             //interrupt

    //connect to wifi
    ESP_ERROR_CHECK(nvs_flash_init());
    wifi_connect_init();
    ESP_ERROR_CHECK(wifi_connect_sta(WIFI_SSID, WIFI_PASSWORD, 10000));

    //start mdns service
    start_mdns_service();
    mount_fs();   
    init_server();

    
    //motorControl();
    motor_c_data = xQueueCreate(10, sizeof(HttpCommand_t));     //create queue before using it
    //check for motor_control task
    if(motor_control_handle == NULL)
    {
       xTaskCreate(motorControl, "motor_control", configMINIMAL_STACK_SIZE * 3, NULL, 5, &motor_control_handle);
    }


    if(dht_test_handle == NULL)
    {
       //xTaskCreate(dht_test, "dht_test", configMINIMAL_STACK_SIZE * 30, NULL, 5, &dht_test_handle);
    }

}

