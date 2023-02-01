#include <stdio.h>  // for printf
#include <string.h> // for memory operation
#include <stdlib.h>
#include <unistd.h>  // for sleep
#include "ohos_init.h"
#include "cmsis_os2.h"

#include "iot_i2c.h"
#include "iot_i2c_ex.h"
#include "iot_gpio.h"
#include "iot_gpio_ex.h"


#define I2C_SDA_GPIO_IDX 13
#define I2C_SCL_GPIO_IDX 14

#define WIFI_IOT_I2C_IDX_0 0

#define SHT30_ADDR 0x44
#define I2C_WRITE_FLAG 0

float g_temp;
float g_humi;


void initI2C();

static uint8_t SHT3xCheckCrc(uint8_t data[], uint8_t nbrOfBytes, uint8_t checksum);

static float SHT3xCalcTemperatureC(uint16_t u16sT);

static float SHT3xCalcRH(uint16_t u16sRH);

static int InitSHT30(void);

int SHT3X_ReadMeasurementBuffer(float *temperature,float *humidity);

static int temp_humi_task();

int temp_humi_init();