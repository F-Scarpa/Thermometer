#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H


typedef struct {                                //it's a good idea to put task parameters inside a struct
    int delayTime;
    
} LedTaskParams;

void traffic_light_cycle(void *pvparams);          //void * is a generic pointer, it can be casted to anything


#endif