#ifndef PINSSETUP_H
#define PINSSETUP_H

#define redLed 13
#define yellowLed 12
#define greenLed 14

extern volatile int vehicle_tra_lig_state;

void pinsInit();

void red_led_on();
void red_led_off();

void yellow_led_on();
void yellow_led_off();

void green_led_on();

void all_led_off();
void send_JSON_num(int num);


#endif