#include <stdio.h>  // for printf
#include <string.h> // for memory operation
#include <stdlib.h>
#include <unistd.h>  // for sleep
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "lwip/netifapi.h"
#include "lwip/api_shell.h"
#include "lwip/sockets.h"
#include "wifi_sta_connect.h"


#define CONFIG_WIFI_SSID "MY_PC" // 修改为自己的WiFi 热点账号
#define CONFIG_WIFI_PWD "12345678" // 修改为自己的WiFi 热点密码

#define PEER_TCP_PORT           3861 // TCP服务器监听端口
#define PEER_TCP_IP    "192.168.43.106"

int client_fd = 0;
unsigned char recvbuf[512] = {0};
const char *qihangsendmsg = "Hello! Received your message. This is qihang TCP Server!";

/*任务执行函数，启动TCP服务器端*/
static int tcp_client_task() {
    struct sockaddr_in srv_addr = {0};
    struct sockaddr_in client_addr = {0};
    int ret = 0;
    unsigned int opt = 1;

    // 连接wifi
    WifiConnect(CONFIG_WIFI_SSID, CONFIG_WIFI_PWD);
 

    // 创建socket
    client_fd = socket(AF_INET,SOCK_STREAM,0);
    if (client_fd == -1) {
        printf("tcp_client_task: create socket failed!\r\n");
        return -1;
    } else {
        printf("tcp_client_task: create socket success!\r\n");
    }

    //设置服务器地址
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = inet_addr(PEER_TCP_IP);
    srv_addr.sin_port = htons(PEER_TCP_PORT);

    //连接服务器
    ret = connect(client_fd,(struct sock_addr *)&srv_addr,sizeof(srv_addr));
    if(ret < 0){
        printf("ERROR:connect failed\n");
        return -1;
    }

    // 简单处理，循环处理各客户端连接请求
    while (1)
	{
        char *str="hello i'm siwei!";
        // unsigned char send_array[32]={0};
        // for(int i = 0;i<sizeof(send_array);i++){
        //     send_array[i]=i;
        // }
        memset_s(recvbuf,sizeof(recvbuf),0,sizeof(recvbuf));
		
        if ((ret = send(client_fd,str,strlen(str),0) ) == -1)
        {
            printf("tcp_client_task:send failed!\r\n");
            break;
        } else {
            printf("send %d bytes:\r\n", strlen(str));
      
        }
        // sleep(2);
        if ((ret = recv(client_fd,recvbuf,sizeof(recvbuf),0)) == -1)
        {
            printf("tcp_client_task:recv failed!\r\n");
            break;
        } else {
            printf("recv :%s\r\n", recvbuf);
            // for(int i = 0;i < ret;i++){
            //     printf("%02x ",recvbuf[i]);
            // }
            // printf("\r\n");
        }
		
	}

    closesocket(client_fd);
    client_fd = -1;

    return 0;
}

void tcp_client_demo(void)
{
    osThreadAttr_t attr;
    attr.name = "tcp_client_task";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 4096;
    attr.priority = 20;
    if (osThreadNew((osThreadFunc_t)tcp_client_task, NULL, &attr) == NULL) {
        printf("Failed to create tcp_client_task!\n");
    }
}

APP_FEATURE_INIT(tcp_client_demo);
