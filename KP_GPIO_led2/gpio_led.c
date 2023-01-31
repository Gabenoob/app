#include <stdio.h>
#include <unistd.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_gpio.h"
#define LED_TASK_GPIO1 7
#define LED_TASK_GPIO2 8
#define LED_Time 30
#define LED_TSIZE 512
#define LED_TPRIO 2

static void LedTask1(void)
{
    while(1)
    {
        IoTGpioSetOutputVal(LED_TASK_GPIO1,1);

        IoTGpioSetOutputVal(LED_TASK_GPIO2,1);
        sleep(1);
        IoTGpioSetOutputVal(LED_TASK_GPIO2,0);
        sleep(1);
    }
}
static void LedExampleEntry(void)
{
    osThreadAttr_t attr;
    IoTGpioInit(LED_TASK_GPIO1);
    IoTGpioSetDir(LED_TASK_GPIO1,IOT_GPIO_DIR_OUT);
    IoTGpioInit(LED_TASK_GPIO2);
    IoTGpioSetDir(LED_TASK_GPIO2,IOT_GPIO_DIR_OUT);

    attr.name = "LedTask1";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = LED_TSIZE;
    attr.priority = LED_TPRIO;
    if(osThreadNew((osThreadFunc_t)LedTask1,NULL,&attr) == NULL){
        printf("[ledExample] Falied to create LedTask1!\n");
    }
}
SYS_RUN(LedExampleEntry);

static void LedTask1(void)
{
    while(1)
    {
        IoTGpioSetOutputVal(LED_TASK_GPIO1,1);

        IoTGpioSetOutputVal(LED_TASK_GPIO2,1);
        sleep(1);
        IoTGpioSetOutputVal(LED_TASK_GPIO2,0);
        sleep(1);
    }
}
static void LedExampleEntry(void)
{
    osThreadAttr_t attr;
    IoTGpioInit(LED_TASK_GPIO1);
    IoTGpioSetDir(LED_TASK_GPIO1,IOT_GPIO_DIR_OUT);
    IoTGpioInit(LED_TASK_GPIO2);
    IoTGpioSetDir(LED_TASK_GPIO2,IOT_GPIO_DIR_OUT);

    attr.name = "LedTask1";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = LED_TSIZE;
    attr.priority = LED_TPRIO;
    if(osThreadNew((osThreadFunc_t)LedTask1,NULL,&attr) == NULL){
        printf("[ledExample] Falied to create LedTask1!\n");
    }
}
SYS_RUN(LedExampleEntry);