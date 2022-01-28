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
        static std::string toString(LogLevel::Level level);
};

class LogEvent {
    public: 
        typedef std::shared_ptr<LogEvent> ptr; 
        LogEvent() = default; 
        std::string getFilename() const {return m_filename;}
        uint32_t getLine() const {return m_line;}
        uint32_t getThreadid() const {return m_threadId;}
        uint32_t getFiberid() const {return m_fiberId;}
        uint32_t getElapse() const {return m_elapse;}
        uint64_t getDate() const {return m_timeStamp;} // to do
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
        //Logger(const std::string &name = "root");

        void log(LogLevel::Level level, LogEvent::ptr event); // 如果level >= m_level，就输出
        void debug(LogEvent::ptr event);
        void info(LogEvent::ptr event);
        void warn(LogEvent::ptr event);
        void error(LogEvent::ptr event);
        void fatal(LogEvent::ptr event);
        //void addAppender(LogAppender::ptr appender);
        //void delAppender(LogAppender::ptr appender);
        //LogLevel::level getLevel() const {return m_level;}
        //void setLevel(LogLevel::level val) {m_level = val;}
    private:
        std::string m_name;
        LogLevel::Level m_level;
        std::list<LogAppender::ptr> m_appenders;
};
class LogFormatterItem {
    public:
        typedef std::shared_ptr<LogFormatterItem> ptr;
        LogFormatterItem() = default;
        virtual void format(ostream &os, LogEvent::ptr event) = 0;
        virtual void format(ostream &os, Logger::ptr logger) = 0; // to do
        virtual void format(ostream &os, LogLevel::Level level) = 0;
    private:
}

/*
格式名 含义
1.%filename 输出日志信息所属的文件名 done
2.%l 输出日志事件的发生位置，即输出日志信息的语句处于它所在的文件的第几行
3.%t 输出产生该日志事件的线程名
4.%fiberid 协程名
5.%r 输出自应用启动到输出该日志信息所耗费的毫秒数
6.%d 输出日志时间点的日期或时间，默认格式为ISO8601，也可以在其后指定格式，比如：%d{yyy-MM-dd HH:mm:ss }，输出类似：2002-10-18- 22：10：28
7.%m 输出代码中指定的信息，如log(message)中的message
8.%n 输出一个回车换行符，Windows平台为“rn”，Unix平台为“n”
9.%p 输出优先级，即DEBUG，INFO，WARN，ERROR，FATAL。如果是调用debug()输出的，则为DEBUG，依此类推

        std::string m_filename;  
        uint32_t m_line;
        uint32_t m_threadId;
        uint32_t m_fiberId;       // 协程号
        uint32_t m_elapse;        // 程序启动到现在的毫秒数
        uint64_t m_timeStamp;
        std::string m_content;

*/

class FilenameLogFormatterItem: public LogFormatterItem {
    public:
        typedef std::shared_ptr<FilenameLogFormatterItem> ptr;
        FilenameLogFormatterItem() = default;
        void format(ostream &os, LogEvent::ptr event) const {os << event->getFilaname();}
};
class LineLogFormatterItem: public LogFormatterItem {
    public:
        typedef std::shared_ptr<LineLogFormatterItem> ptr;
        LineLogFormatterItem() = default;
        void format(ostream &os, LogEvent::ptr event) const {os << event->getLine();}
};
class ThreadidLogFormatterItem: public LogFormatterItem {
    public:
        typedef std::shared_ptr<ThreadidFormatterItem> ptr;
        ThreadidLogFormatterItem() = default;
        void format(ostream &os, LogEvent::ptr event) const {os << event->getThreadid();}
};
class FiberidLogFormatterItem: public LogFormatterItem {
    public:
        typedef std::shared_ptr<FiberidFormatterItem> ptr;
        FiberidLogFormatterItem() = default;
        void format(ostream &os, LogEvent::ptr event) const {os << event->getFiberid();}
};
class ElapseLogFormatterItem: public LogFormatterItem {
    public:
        typedef std::shared_ptr<ElapseFormatterItem> ptr;
        ElapseLogFormatterItem() = default;
        void format(ostream &os, LogEvent::ptr event) const {os << event->getElapse();}
};

class DateLogFormatterItem: public LogFormatterItem {
    public:
        typedef std::shared_ptr<DateFormatterItem> ptr;
        DateLogFormatterItem() = default;
        void format(ostream &os, Logevent::ptr event) const {os << event->getDate();}
};

class MessageLogFormatterItem: public LogFormatterItem {
    public:
        typedef std::shared_ptr<MessageFormatterItem> ptr;
        MessageLogFormatterItem() = default;
        void format(ostream &os, Logger::ptr logger) const {os << "logger->getMessage(), to do";} // to do
};

class NewlineLogFormatterItem: public LogFormatterItem {
    public:
        typedef std::shared_ptr<NewlineFormatterItem> ptr;
        NewlineLogFormatterItem() = default;
        void format(ostream &os, LogEvent::ptr event) const {os << std::endl;}
};

class LevelLogFormatterItem: public LogFormatterItem {
    public:
        typedef std::shared_ptr<LevelFormatterItem> ptr;
        LevelLogFormatterItem() = default;
        void format(ostream &os, LogLevel::Level level) const {os << LogLevel::toString(level);} // to do
};
// %m %d{y-m-d h-m-s} %t -> m_items
class LogFormatter {
    public:
        typedef std::shared_ptr<LogFormatter> ptr;
        LogFormatter() = default;
        std::string format(LogEvent::ptr event); // to do
        void init(); // set m_items 
    private:
        std::string m_pattern;
        LogFormatterItem::ptr m_items;
};

class LogAppender {
    public:
        typedef std::shared_ptr<LogAppender> ptr;
        LogAppender() = default;
        virtual ~LogAppender() {}

        virtual void log(LogLevel::Level level, LogEvent::ptr event) = 0; // 如果level >= m_level，就输出
        //void setFormatter(LogFormatter::ptr val) {m_formatter = val;}
        //LogFormatter::ptr getFormatter() const {return m_formatter;}
    protected:
        Loglevel::Level m_level;
        LogFormatter::ptr m_formatter;
};


class StdoutLogAppender: public LogAppender {
    public:
        typedef std::shared_ptr<StdoutLogAppender> ptr;
        void log(LogLevel::Level level, LogEvent::ptr event) override; // 如果level >= m_level，就输出
};

class FileLogAppender: public LogAppender {
    public:
        typedef std::shared_ptr<FileLogAppender> ptr;
        FileLogAppender() = default; 
        //FileLogAppender(const string &filename):m_filename(filename) {}
        void log(LogLevel::Level level, LogEvent::ptr event) override; // 如果level >= m_level，就输出
        //void reopen();
    private:
        std::string m_filename;
        std::fstream m_fstrm;
};

#endif
