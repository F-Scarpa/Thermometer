#include "pinsSetup.h"
#include "driver/gpio.h"  
#include <stdio.h>
#include "cJSON.h"
#include "helper.h"

void send_JSON_num(int num)
{
    static int previous_num = -1;
    if(previous_num != num)
    {
        previous_num = num;
        cJSON *payload = cJSON_CreateObject();                                          
        cJSON_AddNumberToObject(payload,"tra_lig_state",num);                
        char *message = cJSON_Print(payload);       
        printf("message : %s\n", message);
        send_ws_message(message);               //send message to websocket
        //free heap memory
        cJSON_Delete(payload);     
        free(message); 
    }
}

void pinsInit(){
    gpio_set_direction(redLed, GPIO_MODE_OUTPUT );
    gpio_set_direction(greenLed, GPIO_MODE_OUTPUT );    
    gpio_set_direction(yellowLed, GPIO_MODE_OUTPUT );  
}

void red_led_on()
{
    gpio_set_level(redLed,1);
    gpio_set_level(yellowLed,0);
    gpio_set_level(greenLed,0);
    vehicle_tra_lig_state = 1;
    send_JSON_num(vehicle_tra_lig_state);
    /*
    cJSON *payload = cJSON_CreateObject();                                          
    cJSON_AddNumberToObject(payload,"tra-lig-state",vehicle_tra_lig_state);                
    char *message = cJSON_Print(payload);       
    printf("message : %s\n", message);
    send_ws_message(message);               //send message to websocket
    //free heap memory
    cJSON_Delete(payload);     
    free(message); */
}

void red_led_off()
{
    gpio_set_level(redLed,0);
    vehicle_tra_lig_state = 0;
    send_JSON_num(vehicle_tra_lig_state);

}

void yellow_led_on()
{
    gpio_set_level(redLed,0);
    gpio_set_level(yellowLed,1);
    gpio_set_level(greenLed,0);
    vehicle_tra_lig_state = 2;
    send_JSON_num(vehicle_tra_lig_state);
}


void green_led_on()
{
    gpio_set_level(redLed,0);
    gpio_set_level(yellowLed,0);
    gpio_set_level(greenLed,1);
    vehicle_tra_lig_state = 3;
    send_JSON_num(vehicle_tra_lig_state);
}

void yellow_led_off()
{
    gpio_set_level(yellowLed,0);
    vehicle_tra_lig_state = 0;
    send_JSON_num(vehicle_tra_lig_state);
}

void all_led_off()
{
    gpio_set_level(yellowLed,0);
    gpio_set_level(redLed,0);
    gpio_set_level(greenLed,0);
    vehicle_tra_lig_state = 0;
    send_JSON_num(vehicle_tra_lig_state);
}