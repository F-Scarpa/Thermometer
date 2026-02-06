#pragma once
#include <stdint.h>

void motorControl(void *pvParameters);
void pwm_init();

void soft_starter(uint16_t *motor_speed, uint8_t pwm_duty);