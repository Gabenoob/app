/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial API and implementation and/or initial documentation
 *    Sergio R. Caprile - clarifications and/or documentation extension
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmsis_os2.h"
#include "ohos_init.h"

#include "MQTTClient.h"
#include "wifi_sta_connect.h"

#include "cJSON.h"

// #include "temp_data.h"
// #include "motor.h"
// #include "light.h"


static unsigned char sendBuf[1000];
static unsigned char readBuf[1000];

#define WIFI_SSID "LIANGDONGWUYAO_Wi-Fi5"
#define WIFI_PWD "AA6638AA"

#define HOST_ADDR "bed309abac.iot-mqtts.cn-north-4.myhuaweicloud.com"

#define Device_Id "6392ef2d98314b7a1c3ef412_test1"

#define PUBLISH_TOPIC "$oc/devices/"Device_Id"/sys/properties/report"
#define SUBCRIB_TOPIC "$oc/devices/"Device_Id"/sys/commands/#" ///request_id={request_id}"
#define RESPONSE_TOPIC "$oc/devices/"Device_Id"/sys/commands/response" ///request_id={request_id}"


Network network;
MQTTClient client;

void messageArrived(MessageData* data)
{
    int rc;
    printf("Message arrived on topic %.*s: %.*s\n", data->topicName->lenstring.len, data->topicName->lenstring.data,
           data->message->payloadlen, data->message->payload);

    /*
        /xx//x/x//x/requets_id=123456
    */

   //get request id
    char *request_id_idx=NULL;
    char request_id[48]={0};
    request_id_idx = strstr(data->topicName->lenstring.data,"request_id=");
    strncpy(request_id,request_id_idx+11,36);
    printf("request_id = %s\n",request_id);

    //create response topic
    char rsptopic[128]={0};
    sprintf(rsptopic,"%s/request_id=%s",RESPONSE_TOPIC,request_id);
    printf("rsptopic = %s\n",rsptopic);

    //response message
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
    }" );
    message.payloadlen = strlen(payload);

    //publish the msg to responese topic
    if ((rc = MQTTPublish(&client, rsptopic, &message)) != 0) {
        printf("Return code from MQTT publish is %d\n", rc);
        NetworkDisconnect(&network);
        MQTTDisconnect(&client);
        
    }
    
    cJSON *root = cJSON_ParseWithLength( data->message->payload, data->message->payloadlen);
    if(root != NULL){
			//自行完成cjson的解析

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
    userName.cstring = Device_Id; //deviceID

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
    if (rc != 0) {
        printf("MQTTConnect: %d\n", rc);
        NetworkDisconnect(&network);
        MQTTDisconnect(&client);
        osDelay(200);
        goto begin;
    }
    // printf("MQTTSubscribe  ...\n");
    // rc = MQTTSubscribe(&client, SUBCRIB_TOPIC, 0, messageArrived);

    // if (rc != 0) {
    //     printf("MQTTSubscribe: %d\n", rc);
    //     osDelay(200);
    //     goto begin;
    // }

    while (++count) {
        
        float temp;
        float humi;

        // SHT3X_ReadMeasurementBuffer(&temp,&humi);

        MQTTMessage message;
        char *publishtopic=PUBLISH_TOPIC;

        char payload[300]={0};

        cJSON *root = cJSON_CreateObject();
        sleep(1);

        extern float g_temp;
        extern float g_humi;
        // g_humi = humi;
        // g_temp = temp;
        
        if(root != NULL){
			//自行完成CJSON的组装
            cJSON *services_arr = cJSON_AddArrayToObject(root,"services");
            cJSON *service_id = cJSON_CreateString("szfan");
            cJSON *list = cJSON_CreateObject();
            cJSON_AddStringToObject(list,"service_id","szfan");
            cJSON *properties = cJSON_CreateObject();
            cJSON_AddNumberToObject(properties,"temperature",g_temp);
            cJSON_AddNumberToObject(properties,"humidity",g_humi);
            cJSON_AddItemToObject(list,"properties",properties);
            cJSON_AddItemToArray(services_arr,list);
            char *palyload_str = cJSON_PrintUnformatted(root);
            strcpy(payload,palyload_str);
            
        }
     
        message.qos = 0;
        message.retained = 0;
        message.payload = payload;
        
        message.payloadlen = strlen(payload);

        if ((rc = MQTTPublish(&client, publishtopic, &message)) != 0) {
            printf("Return code from MQTT publish is %d\n", rc);
            NetworkDisconnect(&network);
            MQTTDisconnect(&client);
            goto begin;
        }else{
            printf("mqtt publish success:%s\n",payload);
        }
        MQTTYield(&client, 5000);
        osDelay(500);
    }
}    

static void MQTTDemo(void)
{
    osThreadAttr_t attr;

    attr.name = "MQTTDemoTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 10240;
    attr.priority = osPriorityNormal;

    if (osThreadNew((osThreadFunc_t)MQTTDemoTask, NULL, &attr) == NULL) {
        printf("[MQTT_Demo] Failed to create MQTTDemoTask!\n");
    }
}

APP_FEATURE_INIT(MQTTDemo);