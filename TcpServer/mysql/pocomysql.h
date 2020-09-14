#ifndef POCOMYSQL_H
#define POCOMYSQL_H

#include <Poco/Exception.h>
#include <Poco/Format.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/Row.h>
#include <Poco/Tuple.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/StatementImpl.h>
#include <Poco/Data/SessionPool.h>
#include "Poco/Data/Session.h"
#include <Poco/Data/MySQL/Connector.h>
//#include <Poco/Data/MySQL/MySQLException.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
//#include <vector>
#include <list>
using std::string;
using std::wstring;

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Data::RecordSet;
using Poco::format;
using Poco::DateTime;
using Poco::NumberParser;
using Poco::Any;
using Poco::AnyCast;
using Poco::Int32;
using Poco::Nullable;
using Poco::Tuple;
using Poco::DynamicAny;
class PocoMySQL {
public:
    PocoMySQL();
    PocoMySQL(string host,int port,string user,string password,string db);
    PocoMySQL(string connectString);
    bool connect();
    
    PocoMySQL(const PocoMySQL& orig);
    virtual ~PocoMySQL();
    
    int execute(string sql);
    
    template<typename T>
    bool query(string sql,T &results){
        try
        {
            *_pSession << sql, into(results), now;
            return true;
        }
        catch(Poco::Exception& e){
            return false;
        }
    }
    
private:
    //Poco::SharedPtr<Poco::Data::Session> _pSession = 0;
    Session* _pSession;
    string _host;
    int _port;
    string _user;
    string _password;
    string _db;
    string _connectionstring;
    bool _connected;
};

#endif /* POCOMYSQL_H */
