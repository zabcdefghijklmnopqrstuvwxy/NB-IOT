#ifndef _COMMON_H_
#define	_COMMON_H_

#ifdef __cplusplus
extern "C"{
#endif


#define		COM_MIN(a, b)			(a < b ? a : b)
#define		COM_MAX(a, b)			(a > b ? a : b)


#define		MSG_MAX_SIZE	512 		/**< 消息数据的最大长度 */	
#define		FIFO_MAX_SIZE	16			/**< 消息队列最大容量 */



/**
*@消息队列信息
*/
typedef struct _msg_t
{
	char data[MSG_MAX_SIZE];
}msg_t,*p_msg_t;


/**
*@brief 消息队列数据结构
*/
typedef struct _msg_fifo_t
{
	int nFront;						/**< 队列头 */ 
	int nRear;						/**< 队列尾 */
	msg_t tMsg[FIFO_MAX_SIZE];		/**< 队列消息 */
}fifo_t, *p_fifo_t;


/**
*@COMQUEUE_IsEmpty
*@param pobj 队列实例化对象
*@note 通过判断队头和队尾相等则为控，否为不为空
*@return 返回1表示为空，返回-1表示输入参数非法。
*/
int COMQUEUE_IsEmpty(p_fifo_t pobj);


/**
*@COMQUEUE_IsFull
*@param pobj 队列实例化对象
*@note 通过判断队头+1裕量和队尾相等判断为满，否为不为满
*@return 返回1表示为满，返回-1表示输入参数非法。
*/
int COMQUEUE_IsFull(p_fifo_t pobj);


/**
*@COMQUEUE_Enqueue
*@param pobj 队列实例化对象， fifosize 队列容量大小， msg 入队消息
*@note 将消息插入到队头
*@return 返回0表示入队成功，返回-1表示输入参数非法，返回-2表示队列已满
*/
int COMQUEUE_Enqueue(p_fifo_t pobj, char *msg);

/**
*@COMQUEUE_Dequeue
*@param pobj队列实例化对象， fifosize 队列容量大小， msg 出队消息
*@note 将消息从队尾出队
*@return 返回0表示出队成功，返回-1表示输入参数非法，返回-2表示队列为空
*/
int COMQUEUE_Dequeue(p_fifo_t pobj,  char *msg);

#ifdef __cplusplus
}
#endif


#endif
