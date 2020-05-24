/*
********************************************************************************
* @file    dwt_delay.h
* @author  Sai
* @version 01V01
* @date    2020-05-24
* @brief   dwt_delay.h
* @note    跟踪计数器延时头文件 
* @attention
********************************************************************************
*/

#ifndef	_DWT_DELAY_H_
#define	_DWT_DELAY_H_

#ifdef __cplusplus
extern "C" {
#endif
 
#include "stm32f1xx_hal.h"
 
/**
* @brief 初始化DWT的时钟计数
* @param None
* @note None
* @retval 返回1表示时钟计数没有开始，返回0表示正常工作
*/
uint32_t DWT_InitDelay(void);
 
 /**
* @brief 微妙级别的延迟处理
* @param[in] microseconds延迟的微妙数
* @note None
* @retval None
*/
__STATIC_INLINE void DWT_DelayUs(volatile uint32_t microseconds)
{
  uint32_t clk_cycle_start = DWT->CYCCNT;
 
  /* Go to number of cycles for system */
  microseconds *= (HAL_RCC_GetHCLKFreq() / 1000000);
 
  /* Delay till end */
  while ((DWT->CYCCNT - clk_cycle_start) < microseconds);
}
 
#ifdef __cplusplus
}
#endif
 
#endif
