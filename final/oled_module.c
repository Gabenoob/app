#include <stdio.h>
#include <unistd.h>
#include <iot_gpio.h>
#include <iot_gpio_ex.h>
#include <iot_spi.h>
#include "oled.h"
#include <base_type.h>

#include "dbg.h"
#include "ohos_init.h"
#include "cmsis_os2.h"

#define OLED_TASK_STACK_SIZE 4096

void OLedTask(void * para) /* task处理函数 */
{
 
    test_led_screen();

    return NULL;
}

void oled_demo(void)
{    
    int ret;
    IoTGpioInit(Button);
    IoTGpioSetFunc(Button,0);
    IoTGpioSetDir(Button,IOT_GPIO_DIR_IN);
    IoTGpioSetPull(Button,1);
    oled_gpio_io_init();

    IotSpiCfgInitParam init_param={
        .isSlave = 0
    };
    
    screen_spi_master_init(0); 
    osThreadAttr_t attr;
 
    attr.name = "OLedTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = OLED_TASK_STACK_SIZE;
    attr.priority = 20;
    if (osThreadNew((osThreadFunc_t)OLedTask, NULL, &attr) == NULL) {
        printf("[OLedExample] Falied to create LedTask1!\n");
    }
}
APP_FEATURE_INIT(oled_demo);
