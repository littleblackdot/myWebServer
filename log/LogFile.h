#pragma once
#include <memory>
#include <string>
#include "FileUtil.h"
#include "../MutexLock.h"
#include "../noncopyable.h"

class LogFile : noncopyable{
private:
    void append_unlocked(const char* logMess, size_t len);
    const std::string fileName_;
    const int flushEveryN_;
    int count_;
    std::unique_ptr<MutexLock> pMutexLock_;
    std::unique_ptr<BaseFile> file_;
public:
    LogFile(const std::string& filename, int flushEveryN = 1024);
    ~LogFile(){}
    void flush();
    bool rollFile();
    void append(const char * logMess, size_t len);
};