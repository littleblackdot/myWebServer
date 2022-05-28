#include "Log.h"
#include "CurrentThread.h"
#include "Thread.h"
#include "AsynLog.h"
#include <assert.h>
#include <iostream>
#include <time.h>  
#include <sys/time.h> 

static pthread_once_t onceControl = PTHREAD_ONCE_INIT;

static AsynLog *asynLogger;

std::string Logger::logFileName_ =  "./WebServer.log";

void onceInit(){
    asynLogger = new AsynLog(Logger::getLogFileName());
    asynLogger->start();
}

void output(const char * msg, int len){ 
    pthread_once(&onceControl, onceInit);
    asynLogger->append(msg, len);
}

Logger::Impl::Impl(const char *fileName, int line)
    : stream_(),
    line_(line),
    baseFileName_(fileName){
        formatTime();
}

void Logger::Impl::formatTime()
{
    struct timeval tv;
    time_t time;
    char str_t[26] = {0};
    gettimeofday (&tv, NULL);
    time = tv.tv_sec;
    struct tm* p_time = localtime(&time);   
    strftime(str_t, 26, "%Y-%m-%d %H:%M:%S\n", p_time);
    stream_ << str_t;
}

Logger::Logger(const char *fileName, int line)
  : impl_(fileName, line)
{ }

Logger::~Logger()
{
    impl_.stream_ << " -- " << impl_.baseFileName_ << ':' << impl_.line_ << '\n';
    const LogStream::Buffer& buf(stream().buffer());
    output(buf.data(), buf.length());
}