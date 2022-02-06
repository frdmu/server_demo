#include "log.h"
#include <iostream>
int main(int argc, char **argv) {
    frdmu::Logger::ptr logger = frdmu::Logger::ptr(new frdmu::Logger("test log", frdmu::LogLevel::Level::DEBUG));  
    frdmu::StdoutLogAppender::ptr stdoutLogAppender = frdmu::StdoutLogAppender::ptr(new frdmu::StdoutLogAppender(frdmu::LogLevel::Level::DEBUG)); 
    logger->addAppender(stdoutLogAppender); 
    frdmu::LogEvent::ptr logEvent = frdmu::LogEvent::ptr(new frdmu::LogEvent(__FILE__, __LINE__, 0, 0, 1, 2, "hello frdmu!"));
     
    logger->debug(logEvent);
    
    return 0;
}
