# 学习编写服务器框架
## 日志模块
### Logger
成员有： 日志名，日志等级，日志输出地的列表
做的事： 处理指定等级的日志事件
ie: debug(event) -> log(LogLevel::Level::DEBUG, event) -> LogAppender.log(LogLevel::Level::DEBUG, event)
### LogAppender
是一个虚类，子类有StdoutLogAppender, FileLogAppender
log(level, event)会在子类中被重载,
在StdoutLogAppender中，格式化后的事件会被写入cout;
在FileLogAppender中，格式化后的事件会被写入fstream;
成员有：日志等级，日志格式器
做的事：在不同子类中具体实现Logger中的log函数
ie: LogAppender.log(level, event) -> LogFormatter.format( event)
### LogFormatter


#### 1.为什么只处理等级比自己大的情况？


debug(event) -> log(level, event) -> appender.log(level, event) -> logformatter.format(event) -> logformatteritem.format(os, event) 
