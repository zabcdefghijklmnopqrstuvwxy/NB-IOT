/**
********************************************************************************
* @file    dht11.h
* @author  Sai
* @version 01V01
* @date    2020-05-24
* @brief   dht11.h
* @note    温湿度数据头文件 
* @attention
********************************************************************************
*/

#ifndef _DHT11_H_
#define _DHT11_H_

#include "stm32f1xx_hal.h"


/**
 * @brief 温湿度数据监控任务
 * @param[in] argument 任务参数
 * @note 实时采集温湿度数据
 * @retval 无
 */
void DHT11_MonitorTask(void const * argument);


#endif

