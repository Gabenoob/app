#include "mqttstart.h"
int freq = 0;
int value = 40000;
void jSONDeCode(cJSON *root)
{
    extern int mode;
    cJSON *cmd_type = cJSON_DetachItemFromObject(root, "command_name");
    cJSON *paras = cJSON_DetachItemFromObject(root, "paras");
    if (strcmp(cJSON_GetStringValue(cmd_type), "led_cmd") == 0)
    {
        cJSON *led_onoff = cJSON_DetachItemFromObject(paras, "led_onoff");
        char *string = cJSON_GetStringValue(led_onoff);
        if (mode != 4)
            mode = 4;
        sleep(1);
        if (strcmp(string, "BLUE_ON") == 0)
            Led_on_off(LED_ON, LED_GPIO_IDX_BLUE);
        else if (strcmp(string, "BLUE_OFF") == 0)
            Led_on_off(LED_OFF, LED_GPIO_IDX_BLUE);
        else if (strcmp(string, "RED_ON") == 0)
            Led_on_off(LED_ON, LED_GPIO_IDX_RED);
        else if (strcmp(string, "RED_OFF") == 0)
            Led_on_off(LED_OFF, LED_GPIO_IDX_RED);
        else if (strcmp(string, "GREEN_ON") == 0)
        {
            value = 5;
            IoTGpioSetFunc(LED_GPIO_IDX_GREEN, IOT_GPIO_FUNC_GPIO_2_GPIO);
            Led_on_off(LED_ON, LED_GPIO_IDX_GREEN);
        }
        else if (strcmp(string, "GREEN_OFF") == 0)
        {
            value = 0;
            Led_on_off(LED_OFF, LED_GPIO_IDX_GREEN);
        }
        else if (strcmp(string, "WHITE_ON") == 0)
            Led_on_off(LED_ON, LED_GPIO_IDX_WHITE);
        else if (strcmp(string, "WHITE_OFF") == 0)
            Led_on_off(LED_OFF, LED_GPIO_IDX_WHITE);
    }
    else if (strcmp(cJSON_GetStringValue(cmd_type), "motor_control") == 0)
    {
        if (mode != 3)
            mode = 3;
        sleep(1);
        IoTGpioSetFunc(MOTOR_GPIO, IOT_GPIO_FUNC_GPIO_2_PWM2_OUT);
        IoTPwmInit(MOTOR_PWM_CHN);
        cJSON *motor_spd = cJSON_DetachItemFromObject(paras, "motor_speed");
        value = (int)cJSON_GetNumberValue(motor_spd);
        cJSON *motor_freq = cJSON_DetachItemFromObject(paras, "motor_freq");
        freq = (int)cJSON_GetNumberValue(motor_freq);
        motor_speed(value, freq);
    }
    else if (strcmp(cJSON_GetStringValue(cmd_type), "switch_mode") == 0)
    {
        cJSON *modeNO = cJSON_DetachItemFromObject(paras, "mode");
        mode = cJSON_GetNumberValue(modeNO);
    }
}

void messageArrived(MessageData *data)
{
    int rc;
    printf("Message arrived on topic %.*s: %.*s\n", data->topicName->lenstring.len, data->topicName->lenstring.data,
           data->message->payloadlen, data->message->payload);

    /*
        /xx//x/x//x/requets_id=123456
    */

    // get request id
    char *request_id_idx = NULL;
    char request_id[48] = {0};
    request_id_idx = strstr(data->topicName->lenstring.data, "request_id=");
    strncpy(request_id, request_id_idx + 11, 36);
    printf("request_id = %s\n", request_id);

    // create response topic
    char rsptopic[128] = {0};
    sprintf(rsptopic, "%s/request_id=%s", RESPONSE_TOPIC, request_id);
    printf("rsptopic = %s\n", rsptopic);

    // response message
    MQTTMessage message;
    char payload[300];
    message.qos = 0;
    message.retained = 0;
    message.payload = payload;
    sprintf(payload, "{ \
    \"result_code\": 0, \
    \"response_name\": \"COMMAND_RESPONSE\", \
    \"paras\": { \
        \"result\": \"success\" \
    } \
    }");
    message.payloadlen = strlen(payload);

    // publish the msg to responese topic
    if ((rc = MQTTPublish(&client, rsptopic, &message)) != 0)
    {
        printf("Return code from MQTT publish is %d\n", rc);
        NetworkDisconnect(&network);
        MQTTDisconnect(&client);
    }

    cJSON *root = cJSON_ParseWithLength(data->message->payload, data->message->payloadlen);
    if (root != NULL)
    {
        //{"paras":{"led_onoff":"OFF"},"service_id":"szfan","command_name":"led_cmd"}
        //{"paras":{"motor_speed":5},"service_id":"szfan","command_name":"motor_control"}
        jSONDeCode(root);
    }

    cJSON_Delete(root);
}

static void MQTTDemoTask(void)
{

    printf(">> MQTTDemoTask ...\n");
    WifiConnect(WIFI_SSID, WIFI_PWD);
    printf("Starting ...\n");
    int rc, count = 0;

    NetworkInit(&network);
    printf("NetworkConnect  ...\n");

begin:
    NetworkConnect(&network, HOST_ADDR, 1883);
    printf("MQTTClientInit  ...\n");
    MQTTClientInit(&client, &network, 2000, sendBuf, sizeof(sendBuf), readBuf, sizeof(readBuf));

    MQTTString clientId = MQTTString_initializer;
    clientId.cstring = "6392ef2d98314b7a1c3ef412_test1_0_0_2023013108";

    MQTTString userName = MQTTString_initializer;
    userName.cstring = Device_Id; // deviceID

    MQTTString password = MQTTString_initializer;
    password.cstring = "aac5dfb12b861e4557fa84e0defd41be164f08852a32fa33e6685625da614e14";

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.clientID = clientId;

    data.username = userName;
    data.password = password;

    data.willFlag = 0;
    data.MQTTVersion = 4;
    data.keepAliveInterval = 60;
    data.cleansession = 1;

    printf("MQTTConnect  ...\n");
    rc = MQTTConnect(&client, &data);
    if (rc != 0)
    {
        printf("MQTTConnect: %d\n", rc);
        NetworkDisconnect(&network);
        MQTTDisconnect(&client);
        osDelay(200);
        goto begin;
    }
    printf("MQTTSubscribe  ...\n");
    rc = MQTTSubscribe(&client, SUBCRIB_TOPIC, 0, messageArrived);

    if (rc != 0)
    {
        printf("MQTTSubscribe: %d\n", rc);
        osDelay(200);
        goto begin;
    }

    while (++count)
    {

        MQTTMessage message;
        char *publishtopic = PUBLISH_TOPIC;

        char payload[300] = {0};

        cJSON *root = cJSON_CreateObject();
        sleep(1);

        extern float g_temp;
        extern float g_humi;

        if (root != NULL)
        {
            // 自行完成CJSON的组装
            cJSON *services_arr = cJSON_AddArrayToObject(root, "services");
            cJSON *list = cJSON_CreateObject();
            cJSON_AddStringToObject(list, "service_id", "szfan");
            cJSON *properties = cJSON_CreateObject();
            cJSON_AddNumberToObject(properties, "temperature", g_temp);
            cJSON_AddNumberToObject(properties, "humidity", g_humi);
            cJSON_AddItemToObject(list, "properties", properties);
            cJSON_AddItemToArray(services_arr, list);
            char *palyload_str = cJSON_PrintUnformatted(root);
            strcpy(payload, palyload_str);
        }

        message.qos = 0;
        message.retained = 0;
        message.payload = payload;

        message.payloadlen = strlen(payload);

        if ((rc = MQTTPublish(&client, publishtopic, &message)) != 0)
        {
            printf("Return code from MQTT publish is %d\n", rc);
            NetworkDisconnect(&network);
            MQTTDisconnect(&client);
            goto begin;
        }
        else
        {
            printf("mqtt publish success:%s\n", payload);
        }
        MQTTYield(&client, 5000);
        sleep(5);
    }
}

static void MQTTDemo(void)
{
    light_init();
    motor_Init();
    osThreadAttr_t attr;

    attr.name = "MQTTDemoTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 10240;
    attr.priority = osPriorityNormal;

    if (osThreadNew((osThreadFunc_t)MQTTDemoTask, NULL, &attr) == NULL)
    {
        printf("[MQTT_Demo] Failed to create MQTTDemoTask!\n");
    }
}

APP_FEATURE_INIT(MQTTDemo);