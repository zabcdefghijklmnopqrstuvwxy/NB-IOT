#ifndef CustomTCPConnection_hpp
#define CustomTCPConnection_hpp

#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/StreamSocket.h"
#include "pocomysql.h"
#include <string>

using Poco::Net::StreamSocket;
using Poco::Net::TCPServerConnection;

class CustomTCPConnection: public TCPServerConnection
{
public:
    CustomTCPConnection(const StreamSocket& s);
    void run();
	static PocoMySQL *GetMysql();
	
private:
	static PocoMySQL *m_instance;
	static bool bConnectState;
};

#endif /* CustomTCPConnection_hpp */
