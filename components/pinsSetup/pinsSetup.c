#include "pinsSetup.h"
#include "driver/gpio.h"  
#include <stdio.h>
#include "cJSON.h"
#include "helper.h"


//create JSONdata to send to websocket
/*
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
*/
//pinModes
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

    //vehicle_tra_lig_state = 1;
    //send_JSON_num(vehicle_tra_lig_state);
}