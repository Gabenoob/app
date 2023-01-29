#include<stdio.h>
#include <iot_gpio.h>
#include <iot_gpio_ex.h>
#include <iot_i2c.h>
#include <iot_i2c_ex.h>
#include "ohos_init.h"
#include "cmsis_os2.h"


float count_TEMP(uint16_t input){

}

float count_HUMI(uint16_t input){
    
}

void i2c_init(void){

}

void detector(void){
    i2c_init();
}

void temp_humi_demo(void){
    osThreadAttr_t task_CB;
    task_CB.name = "detector";
    task_CB.priority = 20;
    task_CB.attr_bits = 0U;
    task_CB.cb_size = 0U;
    task_CB.stack_mem = NULL;
    task_CB.stack_size = 4096;
    task_CB.cb_mem = NULL;
    if (osThreadNew((osThreadFunc_t)detector, NULL, &task_CB) == NULL) {
        printf("[TH_Example] Falied to create detectorTask!\n");
    }
}





APP_FEATURE_INIT(temp_humi_demo);