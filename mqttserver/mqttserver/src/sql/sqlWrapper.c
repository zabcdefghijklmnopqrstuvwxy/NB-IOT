#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <unistd.h>
#include "sqlWrapper.h"
#include "elog.h"

#define		LOG_TAG			"sqlWrapper"

#define		SQL_UNIT_TEST			0

MYSQL *sql;

/**
*@brief sqlWrapper_init()
*@param None
*@note 连接本地mysql，创建数据库NBIOT，创建表NBDEV。
*@return 返回0表示成功，返回-1表示连接本地mysql失败，返回-2表示创建数据库失败，返回-3表示创建表失败
*/
int sqlWrapper_Init(void)
{

   	char *server = "localhost";
   	char *user = "root";
   	char *password = "123456"; /* 此处改成你的密码 */
    char *database = "NBIOT";
	unsigned int port = 1883;

    sql = mysql_init(NULL);

   /* Connect to database */
    if (!mysql_real_connect(sql, server,
          user, password, database, port, NULL, 0)) {
      log_w("%s\n", mysql_error(sql));
      return -1;
    }	

	return 0;	
}

/**
*@brief sqlWrapper_Insert
*@param table 表名称，keycnt 插入的字段个数，key插入的关键字，value 插入的数值
*@note 将key和value组成sql语句的格式然后调用sqlclient的API接口执行
*@return 返回0表示插入成功，返回-1表示输入参数非法
*/
int sqlWrapper_Insert(char *table, char keycnt, char *key[], char *value[])
{
	int i = 0;
	char sqlbuf[512];
	char keybuf[128];
	char valuebuf[512];

	memset(sqlbuf, 0, sizeof(sqlbuf));
	memset(keybuf, 0, sizeof(keybuf));
	memset(valuebuf, 0, sizeof(valuebuf));

	for(i=0;i<keycnt;i++)
	{
		if(i == keycnt -1)
		{
			snprintf(keybuf+strlen(keybuf), 128-strlen(keybuf), "%s", key[i]);
			snprintf(valuebuf+strlen(valuebuf), 512-strlen(valuebuf), " \"%s\"", value[i]);
		}
		else
		{
			snprintf(keybuf+strlen(keybuf), 128-strlen(keybuf), "%s\,", key[i]);
			snprintf(valuebuf+strlen(valuebuf), 512-strlen(valuebuf), " \"%s\",", value[i]);
		}
		log_d("strlen(keybuf) is %d\t strlen(valuebuf) is %d\n", strlen(keybuf), strlen(valuebuf));
	}
	snprintf(sqlbuf,512,"insert into %s (%s) values (%s);", table, keybuf, valuebuf);

	log_i("insert sql is %s\n",sqlbuf);
	
	return mysql_real_query(sql, sqlbuf, strlen(sqlbuf));
}

#if	SQL_UNIT_TEST
int main()
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char server[] = "localhost";
    char user[] = "root";
    char password[] = "123456";
    char database[] = "mysql";
   
    conn = mysql_init(NULL);
   
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
   
    if (mysql_query(conn, "show tables"))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
   
    res = mysql_use_result(conn);
   
    printf("MySQL Tables in mysql database:\n");
   
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        printf("%s \n", row[0]);
    }
   
    mysql_free_result(res);
    mysql_close(conn);
   
    printf("finish! \n");
    return 0;
}
#endif

