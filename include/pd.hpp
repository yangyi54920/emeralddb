#ifndef PD_HPP__
#define PD_HPP__

#include "core.hpp"

#define PD_LOG_STRINGMAX 4096//日志的最大长度

//大于当前的日志级别时打印出来，__func__表示当前的函数，__FILE__当前的文件，__LINE__当前的语句行
#define PD_LOG(level,fmt,...)                                         \
   do {                                                               \
      if ( _curPDLevel >= level )                                     \
      {                                                               \
         pdLog ( level, __func__, __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
      }                                                               \
   } while ( 0 )                                                      \

//检查条件是否满足，rc是返回码
#define PD_CHECK(cond,retCode,gotoLabel,level,fmt,...)                \
   do {                                                               \
      if ( !(cond) )                                                  \
      {                                                               \
         rc = (retCode) ;                                             \
         PD_LOG  ( (level), fmt, ##__VA_ARGS__) ;                     \
         goto gotoLabel ;                                             \
      }                                                               \
   } while ( 0 )                                                      


//返回码的检测
#define PD_RC_CHECK(rc,level,fmt,...)                                 \
   do {                                                               \
      PD_CHECK ( (EDB_OK==(rc)), (rc), error, (level),                \
                 fmt, ##__VA_ARGS__) ;                                \
   } while ( 0 )                                                      

#define EDB_VALIDATE_GOTOERROR(cond,ret,str)                          \
   {if(!(cond)) { pdLog(PDERROR, __func__, __FILE__, __LINE__, str) ; \
    rc = ret; goto error ; }}


enum PDLEVEL//日志级别
{
   PDSEVERE = 0,//紧急
   PDERROR,
   PDEVENT,//事件
   PDWARNING,
   PDINFO,//一般信息
   PDDEBUG
} ;

extern PDLEVEL _curPDLevel ;//当前日志级别
const char *getPDLevelDesp ( PDLEVEL level ) ;//打印日志级别的名字

#define PD_DFT_DIAGLEVEL PDWARNING
//两个重载的log函数
void pdLog ( PDLEVEL level, const char *func, const char *file,
             unsigned int line, const char *format, ... ) ;
void pdLog ( PDLEVEL level, const char *func, const char *file,
             unsigned int line, std::string message ) ;
#endif
