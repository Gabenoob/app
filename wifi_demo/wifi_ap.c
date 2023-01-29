#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "cmsis_os2.h"
#include "ohos_init.h"

#include "wifi_device.h"
#include "wifi_hotspot.h"
#include "wifi_error_code.h"
#include "lwip/netifapi.h"

// #include "motor.h"
// #include "../temp_humi_demo/temp_data.h"

#define AP_SSID "SD_HWLH6"
#define AP_PSK  "12345678"

#define ONE_SECOND 1
#define DEF_TIMEOUT 15
#define BOOL int

static void OnHotspotStaJoinHandler(StationInfo *info);
static void OnHotspotStateChangedHandler(int state);
static void OnHotspotStaLeaveHandler(StationInfo *info);

static struct netif *g_lwip_netif = NULL;
static int g_apEnableSuccess = 0;
static WifiEvent g_wifiEventHandler = {0};
WifiErrorCode error;

#define UDP_BRODCAST_IPADDR "255.255.255.255"


static BOOL WifiAPTask(void)
{


    //注册wifi事件的回调函数
    g_wifiEventHandler.OnHotspotStaJoin = OnHotspotStaJoinHandler;
    g_wifiEventHandler.OnHotspotStaLeave = OnHotspotStaLeaveHandler;
    g_wifiEventHandler.OnHotspotStateChanged = OnHotspotStateChangedHandler;
    error = RegisterWifiEvent(&g_wifiEventHandler);
    if (error != WIFI_SUCCESS)
    {
        printf("RegisterWifiEvent failed, error = %d.\r\n",error);
        return -1;
    }
    printf("RegisterWifiEvent succeed!\r\n");
    //检查热点模式是否使能
    if (IsHotspotActive() == WIFI_HOTSPOT_ACTIVE)
    {
        printf("Wifi station is  actived.\r\n");
        return -1;
    }
    //设置指定的热点配置
    HotspotConfig config = {0};

    strcpy(config.ssid, AP_SSID);
    strcpy(config.preSharedKey, AP_PSK);
    config.securityType = WIFI_SEC_TYPE_PSK;
    config.band = HOTSPOT_BAND_TYPE_2G;
    config.channelNum = 7;

    error = SetHotspotConfig(&config);
    if (error != WIFI_SUCCESS)
    {
        printf("SetHotspotConfig failed, error = %d.\r\n", error);
        return -1;
    }
    printf("SetHotspotConfig succeed!\r\n");

    //启动wifi热点模式
    error = EnableHotspot(); 
    if (error != WIFI_SUCCESS)
    {
        printf("EnableHotspot failed, error = %d.\r\n", error);
        return -1;
    }
    printf("EnableHotspot succeed!\r\n");

    while(1){
        sleep(1);
    }
    #if 0
    /****************以下为UDP服务器代码,默认IP:192.168.5.1***************/
	//在sock_fd 进行监听
    int sock_fd;
    //服务端地址信息
	struct sockaddr_in server_sock;

    //创建socket
	if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("socket is error.\r\n");
		return -1;
	}

     printf("=== > sock_fd=%d\n",sock_fd);
    const int opt=-1;
    //设置套接字类型
     int nb=setsockopt(sock_fd,SOL_SOCKET,SO_BROADCAST,(char*)&opt,sizeof(opt));
     printf("=== > nb=%d\n",nb);
     if(nb< 0)
     {
         printf("set socket error...\n");
        //  return -1;
     }

	bzero(&server_sock, sizeof(server_sock));
	server_sock.sin_family = AF_INET;
	server_sock.sin_addr.s_addr = htonl(INADDR_ANY);
	server_sock.sin_port = htons(3861);

	//调用bind函数绑定socket和地址
	if (bind(sock_fd, (struct sockaddr *)&server_sock, sizeof(struct sockaddr)) == -1)
	{
		perror("bind is error.\r\n");
		return -1;
	}



    int ret;
    char recvBuf[512] = {0};
    //客户端地址信息
    struct sockaddr_in send_addr;
    int size_send_addr= sizeof(struct sockaddr_in);
    send_addr.sin_family = AF_INET;
    send_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    send_addr.sin_port = htons(3861);

    while (1)
    {
        
        printf("Waiting to send data...\r\n");
        memset(recvBuf, 0, sizeof(recvBuf));
        //格式化数据
        sprintf_s(recvBuf,sizeof(recvBuf),"temp:%.02f,humi:%0.2f\n",g_temp,g_humi);
        printf("send data:%s\r\n",recvBuf);
        //发送数据
        ret = sendto(sock_fd, recvBuf, strlen(recvBuf), 0, 
        (struct sockaddr *)&send_addr, sizeof(send_addr));
        if (ret < 0)
        {
            printf("UDP server send failed!\r\n");
            // return -1;
        }
        sleep(5);
    }
    /*********************END********************/

    #endif

}

static void OnHotspotStaJoinHandler(StationInfo *info)
{
    if (info == NULL) {
    printf("HotspotStaJoin:info is null.\r\n");
    } 
    else {
        static char macAddress[32] = {0};
        unsigned char* mac = info->macAddress;
        snprintf(macAddress, sizeof(macAddress), 
        "%02X:%02X:%02X:%02X:%02X:%02X",
         mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

        printf("HotspotStaJoin: macAddress=%s, reason=%d.\r\n",
         macAddress, info->disconnectedReason);

        g_apEnableSuccess++;
    }
    return;
}

static void OnHotspotStaLeaveHandler(StationInfo *info)
{
    if (info == NULL) {
        printf("HotspotStaLeave:info is null.\r\n");
    } 
    else {
        static char macAddress[32] = {0};
        unsigned char* mac = info->macAddress;
        snprintf(macAddress, sizeof(macAddress), 
        "%02X:%02X:%02X:%02X:%02X:%02X", 
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

        printf("HotspotStaLeave: macAddress=%s, reason=%d.\r\n", 
        macAddress, info->disconnectedReason);
        
        g_apEnableSuccess--;
    }
    return;
}

static void OnHotspotStateChangedHandler(int state)
{
    printf("HotspotStateChanged:state is %d.\r\n", state);
    if (state == WIFI_HOTSPOT_ACTIVE) {
        printf("wifi hotspot active.\r\n");
    } else {
        printf("wifi hotspot noactive.\r\n");
    }
}

static void Wifi_AP_Demo(void)
{
    osThreadAttr_t attr;

    attr.name = "WifiAPTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 10240;
    attr.priority = 25;

    if (osThreadNew((osThreadFunc_t)WifiAPTask, NULL, &attr) == NULL)
    {
        printf("Falied to create WifiAPTask!\r\n");
    }
}

APP_FEATURE_INIT(Wifi_AP_Demo);
