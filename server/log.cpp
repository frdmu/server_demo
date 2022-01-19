#include "log.h"
#include <iostream>
#include <fstream>

Logger::Logger(const std::string &name):m_name(name) {}

// 如果level >= m_level，就输出
void Logger::log(LogLevel::Level level, LogEvent::ptr event) {
    if (level >= m_level) {
        for (auto &i: m_appenders) {
            i->log(level, event);
        }
    }
}

void Logger::debug(LogEvent::ptr event) {
    log(LogLevel::Level::DEBUG, event);
}
void Logger::info(LogEvent::ptr event) {
    log(LogLevel::Level::INFO, event);
}
void Logger::warn(LogEvent::ptr event) {
    log(LogLevel::Level::WARN, event);
}
void Logger::error(LogEvent::ptr event) {
    log(LogLevel::Level::ERROR, event);
}
void Logger::fatal(LogEvent::ptr event) {
    log(LogLevel::Level::FATAL, event);
}
void Logger::addAppender(LogAppender::ptr appender) {
    m_appenders.push_back(appender);    
}
void Logger::delAppender(LogAppender::ptr appender) {
    for (auto it = m_appenders.begin(); it != m_appenders.end(); ++it) {
        if (*it == appender) {
            m_appenders.erase(it);
            break;
        }
    }
}

void StdoutLogAppender::log(LogLevel::Level level, LogEvent::ptr event) {
    if (level >= m_level) {
        std::cout << m_formatter->format(event);
    }
}

bool FileLogAppender::reopen() {
    if (m_fstrm.is_open()) {
        m_fstrm.close();
    }
    m_fstrm.open(m_filename);
    return !!m_fstrm;
}
void FileLogAppender::log(LogLevel::Level level, LogEvent::ptr event) {
    if (level >= m_level) {
        if (reopen()) 
            m_fstrm << m_formatter->format(event);
    }
}
