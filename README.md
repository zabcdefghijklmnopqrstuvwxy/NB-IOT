# 基于NB-IOT的传感器数据采集

## 概述
采集GNSS以及温湿度信息通过NB-IOT传输到云平台，云平台后端保存数据到Mysql中，web页面定时显示具体数据信息。

## 架构
* NBIOT 设备终端采集GPS以及温湿度数据并上传到服务器端
* MqttServer 服务器端将设备端上传的数据保存到Mysql数据库中
* Web Web读取Mysql数据库中最后100条数据并定时实时显示于Web页面

## 技术说明
* NBIOT  
设备端采用STM32F103,通过C语言编程，使用Freertos实时系统来定时采集传感器数据,NB模块使用BC20模块(自带了GNSS)  

* MqttServer  
服务器端建立MQTT服务程序，通过C语言编程，接收到的数据使用Json格式传输，服务端将接收到的Json格式解析  
并保存到Mysql当中。  

* Web  
Web端使用Python+CGI+Html+Mysql+Tomcat。Tomcat的Http服务器，接收到Http请求转到CGI的python后端处理，读取Mysql数据显示到Web页面上。
Html会定时60秒显示。

Web链接为 47.104.60.116:8080/nbiot


