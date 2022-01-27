#ifndef	_SQLWRAPPER_H_
#define	_SQLWRAPPER_H_

#ifdef __cplusplus
extern "C"{
#endif

/**
*@brief sqlWrapper_init()
*@param None
*@note 连接本地mysql，创建数据库NBIOT，创建表NBDEV。
*@return 返回0表示成功，返回-1表示连接本地mysql失败，返回-2表示创建数据库失败，返回-3表示创建表失败
*/
int sqlWrapper_Init(void);

/**
*@brief sqlWrapper_Insert
*@param table 表名称，keycnt 插入的字段个数，key插入的关键字，value 插入的数值
*@note 将key和value组成sql语句的格式然后调用sqlclient的API接口执行
*@return 返回0表示插入成功，返回-1表示输入参数非法
*/
int sqlWrapper_Insert(char *table, char keycnt, char *key[], char *value[]);


#ifdef __cplusplus
}
#endif

#endif
