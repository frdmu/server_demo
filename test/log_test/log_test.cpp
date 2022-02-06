#include "log.h"
#include <iostream>
#include <ctime>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv) {
    time_t start = clock();
    frdmu::Logger::ptr logger = frdmu::Logger::ptr(new frdmu::Logger("test log", frdmu::LogLevel::Level::DEBUG));  
    frdmu::StdoutLogAppender::ptr stdoutLogAppender = frdmu::StdoutLogAppender::ptr(new frdmu::StdoutLogAppender(frdmu::LogLevel::Level::DEBUG)); 
    logger->addAppender(stdoutLogAppender); 
   
    frdmu::LogEvent::ptr logEvent = frdmu::LogEvent::ptr(new frdmu::LogEvent(__FILE__, __LINE__, 0, 0, clock()-start, time(nullptr), "hello frdmu!"));
    logger->debug(logEvent);

    sleep(5);
    
    frdmu::LogEvent::ptr logEvent2 = frdmu::LogEvent::ptr(new frdmu::LogEvent(__FILE__, __LINE__, 0, 0, clock()-start, time(nullptr), "hello frdmu!"));
    logger->error(logEvent2);
    
    return 0;
}
