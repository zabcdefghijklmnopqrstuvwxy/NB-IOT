#include "CustomTCPConnection.hpp"
#include "Poco/Util/Application.h"
#include "Poco/Exception.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/Object.h"
#include "CEasyLog.h"
#include<iostream>
#include<time.h>

using namespace std;
using namespace Poco;
using namespace COMMON;

//#define 	HEARTBEAT_TIMEOUT		60L*1000L*1000*3L

#define 	HEARTBEAT_TIMEOUT		60L*1000L*1000L*4

Poco::Buffer<char> cRecvBuf(1024);
//char cRecvBuf[1024];
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
    LogInfo("Request from " + this->socket().peerAddress().toString());
	m_heartbeat.update();
	while(1)
	{
    	try
    	{
			//num = socket().receiveBytes(cRecvBuf,sizeof(cRecvBuf));
			num = socket().receiveBytes(cRecvBuf,0,60*1000);
			if(num > 0)
			{
				LogInfo("recv msg is " + std::string(cRecvBuf.begin()));
	#if 0
				JSON::Parser parse;
				JSON::Object::Ptr objptr = parse.parse(std::string(cRecvBuf.begin())).extract<JSON::Object::Ptr>();
				
				if(objptr->has("resource"))
				{
					std::string res = objptr->get("resource");
		//			std::string appVer = jsonptr->get("appVersion");
					if(!res.empty())	
					{
						LogInfo("get resource is " + res);
		//			LogInfo("get appVersion is " + appVer);
						if(res == "heartbeat")
						{
							m_heartbeat.update();
						}
					}
				}
					
				cRecvBuf.clear();				

				JSON::Object jsonObj;
        		jsonObj.set("msg", "heartbeat");
        		jsonObj.set("code",0);      //普通命令的id从0到100000，心跳的命令id为100001
				JSON::Object dataObj;
				jsonObj.set("deviceCode","310000000000023D");
				jsonObj.set("commandId",1000001);
				//dataObj.set("serverTime",1000*time(NULL));
				dataObj.set("serverTime",time(NULL));
				jsonObj.set("data",dataObj);

				std::ostringstream oss;
				jsonObj.stringify(oss);
        		std::string strSend = oss.str() + "\r\n";
					
       	 		socket().sendBytes(strSend.data(), (int) strSend.length());
				LogInfo("send device msg is " + strSend);
#endif

#if 1
				if(GetMysql())
				{
					if(!bConnectState)
					{
    					app.logger().information("mysql connect failed\n");
						break;
					}
				}

    			app.logger().information("receive info is \t" + std::string(cRecvBuf.begin()));
				JSON::Parser parse;
				JSON::Object::Ptr objptr = parse.parse(std::string(cRecvBuf.begin())).extract<JSON::Object::Ptr>();
				JSON::Object::Ptr jsonptr = objptr->getObject("data");
				char buf[512];
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

        			std::string dt("{\"data\":{\"status\":\"ok\"}}");
        			dt.append("\r\n");
       	 			socket().sendBytes(dt.data(), (int) dt.length());
				}
				else
				{
    				app.logger().information("get json data is null");
				}
#endif
				cRecvBuf.clear();				
			}			
#if 0			
			if((long long)m_heartbeat.elapsed() > HEARTBEAT_TIMEOUT)
			{
				LogDebug("tcp connect timeout equipment lost elapsed is "  + std::to_string(m_heartbeat.elapsed()));
				m_heartbeat.update();
				break;
			}
#endif
    	}
    	catch (Poco::Exception& ex)
    	{
            LogError("exception happens, err msg:" + ex.displayText());
			break;
    	}
	}
}
