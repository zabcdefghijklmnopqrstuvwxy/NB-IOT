#include "CustomTCPConnection.hpp"
#include "Poco/Util/Application.h"
#include "Poco/Exception.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/Object.h"
#include<iostream>
#include<time.h>

using namespace std;
using namespace Poco;
char cRecvBuf[1024];
static int nClientIndicate = 0; 
PocoMySQL *CustomTCPConnection::m_instance = NULL;
bool CustomTCPConnection::bConnectState = false;

typedef Poco::Tuple<std::string, Int32, std::string, std::string> NBDEV;


CustomTCPConnection::CustomTCPConnection(const StreamSocket& s):
TCPServerConnection(s)
{
}

PocoMySQL* CustomTCPConnection::GetMysql()
{
	if(m_instance)
	{
		return m_instance;
	}
	else
	{
		m_instance = new PocoMySQL("127.0.0.1",3306,"root","123456","NBIOT");
		bConnectState = m_instance->connect();
		return m_instance;
	}
}

void CustomTCPConnection::run()
{
	int num = 0;
    Poco::Util::Application& app = Poco::Util::Application::instance();
    // 日志输出连接的TCP用户的地址（IP和端口）
	nClientIndicate++;
	string sql;
	char buf[256];
    app.logger().information("Request from " + this->socket().peerAddress().toString());
	while(1)
	{
    	try
    	{
			num = socket().receiveBytes(cRecvBuf,sizeof(cRecvBuf));
			if(num > 0)
			{
				if(GetMysql())
				{
					if(!bConnectState)
					{
    					app.logger().information("mysql connect failed\n");
						break;
					}
				}

    			app.logger().information("receive info is \t" + std::string(cRecvBuf));
				JSON::Parser parse;
				JSON::Object::Ptr objptr = parse.parse(std::string(cRecvBuf)).extract<JSON::Object::Ptr>();
				JSON::Object::Ptr jsonptr = objptr->getObject("data");
				memset(buf,0,sizeof(buf));
				if(jsonptr)
				{
					std::string gnss = jsonptr->get("gnss");
					std::string temp = jsonptr->get("temp");
					Int32 devno = jsonptr->get("devno");

					sprintf(buf,"insert into NBDEV(timestamp,devno,gps,tempuature) values(%d,%d,%s,%s)",time(NULL),devno,gnss.c_str(),temp.c_str());

    				app.logger().information("json data is \t" + std::string(buf));
					cout << "buf:" << std::string(buf) << endl;
					sql = buf;
					GetMysql()->execute(sql);

        			std::string dt("{\"data\":{\"status\":\"OK\"}}");
        			dt.append("\r\n");
       	 			socket().sendBytes(dt.data(), (int) dt.length());
				}
				else
				{
    				app.logger().information("get json data is null");
				}
				memset(cRecvBuf,0,sizeof(cRecvBuf));
			}
    	}
    	catch (Poco::Exception& e)
    	{
        	app.logger().log(e);
    	}
	}
}
