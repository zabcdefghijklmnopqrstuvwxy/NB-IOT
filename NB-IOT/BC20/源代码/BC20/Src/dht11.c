/**
********************************************************************************
* @file    dht11.c
* @author  Sai
* @version 01V01
* @date    2020-05-24
* @brief   dht11.c
* @note    温湿度数据处理源文件
* @attention
********************************************************************************
*/

#include "dht11.h"
#include "main.h"
#include "cmsis_os.h"
#include "dwt_delay.h"


#define		DHT11_DQ_SET 		HAL_GPIO_WritePin(DHT11_GPIO_Port,DHT11_Pin,GPIO_PIN_SET)     /**< IO电平拉高 */
#define		DHT11_DQ_RST 		HAL_GPIO_WritePin(DHT11_GPIO_Port,DHT11_Pin,GPIO_PIN_RESET)   /**< IO电平拉低 */
#define		DHT11_DQ_READ		HAL_GPIO_ReadPin(DHT11_GPIO_Port,DHT11_Pin)					  /**< IO电平读取 */


void DHT11_DelayUs(uint32_t us)
{		
	HAL_RCC_GetHCLKFreq();	 
}


/**
 * @brief 温湿度传感器IO设置成输出引脚
 * @param None
 * @note 
 * @retval None
 */
void DHT11_IOOut(void)	
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	HAL_GPIO_DeInit(DHT11_GPIO_Port,DHT11_Pin);	
	GPIO_InitStruct.Pin = DHT11_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;	
	HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

/**
 * @brief 温湿度数据监控任务
 * @param[in] argument 任务参数
 * @note 实时采集温湿度数据
 * @retval 无
 */
void DHT11_IOIn(void)	
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	HAL_GPIO_DeInit(DHT11_GPIO_Port,DHT11_Pin);	
	GPIO_InitStruct.Pin = DHT11_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

/**
 * @brief 温湿度采集数据开始
 * @param None
 * @note 主机发送一个20~40us的低脉冲
 * @retval 无
 */
void DHT11_Rst(void)	   
{                 
	DHT11_IOOut(); 		//SET OUTPUT
    DHT11_DQ_RST; 		//拉低DQ
    osDelay(20);    	//拉低至少18ms
    DHT11_DQ_SET; 		//DQ=1 
	DWT_DelayUs(30);    //主机拉高20~40us
}

/**
 * @brief 应答检查
 * @param None
 * @note 主机等待从机的应答
 * @retval 返回1未检测到DHT11的存在，返回0表示检测到DHT11的存在
 */
uint8_t DHT11_Check(void) 	   
{   
	uint8_t ucRetry=0;
	DHT11_IOIn();//SET INPUT	 
    while (DHT11_DQ_READ&&ucRetry<100)  //DHT11会拉低40~80us
	{
		ucRetry++;
		DWT_DelayUs(1);
	}	
	
	if(ucRetry>=100)
	{
		return 1;
	}
	else 
	{
		ucRetry=0;
	}
	
    while (!DHT11_DQ_READ&&ucRetry<100)//DHT11拉低后会再次拉高40~80us
	{
		ucRetry++;
		DWT_DelayUs(1);
	}
	
	if(ucRetry>=100)
	{
		return 1;	    
	}
	
	return 0;
}

/**
 * @brief 读取一位数据
 * @param None
 * @note None
 * @retval 返回1未检测到DHT11的存在，返回0表示检测到DHT11的存在
 */
uint8_t DHT11_Read_Bit(void) 			 
{
 	uint8_t ucRetry=0;
	while(DHT11_DQ_READ&&ucRetry<100)//等待变为低电平
	{
		ucRetry++;
		DWT_DelayUs(1);
	}
	
	ucRetry=0;
	while(!DHT11_DQ_READ&&ucRetry<100)//等待变高电平
	{
		ucRetry++;
		DWT_DelayUs(1);
	}
	DWT_DelayUs(40);//等待40us
	if(DHT11_DQ_READ)
	{
		return 1;
	}
	else 
	{
		return 0;
	}	
}

/**
 * @brief 从DHT11中读取一个字节的数据
 * @param None
 * @note None
 * @retval 返回读取到数据
 */
uint8_t DHT11_Read_Byte(void)    
{        
    uint8_t i,ucData;
    ucData=0;
	for (i=0;i<8;i++) 
	{
   		ucData<<=1; 
	    ucData|=DHT11_Read_Bit();
    }						    
    return ucData;
}

/**
 * @brief 从DHT11中温湿度数据
 * @param[in] temp温度数据指针，humi湿度数据指针
 * @note None
 * @retval 返回0表示正常，返回1表示读取失败
 */
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi)    
{        
 	uint8_t buf[5];
	uint8_t i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//读取40位数据
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
		}
	}
	else
	{
		return 1;
	}
	
	return 0;	    
}

/**
 * @brief DHT11初始化
 * @param None
 * @note 检测DHT11设备是否存在
 * @retval 返回0表示正常，返回1表示不存在
 */   	 
uint8_t DHT11_Init(void)
{   
	DHT11_Rst();
	return DHT11_Check();
}

/**
 * @brief 温湿度数据监控任务
 * @param[in] argument 任务参数
 * @note 实时采集温湿度数据
 * @retval 无
 */
void DHT11_MonitorTask(void const * argument)
{
	DHT11_Init();
	uint8_t ucTemp = 0;
	uint8_t ucHumi = 0;
	while(1)
	{
		DHT11_Read_Data(&ucTemp,&ucHumi);
		osDelay(10);
	}
}
