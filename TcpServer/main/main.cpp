#include "CustomTCPServer.hpp"
#include "pocomysql.h"
#include "CEasyLog.h"
#include <iostream>

int main(int argc, char **argv)
{
    return CustomTCPServer().run(argc, argv);
}
