#ifndef _log_h
#define _log_h

#include <string>
#include <memory>
#include <list>
#include <fstream>
#include <vector>
#include <iostream>
#include <time.h>
namespace frdmu {
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
            //LogEvent() = default; 
            LogEvent(std::string filename, uint32_t line, uint32_t threadId, uint32_t fiberId, uint32_t elapse, uint64_t timeStamp, std::string content): m_filename(filename), m_line(line), m_threadId(threadId), m_fiberId(fiberId), m_elapse(elapse), m_timeStamp(timeStamp), m_content(content) {}
            std::string getFilename() const {return m_filename;}
            uint32_t getLine() const {return m_line;}
            uint32_t getThreadid() const {return m_threadId;}
            uint32_t getFiberid() const {return m_fiberId;}
            uint32_t getElapse() const {return m_elapse;}
            std::string getDate(std::string fmt); // to do
            std::string getContent() const {return m_content;}
        private:
            std::string m_filename;  
            uint32_t m_line;
            uint32_t m_threadId;
            uint32_t m_fiberId;       // 协程号
            uint32_t m_elapse;        // 程序启动到现在的毫秒数
            time_t m_timeStamp;
            std::string m_content;
    };

    class Logger;
    class LogFormatterItem {
        public:
            typedef std::shared_ptr<LogFormatterItem> ptr;
            LogFormatterItem() = default;
            LogFormatterItem(std::string fmt): m_fmt(fmt) {}
            virtual void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
        protected:
            std::string m_fmt;
    };

    // %m %d{y-m-d h-m-s} %t -> m_items
    class LogFormatter {
        public:
            typedef std::shared_ptr<LogFormatter> ptr;
            //LogFormatter() = default;
            LogFormatter(std::string pattern): m_pattern(pattern) {init();}
            std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event); 
            void init(); // set m_items 
        private:
            std::string m_pattern;
            std::vector<LogFormatterItem::ptr> m_items;
    };

    class LogAppender {
        public:
            typedef std::shared_ptr<LogAppender> ptr;
            //LogAppender() = default;
            LogAppender(LogLevel::Level level): m_level(level) {}
            //virtual ~LogAppender() {}
            virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0; // 如果level >= m_level，就输出
            //void setFormatter(LogFormatter::ptr val) {m_formatter = val;}
            //LogFormatter::ptr getFormatter() const {return m_formatter;}
        protected:
            LogLevel::Level m_level;
            LogFormatter::ptr m_formatter = LogFormatter::ptr(new LogFormatter("[%p] %r(ms) %d{YYYY-MM-DD HH:mm:ss} -> %filename %l(row) %t %fiberid message:%m %n"));
    };

    class Logger : public std::enable_shared_from_this<Logger> {
        public:
            typedef std::shared_ptr<Logger> ptr;
            //Logger() = default;
            Logger(const std::string &name, const LogLevel::Level level): m_name(name), m_level(level) {}
            void log(LogLevel::Level level, LogEvent::ptr event); // 如果level >= m_level，就输出
            void debug(LogEvent::ptr event);
            void info(LogEvent::ptr event);
            void warn(LogEvent::ptr event);
            void error(LogEvent::ptr event);
            void fatal(LogEvent::ptr event);
            void addAppender(LogAppender::ptr appender);
            //void delAppender(LogAppender::ptr appender);
            //LogLevel::level getLevel() const {return m_level;}
            //void setLevel(LogLevel::level val) {m_level = val;}
            //std::string getName() const {return m_name;}
        private:
            std::string m_name;
            LogLevel::Level m_level;
            std::list<LogAppender::ptr> m_appenders;
    };

    /*
     * ie: [%p] %r %d{yyy-MM-dd HH:mm:ss} : %filename %l %t %fiberid %m %n 
    格式名 含义
    - 1.%filename 输出日志信息所属的文件名 done
    - 2.%l 输出日志事件的发生位置，即输出日志信息的语句处于它所在的文件的第几行
    - 3.%t 输出产生该日志事件的线程名
    - 4.%fiberid 协程名
    - 5.%r 输出自应用启动到输出该日志信息所耗费的毫秒数
    - 6.%d 输出日志时间点的日期或时间，默认格式为ISO8601，也可以在其后指定格式，比如：%d{yyy-MM-dd HH:mm:ss }，输出类似：2002-10-18- 22：10：28
    - 7.%m 输出代码中指定的信息，如log(message)中的message
    - 8.%n 输出一个回车换行符，Windows平台为“rn”，Unix平台为“n”
    - 9.%p 输出优先级，即DEBUG，INFO，WARN，ERROR，FATAL。如果是调用debug()输出的，则为DEBUG，依此类推

            std::string m_filename;  
            uint32_t m_line;
            uint32_t m_threadId;
            uint32_t m_fiberId;       // 协程号
            uint32_t m_elapse;        // 程序启动到现在的毫秒数
            uint64_t m_timeStamp;
            std::string m_content;

    */
    class NormalstringLogFormatterItem: public LogFormatterItem {
        public:
            typedef std::shared_ptr<NormalstringLogFormatterItem> ptr;
            // NormalstringLogFormatterItem() = default;
            NormalstringLogFormatterItem(std::string str): m_str(str) {}
            void format(std::ostream &os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {os << m_str;}
        private:
            std::string m_str;
    };

    class FilenameLogFormatterItem: public LogFormatterItem {
        public:
            typedef std::shared_ptr<FilenameLogFormatterItem> ptr;
            //FilenameLogFormatterItem() = default;
            FilenameLogFormatterItem(std::string fmt): LogFormatterItem(fmt) {}
            void format(std::ostream &os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {os << event->getFilename();}
    };

    class LineLogFormatterItem: public LogFormatterItem {
        public:
            typedef std::shared_ptr<LineLogFormatterItem> ptr;
            //LineLogFormatterItem() = default;
            LineLogFormatterItem(std::string fmt): LogFormatterItem(fmt) {}
            void format(std::ostream &os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {os << event->getLine();}
    };

    class ThreadidLogFormatterItem: public LogFormatterItem {
        public:
            typedef std::shared_ptr<ThreadidLogFormatterItem> ptr;
            //ThreadidLogFormatterItem() = default;
            ThreadidLogFormatterItem(std::string fmt): LogFormatterItem(fmt) {}
            void format(std::ostream &os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {os << event->getThreadid();}
    };

    class FiberidLogFormatterItem: public LogFormatterItem {
        public:
            typedef std::shared_ptr<FiberidLogFormatterItem> ptr;
            //FiberidLogFormatterItem() = default;
            FiberidLogFormatterItem(std::string fmt): LogFormatterItem(fmt) {}
            void format(std::ostream &os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {os << event->getFiberid();}
    };

    class ElapseLogFormatterItem: public LogFormatterItem {
        public:
            typedef std::shared_ptr<ElapseLogFormatterItem> ptr;
            //ElapseLogFormatterItem() = default;
            ElapseLogFormatterItem(std::string fmt): LogFormatterItem(fmt) {}
            void format(std::ostream &os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {os << event->getElapse();}
    };

    // to do fmt
    class DateLogFormatterItem: public LogFormatterItem {
        public:
            typedef std::shared_ptr<DateLogFormatterItem> ptr;
            //DateLogFormatterItem() = default;
            DateLogFormatterItem(std::string fmt): LogFormatterItem(fmt) {}
            void format(std::ostream &os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {os << event->getDate(m_fmt);}
    };

    class MessageLogFormatterItem: public LogFormatterItem {
        public:
            typedef std::shared_ptr<MessageLogFormatterItem> ptr;
            //MessageLogFormatterItem() = default;
            MessageLogFormatterItem(std::string fmt): LogFormatterItem(fmt) {}
            void format(std::ostream &os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {os << event->getContent();} 
    };

    class NewlineLogFormatterItem: public LogFormatterItem {
        public:
            typedef std::shared_ptr<NewlineLogFormatterItem> ptr;
            //NewlineLogFormatterItem() = default;
            NewlineLogFormatterItem(std::string fmt): LogFormatterItem(fmt) {}
            void format(std::ostream &os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {os << std::endl;} 
    };

    class LevelLogFormatterItem: public LogFormatterItem {
        public:
            typedef std::shared_ptr<LevelLogFormatterItem> ptr;
            //LevelLogFormatterItem() = default;
            LevelLogFormatterItem(std::string fmt): LogFormatterItem(fmt) {}
            void format(std::ostream &os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {os << LogLevel::toString(level);} 
    };

    class StdoutLogAppender: public LogAppender {
        public:
            typedef std::shared_ptr<StdoutLogAppender> ptr;
            StdoutLogAppender(LogLevel::Level level): LogAppender(level) {}
            void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override; // 如果level >= m_level，就输出
    };
/*
    // to do
    class FileLogAppender: public LogAppender {
        public:
            typedef std::shared_ptr<FileLogAppender> ptr;
            //FileLogAppender() = default; 
            //FileLogAppender(const string &filename):m_filename(filename) {}
            void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override; // 如果level >= m_level，就输出
            //void reopen();
        private:
            std::string m_filename;
            std::fstream m_fstrm;
    };
*/
}

#endif
