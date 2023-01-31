#include "iot_gpio.h"
#include "iot_gpio_ex.h"
#include "iot_pwm.h"

#define LED_GPIO_IDX 6
#define LED_ON 1
#define LED_OFF 0

int light_init();
int Led_on_off(int value);