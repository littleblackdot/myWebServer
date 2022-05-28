#pragma once

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include "LogStream.h"

class Logger {
private:
    class Impl{
        public:
            Impl(const char *fileName, int line);
            void formatTime();
            LogStream stream_;
            int line_;
            std::string baseFileName_;
    };

    Impl impl_;
    static std::string logFileName_;
public:
    Logger(const char *fileName, int len);
    ~Logger();
    LogStream &stream(){ return impl_.stream_;}
    static void setLogFileName(std::string logFileName){ logFileName_ = logFileName; }
    static std::string getLogFileName() { return logFileName_; }
};

#define LOG Logger(__FILE__, __LINE__).stream();