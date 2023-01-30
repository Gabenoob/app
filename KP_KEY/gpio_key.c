#include <stdio.h>
#include <unistd.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_gpio.h"
#define LED_TASK_GPIO 8
#define LED_INTERVAL_TIME 30
#define LED_TSIZE 2048
#define LED_TPRIO 25
#define IOT_GPIO_KEY 5
enum LedState{
    LED_ON=0,
    LED_OFF=1,
    LED_SPARK=2
};
enum LedState g_ledState = LED_ON;
static void *LedTask(const char *arg)
{
    (void)arg;
    while (1){
        switch (g_ledState)
        {
        case LED_ON:
            IoTGpioSetOutputVal(LED_TASK_GPIO,1);
            osDelay(LED_INTERVAL_TIME);
            break;
        case LED_OFF:
            IoTGpioSetOutputVal(LED_TASK_GPIO,0);
            osDelay(LED_INTERVAL_TIME);
            break;
        case LED_SPARK:
            IoTGpioSetOutputVal(LED_TASK_GPIO,0);
            osDelay(LED_INTERVAL_TIME);
            IoTGpioSetOutputVal(LED_TASK_GPIO,1);
            osDelay(LED_INTERVAL_TIME);
            break;
        default:
            osDelay(LED_INTERVAL_TIME);
            break;
        }
    }
}
unsigned int lastCount;
static GpioIsrCallbackFunc OnButtonPressed(char *arg){
    (void)arg;
    unsigned int tickCount = osKernelGetTickCount();
    unsigned int count = tickCount - lastCount;
    lastCount = tickCount;
    if (count<50){
        printf("return\n");
        return ;
    }

    enum LedState nextState = LED_SPARK;
    switch (g_ledState)
    {
    case LED_ON:
        nextState = LED_SPARK;
        break;
    case LED_OFF:
        nextState = LED_ON;
        break;
    case LED_SPARK:
        nextState = LED_ON;
        break;
    default:
        break;
    }
    g_ledState = nextState;
}

static void LedExampleEntry(void)
{
    osThreadAttr_t attr;
    IoTGpioInit(LED_TASK_GPIO);
    IoTGpioSetDir(LED_TASK_GPIO,IOT_GPIO_DIR_OUT);

    IoTGpioInit(IOT_GPIO_KEY);
    IoTGpioSetFunc(IOT_GPIO_KEY,0);
    IoTGpioSetDir(IOT_GPIO_KEY,IOT_GPIO_DIR_IN);
    IoTGpioSetPull(IOT_GPIO_KEY,1);

    int ret = IoTGpioRegisterIsrFunc(IOT_GPIO_KEY,IOT_INT_TYPE_EDGE,IOT_GPIO_EDGE_FALL_LEVEL_LOW,OnButtonPressed,NULL);
    attr.name = "LedTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = LED_TSIZE;
    attr.priority = LED_TPRIO;
    if(osThreadNew((osThreadFunc_t)LedTask,NULL,&attr) == NULL){
        printf("[ledExample] Falied to create LedTask1!\n");
    }
}
SYS_RUN(LedExampleEntry);