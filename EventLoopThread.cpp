#include "EventLoopThread.h"
#include<functional>

EventLoopThread::EventLoopThread()
  : loop_(nullptr),
    isExit_(false),
    thread_(std::bind(&EventLoopThread::ThreadFunc, this), "EventLoopThread"),
    mutex_(),
    cond_(mutex_){}

EventLoopThread::~EventLoopThread(){
    isExit_ = true;
    if(loop_ != nullptr){    
        loop_->quit();
        thread_.join();
    }
}

EventLoop* EventLoopThread::startLoop(){
    assert(!thread_.started());
    thread_.start();
    {
        MutexLockGuard lockGuard(mutex_);
        while(loop_ == nullptr){
            cond_.wait();
        }
    }
    return loop_;
}


void EventLoopThread::ThreadFunc(){
    EventLoop loop;
    {
        MutexLockGuard lockGuard(mutex_);
        loop_ =  &loop;
        cond_.signal();
    }
    loop.loop();
    loop_ = NULL;
}