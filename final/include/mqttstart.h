#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmsis_os2.h"
#include "ohos_init.h"

#include "MQTTClient.h"
#include "wifi_sta_connect.h"

#include "cJSON.h"

// #include "temp_data.h"
#include "motor.h"
#include "light.h"

static unsigned char sendBuf[1000];
static unsigned char readBuf[1000];

#define WIFI_SSID "LIANGDONGWUYAO_Wi-Fi5"
#define WIFI_PWD "AA6638AA"

#define HOST_ADDR "bed309abac.iot-mqtts.cn-north-4.myhuaweicloud.com"

#define Device_Id "6392ef2d98314b7a1c3ef412_test1"

#define PUBLISH_TOPIC "$oc/devices/" Device_Id "/sys/properties/report"
#define SUBCRIB_TOPIC "$oc/devices/" Device_Id "/sys/commands/#"         /// request_id={request_id}"
#define RESPONSE_TOPIC "$oc/devices/" Device_Id "/sys/commands/response" /// request_id={request_id}"

#define MOTOR_GPIO 2

Network network;
MQTTClient client;

void jSONDeCode(cJSON *root);

void messageArrived(MessageData *data);

static void MQTTDemoTask(void);

static void MQTTDemo(void);