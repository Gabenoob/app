# 【项目名称】基于OpenHarmony的智能学习桌面

## 一、项目详细

### 1、项目描述

在当今社会，人们的生活和学习越来越离不开计算机,而随着互联网技术的不断发展、进步以及普及，各种计算机的应用也逐渐进入了我们的日常生活中，同时也有越来越多的人开始使用智能学习设备。因此基于此背景下对智能学习桌面进行研究具有非常重要意义，通过智能学习桌面可明确当日的学习任务，提高学习者的学习效率，并为学习者提供健康提醒。

### 2、项目功能

（1） 包含四个模块：智能风扇模块（包括：单色灯、蜂鸣器、风扇、温湿度传感器、光照传感器）、智能计算器模块、每日学习任务显示模块、智能闹钟模块;
（2） 智能风扇模块：

- 可将温湿度、光照强度的数据上传到华为云IoT平台，并将温湿度数据、光照强度数据实时显示到LCD显示屏中；
- 当温度大于某一特定的值时，蜂鸣器会启动，发出声音；
- 当光照强度小于一定值时，灯光会自动开启；
- 可以通过华为云下发命令或者LCD中的智能风扇模块，打开/关闭照明灯和风扇；
- LCD也可显示灯和风扇的开启状态；

（3） 智能计算器模块：可实现加减乘除运算，为学习者节省计算时间，提高学习效率；
（4） 每日学习任务显示模块：可实现当日学习任务的推送，为学习者明确每日的学习目标；
（5） 智能闹钟模块：可为学习者设置学习和休息提醒，努力学习的同时还要学会休息，劳逸结合；

### 3、项目产出

（1）基于OpenHarmony的智能学习桌面；
__（2）解决联合国17项可持续发展目标中的目标3（良好的健康与福祉：保持身体健康，提醒学习者合理学习和休息的时间）与目标4（优质教育：为学习者提供优质的学习服务）；__
（3）开源完整开发样例代码；
（4）产品的使用说明文档、图片、视频；

### 4、项目主要创新点

LCD显示屏实现当日学习任务的推送，显示每天的学习任务、室内温湿度和光照强度。

## 二、设计思路与方案

### 1、设计思路

#### （1）系统架构图

![Image](image/structure_chart.png)

#### （2）程序流程图

![Image](image/flow_chart.png)

#### （3）软硬件工具

1）软件工具：Docker开发环境、VScode、DevEco Studio、华为ioL平台；
2）硬件工具：BearPi-HM Micro开发板、智能风扇模块、外接蜂鸣器模块;

## 三、应用价值与成果演示

### 1、应用价值

在当今信息量大爆炸的时代，每个人都需要不断学习新知识，不断提升自己的能力。因此，人们需要一个良好的学习环境，使学习者专注于学习。基于此背景下对智能学习桌面进行研究具有非常重要意义，通过智能学习桌面可明确当日的学习任务，提高学习者的学习效率，并为学习者提供健康提醒，为学习者带来更好的智能学习服务。

### 2、演示视频

#### [基于OpenHarmony的智能学习桌面演示视频](https://www.bilibili.com/video/BV1NW4y1b72W/?vd_source=c8ec0727c69a7271029524f93bac4a46)

### 3、整体效果

![Image](image/desktop.png)
![Image](image/fan.png)
![Image](image/huaweiyun2.png)
![Image](image/huaweiyun1.png)
![Image](image/fan_ima.png)
![Image](image/calculator_img.png)
![Image](image/learning_tasks_img1.png)
![Image](image/alarm_img.png)

## 四、快速上手

### 1、视频教程（B站）

[（1）初识BearPi-HM Micro开发板（视频）](https://www.bilibili.com/video/BV12Y411H7ry?p=1)

### 2、文档（Gitee）

[（1）初识BearPi-HM Micro开发板（文档）](https://gitee.com/bearpi/bearpi-hm_micro_app/blob/master/docs/%E5%88%9D%E8%AF%86BearPi-HM_Micro%E5%BC%80%E5%8F%91%E6%9D%BF.md)
[（2）开发环境搭建（南向）](docs/开发环境搭建（南向）.md)
[（3）开发环境搭建（北向）](docs/开发环境搭建（北向）.md)
[（4）如何在开发板上安装hap应用](docs/如何在开发板上安装HAP应用.md)

## 五、开发指导

### 目录介绍

```
├─docs           //说明文档
│ 
├─image    //文档图片
│ 
├─My_calculator_hap  //计算器hap应用代码
│ 
├─My_fan_drive   //智能风扇模块的驱动代码
|
├─My_fan_hap          //智能风扇模块hap应用代码
│ 
├─My_fan_mqtt   //智能风扇模块连接华为云的代码
│ 
└─My_learning_tasks_hap         //学习任务安排表hap应用代码
│ 
└─My_alarm_hap                  //智能闹钟hap应用代码
│ 
└─tools                         //烧入文件
```

#### 1、智能风扇硬件驱动部分

[（1）My_fan_drive驱动代码](https://gitee.com/openharmony-sig/online_event/tree/master/solution_student_challenge/%E5%9F%BA%E4%BA%8EOpenHarmony%E7%9A%84%E6%99%BA%E8%83%BD%E5%AD%A6%E4%B9%A0%E6%A1%8C%E9%9D%A2-%E9%99%88%E6%B2%9B%E6%B8%9D/My_fan_drive)
[（2）My_fan_drive驱动开发文档](docs/驱动开发文档.md)

#### 2、智能风扇硬件连接华为云代码

[（1）My_fan_mqtt代码](https://gitee.com/openharmony-sig/online_event/tree/master/solution_student_challenge/%E5%9F%BA%E4%BA%8EOpenHarmony%E7%9A%84%E6%99%BA%E8%83%BD%E5%AD%A6%E4%B9%A0%E6%A1%8C%E9%9D%A2-%E9%99%88%E6%B2%9B%E6%B8%9D/My_fan_mqtt)
[（2）开发中涉及到的第三方组件移植文件](https://github.com/eclipse/paho.mqtt.embedded-c)

#### 3、智能风扇

[（1）My_fan_hap应用代码](https://gitee.com/openharmony-sig/online_event/tree/master/solution_student_challenge/%E5%9F%BA%E4%BA%8EOpenHarmony%E7%9A%84%E6%99%BA%E8%83%BD%E5%AD%A6%E4%B9%A0%E6%A1%8C%E9%9D%A2-%E9%99%88%E6%B2%9B%E6%B8%9D/My_fan_hap)
[（2）My_fan_hap应用开发文档](docs/智能风扇hap应用开发.md)

#### 4、智能计算器

[（1）My_calculator_hap应用代码](https://gitee.com/openharmony-sig/online_event/tree/master/solution_student_challenge/%E5%9F%BA%E4%BA%8EOpenHarmony%E7%9A%84%E6%99%BA%E8%83%BD%E5%AD%A6%E4%B9%A0%E6%A1%8C%E9%9D%A2-%E9%99%88%E6%B2%9B%E6%B8%9D/My_calculator_hap)
[（2）My_calculator_hap应用开发文档](docs/计算器hap应用开发.md)

#### 5、学习任务安排表

[（1）My_learning_tasks_hap应用代码](https://gitee.com/openharmony-sig/online_event/tree/master/solution_student_challenge/%E5%9F%BA%E4%BA%8EOpenHarmony%E7%9A%84%E6%99%BA%E8%83%BD%E5%AD%A6%E4%B9%A0%E6%A1%8C%E9%9D%A2-%E9%99%88%E6%B2%9B%E6%B8%9D/My_learning_tasks_hap)
[（2）My_learning_tasks_hap应用开发文档](docs/学习任务安排表hap应用开发.md)

#### 6、智能闹钟

[（1）My_alarm_hap应用代码](https://gitee.com/openharmony-sig/online_event/tree/master/solution_student_challenge/%E5%9F%BA%E4%BA%8EOpenHarmony%E7%9A%84%E6%99%BA%E8%83%BD%E5%AD%A6%E4%B9%A0%E6%A1%8C%E9%9D%A2-%E9%99%88%E6%B2%9B%E6%B8%9D/My_alarm_hap)
[（2）My_alarm_hap应用开发文档](docs/智能闹钟hap应用开发.md)

## 六、常见问题解决

### 1、hap应用的界面跳转问题

在hap应用中界面的跳转设置时，需要在config.json中加入跳转界面的路径，操作步骤如下图所示。
![Image](image/problem1.png)

### 2、蜂鸣器引脚绑定错误

解决办法：

- 查看板子的原理图，找到板子上GPIO引脚和E53_IO的对应关系
- 将查找原理图，蜂鸣器绑定的引脚是PB5，对应的数字是21，而21对应E53_IO_10，如下图所示：
![Image](image/01.png)
这样我就找到要自己蜂鸣器绑定的GPIO引脚了

### 3、WiFi自动连接失败

解决办法：
（1）修改applications/sample/camera/communication/wpa_supplicant/src/wpa_sample.c
主要修改如下图，在main函数中添加启动的参数：
![Image](image/02.png)

（2）其次，还需要进行一个延时处理，因为开机启动的时候wlan0初始化需要时间，如下图，我这里在线程里面延时了15s，经测试可以正常启动并连接WiFi，你可以根据板子的情况设置相应的延时长度。
![Image](image/03.png)

（3）添加开机启动项，修改vendor/hisilicon/hispark_aries/init_configs/init_liteos_a_3518ev300.cfg，这个也是一样的，修改板子对应的cfg就进行了。如下图：
![Image](image/04.png)

（4）init中添加start wpa_supplicant，post-init中添加chmod 0666 /dev/hdf/hdfwifi，第二个是修改权限，因为在job里运行的用户和在shell里面运行的用户是不一样的。不这样修改就会报下图那个错，同时这个可能也存在版本差异，有的版本可能是/dev/hdfwifi，可以根据报错自行修改。
![Image](image/06.png)

（5）添加service，如下图：
![Image](image/05.png)

（6）如下图，程序在驱动初始化之后运行，并成功初始化：

![Image](image/07.png)

## 七、参考资料

__[BearPi-HM Micro南向开发](https://gitee.com/bearpi/bearpi-hm_micro_small)__
__[BearPi-HM Micro北向开发](https://gitee.com/bearpi/bearpi-hm_micro_app)__

&emsp;[回到文章顶部](#文章顶部)
