#ifndef CustomTCPServer_hpp
#define CustomTCPServer_hpp

#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Application.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/TCPServer.h"
#include "Poco/Timestamp.h"

using Poco::Util::ServerApplication;
using Poco::Util::Application;

class CustomTCPServer : public ServerApplication
{
public:
    CustomTCPServer() {}
    ~CustomTCPServer() {}
protected:
    void initialize(Application& self);
    void uninitialize();
    int main(const std::vector<std::string>& args);
};

#endif /* CustomTCPServer_hpp */
