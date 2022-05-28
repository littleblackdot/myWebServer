#include "LogFile.h"
#include "FileUtil.h"
#include<assert.h>
#include<time.h>
#include<cstdio>

LogFile::LogFile(const std::string& filename, int flushEveryN)
    : fileName_(filename),
     flushEveryN_(flushEveryN),
     count_(0),
     pMutexLock_(new MutexLock){
         file_.reset(new BaseFile(fileName_));
}


void LogFile::append_unlocked(const char * logMess, size_t len){
    file_->append(logMess, len);
    ++count_;
    if(count_ >= flushEveryN_){
        count_ = 0;
        file_->flush();
    }
}

void LogFile::append(const char * logMess, size_t len){
    MutexLockGuard guard(*pMutexLock_);
    append_unlocked(logMess, len);
}

void LogFile::flush(){
    MutexLockGuard guard(*pMutexLock_);
    file_->flush();
}