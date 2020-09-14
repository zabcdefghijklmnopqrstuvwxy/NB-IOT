#include "Config.h"
#include "CustomTCPServer.hpp"
#include "CustomTCPConnectionFactory.hpp"
#include "Poco/Timestamp.h"

void CustomTCPServer::initialize(Application& self)
{
    ServerApplication::loadConfiguration();
    ServerApplication::initialize(self);
}

void CustomTCPServer::uninitialize()
{
    ServerApplication::uninitialize();
}

int CustomTCPServer::main(const std::vector<std::string>& args)
{
    unsigned short port = (unsigned short) config().getInt("CustomTCPServer.port", ServerPort);

    // 1. Bind a ServerSocket with an address
    Poco::Net::ServerSocket serverSocket(port);

    // 2. Pass the ServerSocket to a TCPServer
    Poco::Net::TCPServer server(new CustomTCPConnectionFactory(), serverSocket);

    // 3. Start the TCPServer
    server.start();

    // 4. Wait for termination
    waitForTerminationRequest();

    // 5. Stop the TCPServer
    server.stop();

    return Application::EXIT_OK;
}
