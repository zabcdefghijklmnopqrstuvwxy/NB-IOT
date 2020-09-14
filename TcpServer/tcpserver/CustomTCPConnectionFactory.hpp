#ifndef CustomTCPConnectionFactory_hpp
#define CustomTCPConnectionFactory_hpp

#include "CustomTCPConnection.hpp"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/StreamSocket.h"
#include <string>

using Poco::Net::TCPServerConnection;
using Poco::Net::StreamSocket;

class CustomTCPConnectionFactory: public Poco::Net::TCPServerConnectionFactory
{
public:
    TCPServerConnection* createConnection(const StreamSocket& socket);
};

#endif /* CustomTCPConnectionFactory_hpp */
