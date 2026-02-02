#include "traffic_light.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"  
#include "pinsSetup.h"


/*
//task logic
void traffic_light_cycle(void *pvparams)            //void * is a generic pointer, it can be casted to anything
{
    (void) pvparams;        //when dont use parameter

    LedTaskParams params = {.delayTime = 2000};  //create object from struct and customize values

    //task "global" variables
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
*/
