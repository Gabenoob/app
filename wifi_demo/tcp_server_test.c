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

#include "iot_gpio.h"
#include "iot_gpio_ex.h"
// #include "iot_pwm.h"



#define CONFIG_WIFI_SSID "LIANGDONGWUYAO_Wi-Fi5" // 修改为自己的WiFi 热点账号
#define CONFIG_WIFI_PWD "AA6638AA" // 修改为自己的WiFi 热点密码

#define TCP_LISTEN_PORT           12345 // TCP服务器监听端口
#define IP_TCP_SERVER_LISTEN_NUM  4  // TCP最大连接数


static int g_listen_fd = 0;
int client_fd = 0;
char recvbuf[512] = {0};
const char *qihangsendmsg = "Hello! Received your message. This is qihang TCP Server!";

#define LED_GPIO_IDX 7
#define MOTOR_GPIO_IDX 2
#define MOTOR_PWM_CHN 2

int led_control(int onoff)
{
    IoTGpioSetOutputVal(LED_GPIO_IDX,onoff ? 1 : 0);
}

int buzzer_control(int onoff)
{
    int i=10000000;

    if(onoff==0){
        IoTPwmStop(2);
        // IoTGpioSetOutputVal(BUZZER_GPIO_IDX,0);
    }else{
        // while(i > 0){
        //     IoTGpioSetOutputVal(BUZZER_GPIO_IDX,1);
        //     usleep(1);
        //     IoTGpioSetOutputVal(BUZZER_GPIO_IDX,0);
        //     usleep(1);
        //     i--;
        // }

        IoTPwmStart(2, 50, 4000);
    }
}

int motor_speed(int speed){

    if(speed <0){
        speed = 0;
    }
    int level = (speed > 5) ? 5 : speed ;
    

    if(speed == 0){
        //close motor
        IoTPwmStop(MOTOR_PWM_CHN);
    }else{
        IoTPwmStart(MOTOR_PWM_CHN, level, 40000);
    }

}

/*任务执行函数，启动TCP服务器端*/
static int tcp_server_task() {
    struct sockaddr_in srv_addr = {0};
    struct sockaddr_in client_addr = {0};
    int ret = 0;
    unsigned int opt = 1;

    IoTGpioInit(LED_GPIO_IDX);
    IoTGpioSetFunc(LED_GPIO_IDX, IOT_GPIO_FUNC_GPIO_7_GPIO);
    IoTGpioSetDir(LED_GPIO_IDX,IOT_GPIO_DIR_OUT);
    // led_control(1);

    // IoTGpioInit(MOTOR_GPIO_IDX);
    // IoTGpioSetFunc(MOTOR_GPIO_IDX, IOT_GPIO_FUNC_GPIO_2_PWM2_OUT);
    // IoTGpioSetDir(MOTOR_GPIO_IDX,IOT_GPIO_DIR_OUT);
    // IoTPwmInit(MOTOR_PWM_CHN);
    // buzzer_control(1);
    // sleep(5);
    // buzzer_control(0);
    // 连接wifi
    WifiConnect(CONFIG_WIFI_SSID, CONFIG_WIFI_PWD);
    // sleep(2);

    // 创建socket
    g_listen_fd = socket(AF_INET,SOCK_STREAM,0);
    if (g_listen_fd == -1) {
        printf("tcp_server_task: create socket failed!\r\n");
        return -1;
    } else {
        printf("tcp_server_task: create socket success!\r\n");
    }

    // 设置socket参数
    ret = setsockopt(g_listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (ret == -1) {
        printf("tcp_server_task: set socket option failed!\r\n");
        return -1;
    } else {
        printf("tcp_server_task: set socket option success!\r\n");
    }

    // 绑定端口
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_addr.sin_port = htons(TCP_LISTEN_PORT);
    ret = bind(g_listen_fd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
    if (ret != 0) {
        printf("tcp_server_task:bind failed, return is %d\r\n", ret);
        closesocket(g_listen_fd);
        g_listen_fd = -1;
        return -1;
    } else {
        printf("tcp_server_task: bind success!\r\n");
    }

    // 监听端口
    ret = listen(g_listen_fd,0);
    if (ret != 0) {
        printf("tcp_server_task:listen failed, return is %d\r\n", ret);
        closesocket(g_listen_fd);
        g_listen_fd = -1;
        return -1;
    } else {
        printf("tcp_server_task: listen success!\r\n");
    }

    // 简单处理，循环处理各客户端连接请求
    while (1)
	{
		int sin_size = sizeof(struct sockaddr_in);
        ret = accept(g_listen_fd,(struct sockaddr *)&client_addr,(socklen_t *)&sin_size);
		if (ret == -1)
		{
			printf("tcp_server_task:accept failed!\r\n");
			continue;
		}
        client_fd = ret;
        printf("tcp_server_task:accept client addr: %s\r\n", inet_ntoa(client_addr.sin_addr));

		//处理目标
		ssize_t ret = 0;
        int count = 0;
		while (1)
		{
            char sendStr[128]={0};
            count++;
            memset_s(recvbuf,sizeof(recvbuf),0,sizeof(recvbuf));
		    // sleep(2);
			if ((ret = recv(client_fd,recvbuf,sizeof(recvbuf),0)) == -1)
			{
				printf("tcp_server_task:recv failed!\r\n");
                break;
			} 

			printf("recvbuf:%s\r\n",recvbuf);

            if(strncmp(recvbuf,"led:",4) == 0){
                 if(strncmp(&recvbuf[4],"on",2) == 0){
                    printf("set led on\n");
                    led_control(1);
                    sprintf_s(sendStr,sizeof(sendStr),"led on ok\n");
                 }else if(strncmp(&recvbuf[4],"off",3) == 0){
                    printf("set led off\n");
                    led_control(0);
                    sprintf_s(sendStr,sizeof(sendStr),"led off ok\n");
                 }else{
                    printf("unkonow command\n");
                    sprintf_s(sendStr,sizeof(sendStr),"unkonow led command\n");
                 }
   
            }
            /*else if(strncmp(recvbuf,"motor:",6) == 0){
                int speed=atoi(&recvbuf[6]);
                printf("motor set speed %d\n",speed);
                sprintf_s(sendStr,sizeof(sendStr),"motor set speed %d ok\n",speed);
                motor_speed(speed);
            }*/
            
            else{

                printf("unsupport order!");
                sprintf_s(sendStr,sizeof(sendStr),"unsupport order!\n");

            }

            // if(recvbuf[0]=='0')
            // {
            //     printf("==> set 0\n");
            //     led_control(0);
            //     buzzer_control(0);

            // }else{

            //     printf("==> set 1\n");
            //     led_control(1);
            //     buzzer_control(1);
            // }

			// sleep(2);
            // sprintf_s(sendStr,sizeof(sendStr),"%s ++ %d\n",recvbuf,count);
			if ( (ret = send(client_fd,sendStr,strlen(sendStr),0) ) == -1)
			{
				printf("tcp_server_task:send failed!\r\n");
                break;
			} else {
				printf("send :%s\r\n", sendStr);
			}
		}
	}

    closesocket(g_listen_fd);
    g_listen_fd = -1;

    return 0;
}

void tcp_server_demo(void)
{
    osThreadAttr_t attr;
    attr.name = "tcp_server_task";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 4096;
    attr.priority = 20;
    if (osThreadNew((osThreadFunc_t)tcp_server_task, NULL, &attr) == NULL) {
        printf("Failed to create tcp_server_task!\n");
    }
}

APP_FEATURE_INIT(tcp_server_demo);
