#include "light.h"

int g_light_state;

int light_init(){
    IoTGpioInit(LED_GPIO_IDX_BLUE);
    IoTGpioSetFunc(LED_GPIO_IDX_BLUE,IOT_GPIO_FUNC_GPIO_6_GPIO);
    IoTGpioSetDir(LED_GPIO_IDX_BLUE,IOT_GPIO_DIR_OUT);
    IoTGpioInit(LED_GPIO_IDX_WHITE);
    IoTGpioSetFunc(LED_GPIO_IDX_WHITE,IOT_GPIO_FUNC_GPIO_6_GPIO);
    IoTGpioSetDir(LED_GPIO_IDX_WHITE,IOT_GPIO_DIR_OUT);
    IoTGpioInit(LED_GPIO_IDX_GREEN);
    IoTGpioSetFunc(LED_GPIO_IDX_GREEN,IOT_GPIO_FUNC_GPIO_6_GPIO);
    IoTGpioSetDir(LED_GPIO_IDX_GREEN,IOT_GPIO_DIR_OUT);
    IoTGpioInit(LED_GPIO_IDX_RED);
    IoTGpioSetFunc(LED_GPIO_IDX_RED,IOT_GPIO_FUNC_GPIO_6_GPIO);
    IoTGpioSetDir(LED_GPIO_IDX_RED,IOT_GPIO_DIR_OUT);
    printf("\nLIGHTINIT\n");
}

void Led_on_off(int value,int GPIO_IDX){
    printf("in!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    if(value==LED_ON)
        IoTGpioSetOutputVal(GPIO_IDX,IOT_GPIO_VALUE1);
    else
        IoTGpioSetOutputVal(GPIO_IDX,IOT_GPIO_VALUE0);
    return 1;
}
int light_Deinit(){
    IoTGpioDeInit(LED_GPIO_IDX_BLUE);
    IoTGpioDeInit(LED_GPIO_IDX_WHITE);
    IoTGpioDeInit(LED_GPIO_IDX_GREEN);
    IoTGpioDeInit(LED_GPIO_IDX_RED);
}