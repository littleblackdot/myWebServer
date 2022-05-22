#pragma once

#include "EventLoop.h"
#include "Thread.h"
#include "MutexLock.h"
#include "Condition.h"
#include "noncopyable.h"

class EventLoopThread : noncopyable{
private:
    void ThreadFunc();
    EventLoop *loop_;
    bool isExit_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;
public:
    EventLoopThread(/* args */);
    ~EventLoopThread();
    EventLoop* startLoop();
};

