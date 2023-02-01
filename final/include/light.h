#include "iot_gpio.h"
#include "iot_gpio_ex.h"
#include "iot_pwm.h"
#include <stdio.h>

#define LED_GPIO_IDX_WHITE 6
#define LED_GPIO_IDX_BLUE 7
#define LED_GPIO_IDX_RED 8
#define LED_GPIO_IDX_GREEN 2
#define LED_ON 1
#define LED_OFF 0

int light_init();
void Led_on_off(int value,int GPIO_IDX);
int light_Deinit();