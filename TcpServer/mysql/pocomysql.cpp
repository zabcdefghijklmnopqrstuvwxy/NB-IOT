#include "pocomysql.h"

PocoMySQL::PocoMySQL() {
    Poco::Data::MySQL::Connector::registerConnector();
}
PocoMySQL::PocoMySQL(string host,int port,string user,string password,string db){
    this->_host = host;
    this->_port = port;
    this->_user = user;
    this->_password = password;
    this->_db = db;
    
    //const char fmt[]="host=%s;port=%d;db=%s;user=%s;password=%s;compress=true;auto-reconnect=true";
    char* buff = new char[512];
    sprintf(buff,"host=%s;port=%d;db=%s;user=%s;password=%s;compress=true;auto-reconnect=true",this->_host.c_str(),this->_port,this->_db.c_str(),this->_user.c_str(),this->_password.c_str());
    this->_connectionstring = buff;
    delete buff;
    Poco::Data::MySQL::Connector::registerConnector();
}

PocoMySQL::PocoMySQL(string connectString){
    this->_connectionstring = connectString;
    Poco::Data::MySQL::Connector::registerConnector();
}

bool PocoMySQL::connect(){
    //Poco::Data::MySQL::Connector.registerConnector();
    
    try
    {
    //Session session(this->_connectionstring);
    _pSession = new Session(Poco::Data::MySQL::Connector::KEY, this->_connectionstring);
    std::cout << "connect to dabase " << this->_db << " success..." << std::endl;
    //this->_pSession = new Session(Poco::Data::MySQL::Connector.createSession(this->_connectionstring));
    //return this->_pSession->isConnected();
    }
    catch(Poco::Exception& e){
        std::cout << "connect to dabase " << this->_db << " fail..." << std::endl;
        return false;
    }
    return true;
}


//执行增，删，修改操作，返回影响记录的行数
int PocoMySQL::execute(string sql){
    try
    {
        Statement stt(*this->_pSession);
        stt << sql;
        size_t r = stt.execute();
        std::cout << "affected " << r << " rows" << std::endl;
        return r;
    }
    catch(Poco::Exception& e){
        std::cout << "execute " << e.displayText() << std::endl;
        return -1;
    }
    //this->_pSession << sql ,into(count),now;
}

PocoMySQL::PocoMySQL(const PocoMySQL& orig) {
}

PocoMySQL::~PocoMySQL() {
    Poco::Data::MySQL::Connector::unregisterConnector();
}
