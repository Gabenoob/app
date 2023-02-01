#include "temp_humi_test.h"
void initI2C()
{
    // 初始化io
    IoTGpioInit(I2C_SDA_GPIO_IDX);
    IoTGpioSetFunc(I2C_SDA_GPIO_IDX, IOT_GPIO_FUNC_GPIO_13_I2C0_SDA); // GPIO_0复用为I2C1_SDA
    IoTGpioInit(I2C_SCL_GPIO_IDX);
    IoTGpioSetFunc(I2C_SCL_GPIO_IDX, IOT_GPIO_FUNC_GPIO_14_I2C0_SCL); // GPIO_1复用为I2C1_SCL
    IoTI2cInit(WIFI_IOT_I2C_IDX_0, 400 * 1000);
}
// checkCRC

/***************************************************************
* 函数名称: SHT3xCheckCrc
* 说    明: 检查数据正确性
* 参    数: data：读取到的数据
                        nbrOfBytes：需要校验的数量
                        checksum：读取到的校对比验值
* 返 回 值: 校验结果，0-成功		1-失败
***************************************************************/
static uint8_t SHT3xCheckCrc(uint8_t data[], uint8_t nbrOfBytes, uint8_t checksum)
{
    uint8_t crc = 0xFF;
    uint8_t bit = 0;
    uint8_t byteCtr;
    const int16_t POLYNOMIAL = 0x131;
    // calculates 8-Bit checksum with given polynomial
    for (byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr)
    {
        crc ^= (data[byteCtr]);
        for (bit = 8; bit > 0; --bit)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ POLYNOMIAL;
            else
                crc = (crc << 1);
        }
    }

    if (crc != checksum)
        return 1;
    else
        return 0;
}

/***************************************************************
 * 函数名称: SHT3xCalcTemperatureC
 * 说    明: 温度计算
 * 参    数: u16sT：读取到的温度原始数据
 * 返 回 值: 计算后的温度数据
 ***************************************************************/
static float SHT3xCalcTemperatureC(uint16_t u16sT)
{
    float temperatureC = 0; // variable for result
    // u16sT &= ~0x0003;       // clear bits [1..0] (status bits)
    //-- calculate temperature [℃] --
    temperatureC = (175 * (float)u16sT / 65535 - 45); // T = -45 + 175 * rawValue / (2^16-1)
    return temperatureC;
}

/***************************************************************
 * 函数名称: SHT3xCalcRH
 * 说    明: 湿度计算
 * 参    数: u16sRH：读取到的湿度原始数据
 * 返 回 值: 计算后的湿度数据
 ***************************************************************/
static float SHT3xCalcRH(uint16_t u16sRH)
{
    float humidityRH = 0; // variable for result
    // u16sRH &= ~0x0003;    // clear bits [1..0] (status bits)
    //-- calculate relative humidity [%RH] --
    humidityRH = (100 * (float)u16sRH / 65535); // RH = rawValue / (2^16-1) * 10
    return humidityRH;
}

// 0x44 | 1

static int InitSHT30(void)
{
    int ret;
    uint8_t send_data[2] = {0x22, 0x36};
    ret = IoTI2cWrite(WIFI_IOT_I2C_IDX_0, (SHT30_ADDR << 1) | I2C_WRITE_FLAG, send_data, 2);
    if (ret != 0)
    {
        printf("===== Error: I2C write ret = 0x%x! =====\r\n", ret);
        return -1;
    }
    return 0;
}

int SHT3X_ReadMeasurementBuffer(float *temperature, float *humidity)
{
    int ret;
    uint8_t data[3]; // data array for checksum verification
    uint16_t dat, tmp;
    uint8_t SHT3X_Data_Buffer[6]; // byte 0,1 is temperature byte 4,5 is humidity

    IotI2cData sht30_i2c_data = {0};
    uint8_t send_data[2] = {0xE0, 0x00};
    sht30_i2c_data.sendBuf = send_data;
    sht30_i2c_data.sendLen = 2;
    sht30_i2c_data.receiveBuf = SHT3X_Data_Buffer;
    sht30_i2c_data.receiveLen = 6;
    ret = IoTI2cWriteread(WIFI_IOT_I2C_IDX_0, (SHT30_ADDR << 1) | 0x00, &sht30_i2c_data);
    if (ret != 0)
    {
        return -1;
    }
    /* check tem */
    data[0] = SHT3X_Data_Buffer[0];
    data[1] = SHT3X_Data_Buffer[1];
    data[2] = SHT3X_Data_Buffer[2];

    tmp = SHT3xCheckCrc(data, 2, data[2]);
    /* value is ture */
    if (!tmp)
    {
        dat = ((uint16_t)data[0] << 8) | data[1];
        *temperature = SHT3xCalcTemperatureC(dat);
    }

    /* check humidity */
    data[0] = SHT3X_Data_Buffer[3];
    data[1] = SHT3X_Data_Buffer[4];
    data[2] = SHT3X_Data_Buffer[5];
    /* value is ture */
    tmp = SHT3xCheckCrc(data, 2, data[2]);
    if (!tmp)
    {
        dat = ((uint16_t)data[0] << 8) | data[1];
        *humidity = SHT3xCalcRH(dat);
    }
}

static int temp_humi_task()
{

    initI2C();

    // 初始化sht30
    InitSHT30();
    float temp;
    float humi;
    while (1)
    {
        
        SHT3X_ReadMeasurementBuffer(&temp,&humi);
        g_humi = humi;
        g_temp = temp;
        sleep(5);
    }
}

int temp_humi_init()
{
    osThreadAttr_t attr;
    attr.name = "temp_humi_task"; //"task_main_entry"
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 4096;
    attr.priority = 20;
    if (osThreadNew((osThreadFunc_t)temp_humi_task, NULL, &attr) == NULL)
    {
        printf("Failed to create tcp_server_task!\n");
    }
}

APP_FEATURE_INIT(temp_humi_init);