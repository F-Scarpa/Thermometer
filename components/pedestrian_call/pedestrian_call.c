#include "pedestrian_call.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h" 
#include "freertos/queue.h"
#include "pinsSetup.h"

QueueHandle_t interruptQueue;

//static function are to be used within this file, dont need to put them in .h file
static void IRAM_ATTR gpio_isr_handler(void *args)                  //callback function for interrupt
{
    int pinNumber = (int)args;
    xQueueSendFromISR(interruptQueue, &pinNumber,  NULL);            //we use queue because we pass parameter else we could use semaphore 
}

static void test_call(int greenT, int yelloT, int redT)
{
    vTaskDelay(pdMS_TO_TICKS(greenT));
    yellow_led_on();
    vTaskDelay(pdMS_TO_TICKS(yelloT));
    red_led_on();
    vTaskDelay(pdMS_TO_TICKS(redT));
    green_led_on();
}

void buttonPushedTask(void *params)                                 //function wakes up after button press which activates gpio_isr_handler
{
    int pinNumber, count = 0;
    TickType_t lastWakeTime = 0;                                    //store 0 but will update as interrupt happen 
    const TickType_t debounceTicks = pdMS_TO_TICKS(150);            //debounce
    while (true)
    {
        if (xQueueReceive(interruptQueue, &pinNumber, portMAX_DELAY))
        {
            if (xTaskGetTickCount() - lastWakeTime >= debounceTicks)
            {
                printf("GPIO %d was pressed %d times. The state is %d\n", pinNumber, count++, gpio_get_level(interrupt_button));
                test_call(2000,2000,5000);
                lastWakeTime = xTaskGetTickCount();                 //update as ticksCount from the kernel start
            }
        }
    }
}

void int_init()
{
    gpio_set_direction(interrupt_button, GPIO_MODE_INPUT);
    gpio_pulldown_dis(interrupt_button);
    gpio_pullup_en(interrupt_button);
    gpio_set_intr_type(interrupt_button, GPIO_INTR_NEGEDGE);     
    interruptQueue = xQueueCreate(10, sizeof(int));
    gpio_install_isr_service(0);
    gpio_isr_handler_add(interrupt_button, gpio_isr_handler, (void *)interrupt_button);             
}


