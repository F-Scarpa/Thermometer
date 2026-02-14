#include "motor_control.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"  
#include "pinsSetup.h"
#include "driver/ledc.h"
#include "urls.h"
#include "dht_read.h"
#include "cJSON.h"
#include "helper.h"     //helper import the send_ws_message


extern float temperature;
extern uint16_t high_temp_threshold;
uint8_t mode = 0;

void send_JSON_bool(bool boolVal)     //send data as JSON
{
    cJSON *payload = cJSON_CreateObject();                                          
    cJSON_AddBoolToObject(payload,"toggle-state",boolVal);                
    char *message = cJSON_Print(payload);       
    //printf("message : %s\n", message);
    //send message to websocket
    send_ws_message(message);               
    //free heap memory
    cJSON_Delete(payload);     
    free(message);    
}


void soft_starter(uint16_t *motor_speed, uint8_t pwm_duty)      //soft start and stop to avoid dangerous current spikes
{
    if (*motor_speed < 1023 - pwm_duty)
    {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, *motor_speed);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        (*motor_speed) += pwm_duty;
        //printf("%d\n",*motor_speed);
    }
    //used inside task, can use task's delay time
}

void soft_stop(uint16_t *motor_speed, uint8_t pwm_duty)
{
    //if speed is greater than pwm_duty but still not 0, force value to 0
    if(*motor_speed < pwm_duty)     
    {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, *motor_speed);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    }

    if (*motor_speed > 0)
    {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, *motor_speed);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        (*motor_speed) -= pwm_duty;
        //printf("%d\n",*motor_speed);
    }
    //used inside task, can use task's delay time
}



void pwm_init(){
    //setup timer, used for pwm
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
    static uint16_t motor_speed_shared = 0; //variable used by softstart and softstop
    static bool toggle_mode = false;        //toggle actuator for manual mode

    while(1)            //tasks always need loop and a delay
    {

        if(xQueueReceive(motor_c_data, &rxCmd, 0) == pdTRUE)      //check for queue data, must be created before use, 0) ==pdTrue makes the queue not blocking
                                                                    //with var = xQueueCreate(10, sizeof(HttpCommand_t));
        {
            

            //off
            //read value inside queue and select a mode
            if(rxCmd.motor_mode == 0)       //read value 0 setted by the queue sender, url-callback from http
            {
                //force toggle mode to false when disable mode or auto mode is active

                //ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
                //ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
                printf("disabled from motor_control\n");

                mode = 0;       //mode is needed to update the program, without mode when the queue is read it is exited immediately and nothing would happen
                                //mode allows to update based on mode we are in
            }
            //auto
            else if (rxCmd.motor_mode == 1)      
            {

                printf("auto from motor_control\n");
                mode = 1;

            }
            //man
            else if (rxCmd.motor_mode == 2)      
            {
                //ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
                //ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
                //printf("man from motor_control\n");
                mode = 2;
            }
        }

        //logic 
        if(mode == 0)
        {
            soft_stop(&motor_speed_shared, 20);  //first argument passed as a address to avoid updating it every function call,
                                                 //a pointer enable the function to leverage value inside address instead of creating a copy
        }
        

        if (mode == 1)      //auto mode
        {
            if(temperature > high_temp_threshold)
            {
                soft_starter(&motor_speed_shared, 20);

            }

            if(temperature <= high_temp_threshold - 5)
            {   
                soft_stop(&motor_speed_shared, 20);
            }
        }

        if(mode == 2)         //manual mode
        {
            toggle_mode ? soft_starter(&motor_speed_shared, 20) : soft_stop(&motor_speed_shared, 20);
            if(xSemaphoreTake(toggle_led_semaphore, 0) == pdTRUE)
            //send_JSON_bool(toggle_mode);
            {
                send_JSON_bool(toggle_mode);
                printf("Toggle mode called from semaphore\n");
                toggle_mode = !toggle_mode;
            }
            
        }
        
        



        vTaskDelay(pdMS_TO_TICKS(100));    
    }
};
