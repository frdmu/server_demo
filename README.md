# 学习编写服务器框架
## 日志模块
### Work Flow
logger.debug(event) -> logger.log(level, event) -> logAppender.log(logger, level, event) -> logFormatter.format(logger, level, event) -> logFormatterItem.format(os, logger, level, event) 
