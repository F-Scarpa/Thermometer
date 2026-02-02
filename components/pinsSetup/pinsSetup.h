#ifndef PINSSETUP_H
#define PINSSETUP_H

//esp GPIOS
#define redLed 13
#define yellowLed 12
#define greenLed 14

//extern volatile int vehicle_tra_lig_state;            //extern = defined elsewhere 
                                                        // volatile = changes frequently


void pinsInit();

void red_led_on();

void send_JSON_num(int num);


#endif