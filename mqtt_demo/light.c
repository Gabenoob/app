#include "light.h"

int g_light_state;

int light_init(){
    IoTGpioInit(LED_GPIO_IDX);
    IoTGpioSetFunc(LED_GPIO_IDX,IOT_GPIO_FUNC_GPIO_6_GPIO);
    IoTGpioSetDir(LED_GPIO_IDX,IOT_GPIO_DIR_OUT);
}

int Led_on_off(int value){
    int ret = IoTGpioSetOutputVal(LED_GPIO_IDX,value?LED_ON:LED_OFF);
    return ret;
}