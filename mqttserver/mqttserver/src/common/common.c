#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"common.h"
#include"elog.h"

#define		LOG_TAG			"COMMON"

/**
*@COMQUEUE_IsEmpty
*@param pobj 队列实例化对象
*@note 通过判断队头和队尾相等则为控，否为不为空
*@return 返回1表示为空，返回-1表示输入参数非法。
*/
int COMQUEUE_IsEmpty(p_fifo_t pobj)
{
	if(!pobj)
	{
		return -1;
	}

	if(pobj->nFront == pobj->nRear)
	{
		return 1;
	}

	return 0;
}

/**
*@COMQUEUE_IsFull
*@param pobj 队列实例化对象
*@note 通过判断队头+1裕量和队尾相等判断为满，否为不为满
*@return 返回1表示为满，返回-1表示输入参数非法。
*/
int COMQUEUE_IsFull(p_fifo_t pobj)
{
	if(!pobj)
	{
		return -1;
	}

	if((pobj->nFront + 1)%FIFO_MAX_SIZE == pobj->nRear)
	{
		return 1;
	}

	return 0;
}

/**
*@COMQUEUE_Enqueue
*@param pobj 队列实例化对象， msg 入队消息
*@note 将消息插入到队头
*@return 返回0表示入队成功，返回-1表示输入参数非法，返回-2表示队列已满
*/
int COMQUEUE_Enqueue(p_fifo_t pobj, char *msg)
{
	if(!pobj || !msg)
	{
		log_w("input param invaild\n");
		return -1;
	}

	
	if(COMQUEUE_IsFull(pobj))
	{
		return -2;
	}

	memset(pobj->tMsg[pobj->nFront].data, 0, sizeof(MSG_MAX_SIZE));
	memcpy(pobj->tMsg[pobj->nFront].data, msg, COM_MIN(MSG_MAX_SIZE, strlen(msg)));
	pobj->nFront = (pobj->nFront + 1) % FIFO_MAX_SIZE;

	return 0;
}

/**
*@COMQUEUE_Dequeue
*@param pobj队列实例化对象， msg 出队消息
*@note 将消息从队尾出队
*@return 返回0表示出队成功，返回-1表示输入参数非法，返回-2表示队列为空
*/
int COMQUEUE_Dequeue(p_fifo_t pobj,  char *msg)
{
	if(!pobj || !msg)
	{
		log_w("input param invaild\n");
		return -1;
	}

	if(COMQUEUE_IsEmpty(pobj))
	{
		return -2;
	}

	memcpy(msg, pobj->tMsg[pobj->nRear].data, strlen(pobj->tMsg[pobj->nRear].data));
	pobj->nRear = (pobj->nRear + 1) % FIFO_MAX_SIZE;

	return 0;
}







