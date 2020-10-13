
#include "CEasyLog.h"
#include "Poco/SplitterChannel.h"
#include "Poco/SimpleFileChannel.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/AutoPtr.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include "Poco/File.h"
#include "Poco/Error.h"
#include "Poco/Util/PropertyFileConfiguration.h"
#include "Poco/File.h"
#include <iostream>
#include <time.h>


#define CONFIG_PATH "../config/config.properties"

using namespace std;
namespace COMMON
{
    static std::string priorities[] = 
	{
		"",
		"fatal",
		"critical",
		"error",
		"warning",
		"notice",
		"information",
		"debug",
		"trace"
	};


    CEasyLog* CEasyLog::m_logIns = NULL;
    Poco::FastMutex CEasyLog::m_mutex;
    CEasyLog::CEasyLog():m_logger(Poco::Logger::root())
    {
    }

    CEasyLog::~CEasyLog()
    {
    }

    CEasyLog* CEasyLog::GetInstance()
    {
        if(m_logIns == NULL)
        {
            Poco::FastMutex::ScopedLock lock(m_mutex);
            if(m_logIns == NULL)
            {
                m_logIns = new CEasyLog();
            }
            Poco::File f(CONFIG_PATH);
            if(f.exists()) 
            {
                Poco::Util::PropertyFileConfiguration* pPff = new Poco::Util::PropertyFileConfiguration(CONFIG_PATH);
                m_logIns->Init(*pPff);
            }
            else
            {
                std::cout << "config logger failed, config file " << CONFIG_PATH << " not exist!" << std::endl;
            }       
        }
        return m_logIns;
    }

    void CEasyLog::CloseInstance()
    {
        if(m_logIns != NULL)
        {
            delete m_logIns;
            m_logIns = NULL;
        }
    }

    void CEasyLog::Log_Trace(const std::string &msg,Poco::Message::Priority pri,const char *file,int line)
    {
        m_logger.trace(msg,file,line);
    }

    void CEasyLog::Log_Debug(const std::string &msg,Poco::Message::Priority pri,const char *file,int line)
    {
        m_logger.debug(msg,file,line);
    }   

    void CEasyLog::Log_Info(const std::string &msg,Poco::Message::Priority pri,const char *file,int line)
    {   
        m_logger.information(msg,file,line);
    }

    void CEasyLog::Log_Warn(const std::string &msg,Poco::Message::Priority pri,const char *file,int line)
    {
        m_logger.warning(msg,file,line);
    }

    void CEasyLog::Log_Error(const std::string &msg,Poco::Message::Priority pri,const char *file,int line)
    {
        m_logger.error(msg,file,line);
    }

    void CEasyLog::Log_Fatal(const std::string &msg,Poco::Message::Priority pri,const char *file,int line)
    {
        m_logger.fatal(msg,file,line);
    }

    std::string CEasyLog::GetLevelString()
    {
        int level = GetLogLevel();
        if(level >=0 && level <= 8)
        {
            return priorities[level];
        }
        return "";
    }

    int CEasyLog::GetLogLevel()
    {
        return m_logger.getLevel();
    }

    int CEasyLog::SetLogLevel(int level)
    {
        if(level >= 0 && level <= 8)
        {
            m_logger.setLevel(level);
            return 0;
        }
        return -1;
    }

    int CEasyLog::SetLogLevel(const std::string &level)
    {
        for(int i = 0; i <= 8; i++)
        {
            if(level == priorities[i])
            {
                m_logger.setLevel(level);
                return 0;
            }
        }
        return -1;
    }

    int CEasyLog::Init(const Poco::Util::AbstractConfiguration& config)
    {
        AutoPtr<SplitterChannel> pSplitter(new SplitterChannel());
        //system("rm ./logs/app.log.0");    

        try
        {
            std::string logPattern  = config.getString("logger.pattern","%t");
            std::string logTime     = config.getString("logger.times","local");
            AutoPtr<PatternFormatter> pPF(new PatternFormatter());
            pPF->setProperty("pattern",logPattern);
            pPF->setProperty("times",logTime);

            bool consoleEnable = config.getBool("logger.console.enable",true);
            if(consoleEnable)
            { 
                AutoPtr<ConsoleChannel> pConsoleChannel(new ConsoleChannel());
                AutoPtr<FormattingChannel> pPFConsole(new FormattingChannel(pPF,pConsoleChannel));
                pSplitter->addChannel(pPFConsole);
            }

            bool fileEnable = config.getBool("logger.file.enable",true);
            if(fileEnable)
            {
                std::string logFileName = config.getString("logger.file","app.log");
                std::string logFileDir  = config.getString("logger.file.dir","../logs/");
                std::string logFilePath = logFileDir + logFileName;
                std::string logSize     = config.getString("logger.file.size","16M");

                Poco::File file(logFilePath);
                file.createFile();
                file.setWriteable();
                AutoPtr<SimpleFileChannel> pFileChannel(new SimpleFileChannel(logFilePath));
                pFileChannel->setProperty("rotation",logSize);
                pFileChannel->setProperty("path",logFilePath);
                pFileChannel->setProperty("secondaryPath",logFilePath);

                AutoPtr<FormattingChannel> pFCFile(new FormattingChannel(pPF,pFileChannel));
                pSplitter->addChannel(pFCFile);
            }
            
			m_logger.setChannel(pSplitter);
            const std::string level = config.getString("logger.level","information");
            m_logger.setLevel(level);
        }
        catch(Poco::Exception& ex)
        {
            std::cout << "init log module failed,exception:" << ex.displayText() << std::endl;
        }
        return 0;
    }

}

