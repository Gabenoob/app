#include<stdio.h>
#include <iot_gpio.h>
#include <iot_gpio_ex.h>
#include <iot_i2c.h>
#include <iot_i2c_ex.h>
#include "ohos_init.h"
#include "cmsis_os2.h"

#define I2C_device_id 0
#define ADDR 0x44

float count_TEMP(uint16_t input){
    return -45+175*(float)input/(2<<16-1);
}

float count_HUMI(uint16_t input){
    return 100*(float)input/(2<<16-1);
}

void MeansureBuffer(float *temp,float*humi){
    uint8_t receive_buffer[6];
    uint8_t send_buff[2];
    uint16_t getTemp;
    uint16_t getHumi;
    send_buff[0] = 0xE0;
    send_buff[1] = 0x00;

    IotI2cData data;
    data.sendBuf = send_buff;
    data.sendLen = 2;
    data.receiveBuf = receive_buffer;
    data.receiveLen = 6;

    
    IoTI2cWriteread(I2C_device_id, ADDR<<1, &data);
    getTemp = (uint16_t)data.receiveBuf[0]<<8|data.receiveBuf[1];
    printf("\n%d\n",getTemp);
    getHumi = (uint16_t)data.receiveBuf[3]<<8|data.receiveBuf[4];
    printf("\n%d\n",getHumi);
    *temp = count_TEMP(getTemp);
    *humi = count_HUMI(getHumi);
    
}

void Init_SHT30(){
    IoTGpioInit(13);
    IoTGpioSetFunc(13,IOT_GPIO_FUNC_GPIO_13_I2C0_SDA);
    IoTGpioInit(14);
    IoTGpioSetFunc(14,IOT_GPIO_FUNC_GPIO_14_I2C0_SCL);
    IoTI2cInit(I2C_device_id,400000);


}

void detector(){
    Init_SHT30();
    float humi;
    float temp;

    while(1){
        MeansureBuffer(&temp,&humi);
        printf("temp is %f\n",temp);
        printf("humi is %f\n",humi);
        sleep(3);
    }
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