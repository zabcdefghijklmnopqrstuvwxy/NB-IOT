
#pragma once
#include <string>
#include "Poco/Message.h"
#include "Poco/Mutex.h"
#include "Poco/Logger.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/SplitterChannel.h"
#include "Poco/PatternFormatter.h"

using namespace Poco;
using namespace Poco::Util;
namespace COMMON
{
    class CEasyLog
    {
        public: 
            ~CEasyLog();
            
            static CEasyLog* GetInstance();
            static void CloseInstance();

            void Log_Trace(const std::string &msg,Poco::Message::Priority pri,const char *file = NULL,int line = 1);
            void Log_Debug(const std::string &msg,Poco::Message::Priority pri,const char *file = NULL,int line = 1);
            void Log_Info(const std::string &msg,Poco::Message::Priority pri,const char *file = NULL,int line = 1);
            void Log_Warn(const std::string &msg,Poco::Message::Priority pri,const char *file = NULL,int line = 1);
            void Log_Error(const std::string &msg,Poco::Message::Priority pri,const char *file = NULL,int line = 1);
            void Log_Fatal(const std::string &msg,Poco::Message::Priority pri,const char *file = NULL,int line = 1);

            std::string GetLevelString();
            int GetLogLevel();
            int SetLogLevel(int level);
            int SetLogLevel(const std::string &level);

            int Init(const Poco::Util::AbstractConfiguration& config);
        private:
            CEasyLog();
			Poco::Logger&		   m_logger;
            static CEasyLog* 	   m_logIns;
            static Poco::FastMutex m_mutex;
    };	
}

#define LogTrace(message)     \
	COMMON::CEasyLog::GetInstance()->Log_Trace(message, Message::PRIO_TRACE,__FILE__,__LINE__);
#define LogDebug(message)     \
	COMMON::CEasyLog::GetInstance()->Log_Debug(message, Message::PRIO_DEBUG,__FILE__,__LINE__)
#define LogInfo(message)      \
	COMMON::CEasyLog::GetInstance()->Log_Info(message, Message::PRIO_INFORMATION,__FILE__,__LINE__)
#define LogWarn(message)      \
	COMMON::CEasyLog::GetInstance()->Log_Warn(message, Message::PRIO_WARNING,__FILE__,__LINE__)
#define LogError(message)     \
	COMMON::CEasyLog::GetInstance()->Log_Error(message, Message::PRIO_ERROR,__FILE__,__LINE__)
#define LogFatal(message)     \
	COMMON::CEasyLog::GetInstance()->Log_Fatal(message, Message::PRIO_FATAL,__FILE__,__LINE__)

