#pragma once
#include <functional>
#include <string>
#include <vector>
#include "../CountDownLatch.h"
#include "FixedBuffer.h"
#include "../MutexLock.h"
#include "../Thread.h"
#include "../noncopyable.h"

class AsynLog : noncopyable{
private:
    void threadFunc();
    typedef FixedBuffer<kLargeBuffer> Buffer;
    typedef std::vector<std::shared_ptr<Buffer>> BufferVector;
    typedef std::shared_ptr<Buffer> BufferPtr;
    const int flushInterval_;
    bool running_;
    std::string logFileName_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;
    BufferPtr curBuffer_;
    BufferPtr nextBuffer_;
    BufferVector buffers_;
    CountDownLatch latch_;
public:
    AsynLog(const std::string& logFileName, int flushInterval = 2);
    ~AsynLog();
    void append(const char *logMess, size_t len);
    void start();
    void stop();
};
