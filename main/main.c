#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"  

#include "nvs_flash.h"   
#include "secrets.h"
#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "esp_err.h"

#include "traffic_light.h"      //from tasks
#include "pedestrian_call.h"   
#include "pinsSetup.h"
#include "mymdns.h"
#include "server_init.h"
#include "wifi_connect.h"

volatile int mode = 0;
volatile int vehicle_tra_lig_state = 0;

//task handlers
TaskHandle_t traffic_handle = NULL;
TaskHandle_t error_handle = NULL;
TaskHandle_t button_handle = NULL;


void main_task (void *params)
{
    int last_mode = -1;     
    while(1)
    {
            if (mode != last_mode)          //only execute block if mode has been modified
                {
                printf("Mode changed: %d\n", mode);

                // which task is alive? kill and removes the old values
                if (traffic_handle)
                {
                    vTaskDelete(traffic_handle);            //kill task
                    traffic_handle = NULL;                  //remove old values from task
                }

                if (error_handle)
                {
                    vTaskDelete(error_handle);
                    error_handle = NULL;
                }

                if (button_handle)
                {
                    vTaskDelete(button_handle);
                    button_handle = NULL;
                }

                last_mode = mode;
            }
    
        switch(mode)        //check if task doesn't exist, if it does not then create it
        {
        case 0:
            all_led_off();
            break;
        case 1:
            if(traffic_handle == NULL)
            {
                xTaskCreate(&traffic_light_cycle,"traffic_light_cycle",2048, NULL, 2, &traffic_handle);     //this will call the defined task using &object as arguments
            }                                                                                    //how arguments are handled is defined in the task function
            break;
        case 2:
            if(error_handle == NULL)
            {
                xTaskCreate(&traffic_light_error,"traffic_light_error",2048, NULL, 2, &error_handle);    //params can also be selected from inside the task but
            }                                                                             //use NULL as 4 parameter in xtaskCreate
            break;
        case 3:
            //green_led_on();
            if(button_handle == NULL)
            {
            xTaskCreate(&buttonPushedTask,"button_pushed_task",2048, NULL, 2, &button_handle);
            }
            break;
        }
    }
}

void app_main(void)
{   
    
    pinsInit();
    int_init();

    ESP_ERROR_CHECK(nvs_flash_init());
    wifi_connect_init();
    ESP_ERROR_CHECK(wifi_connect_sta(WIFI_SSID, WIFI_PASSWORD, 10000));

    start_mdns_service();
    mount_fs();   
    init_server();

    xTaskCreate(main_task,"main_task",2048,NULL,5,NULL);
                                                                          
                                                                             
    

}

