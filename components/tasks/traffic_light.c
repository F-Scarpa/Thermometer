#include "traffic_light.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"  
#include "pinsSetup.h"

void traffic_light_cycle(void *pvparams)            //void * is a generic pointer, it can be casted to anything
{
    (void) pvparams;        

    LedTaskParams params = {.delayTime = 2000};       
    int cycle_flag = 0;
    
    while(1)            //in the task we always need while loop otherwise the program will crash
    {
    if (cycle_flag > 2)
        {
            cycle_flag = 0;
        } 
        switch (cycle_flag)
        {
            case 0:
                red_led_on();
                break;
            case 1:
                green_led_on();
                break;
            case 2:
                yellow_led_on();
                break;
            default:
                break;

        }
        printf("cycle flag is: %d\n", cycle_flag);
        vTaskDelay(pdMS_TO_TICKS(params.delayTime));
        cycle_flag++;

    
    }
}

void traffic_light_error(void *pvparams)
{
    (void) pvparams;        //ignore pvparmas

    LedTaskParams params = {.delayTime = 500};      //create value for a struct filed from inside the task
    while(1)
    {
    yellow_led_on();
    vTaskDelay(pdMS_TO_TICKS(params.delayTime));
    yellow_led_off();
    vTaskDelay(pdMS_TO_TICKS(params.delayTime));
    }
}

void traffic_light_disable(void *pvparams)
{
    all_led_off();
};