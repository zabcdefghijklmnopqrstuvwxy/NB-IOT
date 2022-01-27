#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/ioctl.h>
#include<net/if.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include"mqttserver.h"
#include"mosquitto.h"
#include"elog.h"
#include"common.h"
#include"cJSON.h"
#include"sqlWrapper.h"

#define		LOG_TAG			"mqttserver"

#define		UNIT_TEST			0

#define		SUB_TOPIC_UPLINK			"/uplink"


fifo_t tQueue;	/**< 消息队列实例化 */		

static struct mosquitto *mosq;

int int32ToIp(unsigned int ip, char *addrstr)
{
	if(!addrstr)
	{
		log_w("input param invaild\n");
		return -1;
	}
	
	struct sockaddr_in addr;
	addr.sin_addr.s_addr = htonl(ip);

	strcpy((char*)addrstr, (char*)inet_ntoa(addr.sin_addr));	

	return 0;
}

int getIpAddr(char *ifname)
{
	int fd,ip;

	struct ifreq ifr;
	struct sockaddr_in *sin;
	
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(fd < 0)
	{
		log_w("open socket failed\n");
		return -1;
	}

	memset(&ifr, 0, sizeof(struct ifreq));
	strcpy(ifr.ifr_name, ifname);

	if(ioctl(fd, SIOCGIFADDR, &ifr) < 0)
	{
		log_w("ioctl SIOCGIFADDR failed\n");
		close(fd);
		return -2;
	}
	
	sin = (struct sockaddr_in*)&(ifr.ifr_addr);

	ip = htonl(sin->sin_addr.s_addr);

	return ip;
}

void *msgLoopThread(void *param)
{
	while(1)
	{
		mosquitto_loop(mosq, -1, 1);
		usleep(30*1000);	
	}
}


void message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
	log_i("recv topic is %s\n message is %s\n", message->topic, message->payload);
	
	COMQUEUE_Enqueue(&tQueue, message->payload);
}

void connect_callback(struct mosquitto *mosq, void *userdata, int result)
{
	log_w("connect callback\n");
	mosquitto_subscribe(mosq, NULL, SUB_TOPIC_UPLINK, 2);
}

int jsonparse(char *buf)
{
	if(!buf)
	{
		log_w("input param invaild\n");
		return -1;
	}

	log_i("access in json parse msg is %s\n", buf);

	int nCmd = 0;
	char *table = "NBDEV";
	char *key[4] = {"devno", "timestamp", "gps", "tempeature"};
	char *value[4];
	char timestamp[64];

	cJSON * root = NULL, *item = NULL, *devitem = NULL;
	cJSON *gpsitem = NULL, *tempitem = NULL;
	root = cJSON_Parse(buf);

	if(!root)
	{
		log_w("parse json failed\n");
		return -2;
	}

	item = cJSON_GetObjectItem( root , "cmd" );
	if(!item)
	{
		cJSON_Delete(root);
		log_e("cat not find profile cmd\n");
		return -2;
	}

	nCmd = item->valueint;

	switch(nCmd)
	{
		case 100:
		{
			item = cJSON_GetObjectItem( root , "payload" );
			if(!item)
			{
				cJSON_Delete(root);
				log_e("cat not find profile payload\n");
				return -3;
			}

			devitem = cJSON_GetObjectItem( item , "devno" );
			
			if(!devitem || !devitem->valuestring)
			{
				cJSON_Delete(root);
				log_e("cat not find profile devno\n");
				return -3;
			}	
			log_i("devno is %s\n",devitem->valuestring); 

			gpsitem = cJSON_GetObjectItem( item , "gps" );
			if(!gpsitem || !gpsitem->valuestring)
			{
				cJSON_Delete(root);
				log_e("cat not find profile gps\n");
				return -3;
			}
			log_i("gps is %s\n",gpsitem->valuestring); 
			
			tempitem = cJSON_GetObjectItem( item , "tempeature" );
			if(!tempitem || !tempitem->valuestring)
			{
				cJSON_Delete(root);
				log_e("cat not find profile tempature\n");
				return -3;
			}
		
			log_i("tempitem is %s\n",tempitem->valuestring); 
		}break;
		default:;
	}

	memset(timestamp, 0, sizeof(timestamp));
	sprintf(timestamp, "%d", time(NULL));

	value[0] = strdup((const char*)devitem->valuestring);
	value[1] = strdup((const char*)timestamp);
	value[2] = strdup((const char*)gpsitem->valuestring);
	value[3] = strdup((const char*)tempitem->valuestring);
	
	sqlWrapper_Insert(table, 4, key, value);
	
	cJSON_Delete(root);

	return 0;
}

void *recvProcess(void *param)
{
	char msg[512];	

	while(1)
	{
		int nRet = 0;
		memset(msg, 0, sizeof(msg));
		nRet = COMQUEUE_Dequeue(&tQueue, msg);

		if(nRet != 0)
		{
			usleep(100*1000);
			continue;
		}

		jsonparse(msg);
		usleep(100*1000);
	}	
}

/*
*@brief MQTTSERVER_Init
*@param None
*@note mqtt初始化，连接broker
*@return 返回0表示成功，返回-1表示创建客户端实例失败，返回-2表示连接平台失败
*/
int MQTTSERVER_Init(void)
{
	pthread_t tLoopThread;
	pthread_t tRecvprocessThread;
	int nRet = 0;
	char ip[40];
	bool clean_session = true;
	mosquitto_lib_init();

	mosq = mosquitto_new(NULL, clean_session, NULL);
	
	if(mosq)
	{
		mosquitto_reinitialise(mosq, NULL, clean_session, NULL);
		mosquitto_connect_callback_set(mosq, connect_callback);
		mosquitto_message_callback_set(mosq, message_callback);
	//	mosquitto_subscribe_callback_set(mosq, subscribe_callback);
	//	mosquitto_publish_callback_set(mosq, publish_callback);
	}
	else
	{
		log_w("new mosquitto failed\n");
		return -1;
	}

	memset(ip, 0, sizeof(ip));
	mosquitto_username_pw_set(mosq, "root", "123456");
	int32ToIp(getIpAddr((char*)WIRED_NET), (uint8_t*)ip);
	
	nRet = mosquitto_connect(mosq, ip, PORT, KEEP_ALIVE);
	
	if(nRet != MOSQ_ERR_SUCCESS)
	{
		log_w("connect mqtt broker failed\n");
		return -2;
	}

	if(pthread_create(&tLoopThread, NULL, msgLoopThread, 0)!= 0)
	{
		log_w("create pthread msgLoopThread failed\n");
		return -3;
	}
	
	pthread_detach(tLoopThread);

	if(pthread_create(&tRecvprocessThread, NULL, recvProcess, 0)!= 0)
	{
		log_w("create pthread recvProcess failed\n");
		return -4;
	}
	
	pthread_detach(tRecvprocessThread);

	return 0;
}

#if UNIT_TEST
int main(void)
{
	MQTTSERVER_Init();
	
	while(1);
}
#endif
