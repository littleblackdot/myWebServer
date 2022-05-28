#include "AsynLog.h"
#include<assert.h>
#include"LogFile.h"


AsynLog::AsynLog(const std::string &logFileName, int flushInterval = 2)
    : flushInterval_(flushInterval), 
      running_(false),
      logFileName_(logFileName),
      thread_(std::bind(&AsynLog::threadFunc, this), "log"),
      mutex_(),
      cond_(mutex_),
      curBuffer_(new Buffer),
      nextBuffer_(new Buffer),
      buffers_(),
      latch_(1){
      assert(logFileName_.size() > 1);
      curBuffer_->bzero();
      nextBuffer_->bzero();
      buffers_.reserve(16);
}

void AsynLog::append(const char *logMess, size_t len){
    MutexLockGuard guard(mutex_);
    if(curBuffer_->avail() > len){
        curBuffer_->append(logMess, len);
    } else {
        buffers_.push_back(curBuffer_);
        curBuffer_.reset();
        if (nextBuffer_){
            curBuffer_ = std::move(nextBuffer_);
        }else{
            curBuffer_.reset(new Buffer);
        }
        curBuffer_->append(logMess, len);
        cond_.signal();
    }
}

void AsynLog::threadFunc(){
    assert(running_);
    latch_.countDown();
    LogFile log(logFileName_);
    BufferPtr buffer1(new Buffer);
    BufferPtr buffer2(new Buffer);
    buffer1->bzero();
    buffer2->bzero();
    BufferVector buffersToWrite;
    buffersToWrite.reserve(16);
    while(running_){
        assert(buffer1 && buffer1->length() == 0);
        assert(buffer2 && buffer2->length() == 0);
        assert(buffersToWrite.empty());
        
        {
            MutexLockGuard lockGuard(mutex_);
            if(buffers_.empty()){
                cond_.waitForSeconds(flushInterval_);
            }
            buffers_.push_back(curBuffer_);
            curBuffer_.reset();
            curBuffer_ = std::move(buffer1);
            buffersToWrite.swap(buffers_);
            if(!nextBuffer_){
                nextBuffer_ = std::move(buffer2);
            }
        }

        assert(!buffersToWrite.empty());
        
        //日志堆积过多直接丢弃
        if(buffersToWrite.size() > 25){
            buffersToWrite.erase(buffersToWrite.begin() + 2, buffersToWrite.end());
        }

        for(int i=0; i<buffersToWrite.size(); ++i){
            log.append(buffersToWrite[i]->data(), buffersToWrite[i]->length());
        }

        if(buffersToWrite.size() > 2){
            buffersToWrite.resize(2);
        }

        if(!buffer1){
            assert(!buffersToWrite.empty());
            buffer1 = buffersToWrite.back();
            buffersToWrite.pop_back();
            buffer1->reset();
        }

        if(!buffer2){
            assert(!buffersToWrite.empty());
            buffer2 = buffersToWrite.back();
            buffersToWrite.pop_back();
            buffer2->reset();
        }

        buffersToWrite.clear();
        log.flush();
    }

    log.flush();

}


AsynLog::~AsynLog() {
    if(running_){
        stop();
    }
}


void AsynLog::start(){
    running_ = true;
    thread_.start();
    latch_.wait();
}

void AsynLog::stop(){
    running_ = false;
    cond_.signal();
    thread_.join();
}

