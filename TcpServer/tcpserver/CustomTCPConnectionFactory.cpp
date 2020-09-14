#include "CustomTCPConnectionFactory.hpp"

TCPServerConnection* CustomTCPConnectionFactory::createConnection(const StreamSocket& socket)
{
    return new CustomTCPConnection(socket);
}
