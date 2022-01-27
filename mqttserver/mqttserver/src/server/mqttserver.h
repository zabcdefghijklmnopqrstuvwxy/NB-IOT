#ifndef _MQTTSERVER_H_
#define	_MQTTSERVER_H_

#ifdef __cplusplus
extern "C"{
#endif

#define  PORT		  1883 
#define  WIRED_NET	  "lo"
#define	 KEEP_ALIVE	  60		


/*
*@brief MQTTSERVER_Init
*@param None
*@note mqtt初始化，连接broker
*@return 返回0表示成功，返回-1表示创建客户端实例失败，返回-2表示连接平台失败
*/
int MQTTSERVER_Init(void);

#ifdef __cplusplus
}
#endif


#endif


