#include "motor_control.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"  
#include "pinsSetup.h"
#include "driver/ledc.h"
#include "urls.h"

void pwm_init(){
  ledc_timer_config_t timer = {
      .speed_mode = LEDC_LOW_SPEED_MODE,            //speed = hardware slow = software
      .duty_resolution = LEDC_TIMER_10_BIT,         //duty cycle = divide frequency by this amount (10bit )
      .timer_num = LEDC_TIMER_0,                    
      .freq_hz = 5000,          //hz, how fast signal pulses (5000 = 5000 pulses seconds || on and off 5000 times second)
      .clk_cfg = LEDC_AUTO_CLK  //simple configuration for clock
    };
    ledc_timer_config(&timer);      





   //config channel to control duty cycle, power pushing out 
          ledc_channel_config_t channel = {
      .gpio_num = redLed,                           //output pin
      .speed_mode = LEDC_LOW_SPEED_MODE,
      .channel = LEDC_CHANNEL_0,                    
      .timer_sel = LEDC_TIMER_0,                //timer we configured before
      .duty = 0,                                //how much of a pulse we want to drive
                                                //duty cycle range depends on timer's duty resolution
                                                // (0 - 1024 for 10 bits)

      .hpoint = 0               //when duty cycle trigger, classic PWM = hpoint 0
    };
    ledc_channel_config(&channel);
}





void motorControl(void *pvParameters){      //task receiving queue
    HttpCommand_t rxCmd;        //received Cmd, struct we defined in the handler methos (which sends data in the queue)

    /*
    
    typedef struct      
    {
    int motor_mode;
    } HttpCommand_t;

    */

    while(1)            //tasks always need loop and a delay
    {
        if(xQueueReceive(motor_c_data, &rxCmd, portMAX_DELAY))      //check for queue data, must be created before use
                                                                    //with var = xQueueCreate(10, sizeof(HttpCommand_t));
        {
            printf("Read data from queue %d\n", rxCmd.motor_mode);  //on queue received
            if(rxCmd.motor_mode == 0)       //read value 5 setted by the queue sender, url-callback from http
            {
                ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
                ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
                printf("OK");
            }
        }



        vTaskDelay(pdMS_TO_TICKS(100));
    }
};
