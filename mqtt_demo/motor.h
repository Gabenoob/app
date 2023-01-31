#include "iot_gpio.h"
#include "iot_gpio_ex.h"
#include "iot_pwm.h"

#define MOTOR 2
#define MOTOR_PWM_CHN 2

int motor_Init();
int motor_speed(int speed);