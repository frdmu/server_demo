#include "log.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>

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

std::string LogFormatter::format(LogEvent::ptr event) {
    std::stringstream ss; 
    for (auto item : m_items) {
       item->format(ss, event);
    }

    return ss.str();
}
// set m_items
// %xxx %xxx{xxx} %%
// (str fmt flag)
// m_pattern: hello log: %d{yyy-MM-dd HH:mm:ss} %p %filename %l %t %fiberid %r %m end log!%n
void LogFormatter::init() {
    // parse done
     size_t n = m_pattern.size();
    std::vector<std::tuple<std::string, std::string, int>> vec;
    std::string nstr;
    for (auto i = 0; i < n; ++i) {
        if (m_pattern[i] != '%') {
            nstr += m_pattern[i];
            continue;
        }           
        // 普通字符串或空格 
        if (!nstr.empty()) {
            vec.push_back(std::make_tuple(nstr, "", 0));
            nstr.clear();
        }
        // %%
        if (i+1 < n && m_pattern[i+1] == '%') {
            vec.push_back(std::make_tuple("%", "", 0));
            continue;
        }

        auto j = i + 1;
        std::string str;
        std::string fmt;
        int fmt_begin;
        int fmt_status = 0;
        while (j < n) {
            if (fmt_status == 0) {
                if (m_pattern[j] == '{') {
                    str = m_pattern.substr(i+1, j-i-1);
                    fmt_begin = j;
                    fmt_status = 1;
                } else if (!isalpha(m_pattern[j])) {
                    str = m_pattern.substr(i+1, j-i-1);
                    i = j - 1;
                    break;
                }
                ++j;
            }
            if (fmt_status == 1) {
                if (m_pattern[j] == '}') {
                    fmt = m_pattern.substr(fmt_begin+1, j-fmt_begin-1);
                    i = j; 
                    break;
                }
                ++j;
            }
        } 
        if (fmt_status == 0) {
            if (str.empty() && j == n) {
                str = m_pattern.substr(i+1, n-i-1);
            }
            vec.push_back(std::make_tuple(str, "", 0));
        } else if (fmt_status == 1) {
            vec.push_back(std::make_tuple(str, fmt, 1));
        }
    }

    for (auto t : vec) {
        std::cout << std::get<0>(t) << " " << std::get<1>(t) << " " << std::get<2>(t) << std::endl;
    }
}



static std::string LogLevel::toString(LogLevel::Level level) {
    switch (level) {
#define xx(level) \
        case level: \
            return #level;

        xx(DEBUG);
        xx(INFO);
        xx(WARN);
        xx(ERROR);
        xx(FATAL);
    }
}
