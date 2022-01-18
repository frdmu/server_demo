#ifndef _log_h
#define _log_h

#include <string>
#include <memory>
#include <list>

class LogLevel {
    public:
        enum Level {
            DEBUG = 1,
            INFO = 2,
            WARN = 3,
            ERROR = 4,
            FATAL = 5
        };
};

class LogEvent {
    public: 
        typedef std::shared_ptr<LogEvent> ptr; 
        LogEvent() = default; 
    private:
        std::string m_filename;  
        uint32_t m_line;
        uint32_t m_threadId;
        uint32_t m_fiberId;       // 协程号
        uint32_t m_elapse;        // 程序启动到现在的毫秒数
        uint64_t m_timeStamp;
        std::string m_content;
};

class Logger {
    public:
        typedef std::shared_ptr<Logger> ptr;
        Logger() = default;
        Logger(const std::string &name = "root");

        void log(LogLevel::Level level, LogEvent::ptr event); // 如果level >= m_level，就输出
        void debug(LogEvent::ptr event);
        void info(LogEvent::ptr event);
        void warn(LogEvent::ptr event);
        void error(LogEvent::ptr event);
        void fatal(LogEvent::ptr event);
        void addAppender(LogAppender::ptr appender);
        void delAppender(LogAppender::ptr appender);
        LogLevel::level getLevel() const {return m_level;}
        void setLevel(LogLevel::level val) {m_level = val;}
    private:
        std::string m_name;
        LogLevel::Level m_level;
        std::list<LogAppender::ptr> m_appenders;
};

class LogAppender {
    public:
        typedef std::shared_ptr<LogAppender> ptr;
        LogAppender() = default;
        virtual ~LogAppender() {}
    private:
        Loglevel::Level m_level;
};

class LogFormatter {
    public:
        typedef std::shared_ptr<LogFormatter> ptr;
        LogFormatter() = default;
        std::string formatter(LogEvent::ptr event); // to do
    private:
};

class StdoutLogAppender(): public LogAppender {
    
};

class FileLogAppender(): public LogAppender {

};

#endif
