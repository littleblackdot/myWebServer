#pragma once

#include<memory>
#include<vector>
#include "EventLoopThread.h"
#include  "noncopyable.h"

class EventLoopThreadPool : noncopyable{
private:
    EventLoop* baseLoop_;
    bool started_;
    int threadNum_;
    int next_;
    std::vector<std::shared_ptr<EventLoopThread>> loopThreads_;
    std::vector<EventLoop*> loops_;
public:
    EventLoopThreadPool(EventLoop* baseLoop, int threadNum);
    ~EventLoopThreadPool(){ std::cout<< "~EventLoopThreadPoll()" << std::endl;}
    void start();
    EventLoop* getNextLoop();
};