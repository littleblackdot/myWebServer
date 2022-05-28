#include"EventLoop.h"
#include"Util.h"
#include<sys/eventfd.h>
#include<functional>
#include<iostream>
#include<assert.h>


__thread EventLoop* loopInLoopThread = nullptr;

int createEventFd(){
    int eventFd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC); 
    if(eventFd < 0){
        std::cout<< "fail in eventfd"<<std::endl;
        abort();
    }
    return eventFd;
}

EventLoop::EventLoop()
  : isLooping_(false),
    isQuit_(false),
    eventHandling_(false),
    poller_(new Epoll()),
    weakupFd_(createEventFd()),
    callingPendingFunctions_(false),
    weakupChannel_(new Channel(this, weakupFd_)),
    threadId_(CurrentThread::tid()) {
        if(loopInLoopThread == nullptr){
            loopInLoopThread = this;
        }
        weakupChannel_->setEvents(EPOLLIN | EPOLLET);
        weakupChannel_->setReadHandler(std::bind(&EventLoop::handleRead, this));
        weakupChannel_->setConnHandler(std::bind(&EventLoop::handleConn, this));
        poller_->EpollAdd(weakupChannel_, 0);
        std::cout<<"weakupFd "<< weakupFd_<<std::endl;
}


EventLoop::~EventLoop(){
    close(weakupFd_);
    loopInLoopThread = nullptr;
}
void EventLoop::handleRead(){
    uint64_t one = 1;
    ssize_t n = readn(weakupFd_, &one, sizeof(one));
    if(n != sizeof(one)){
        std::cout<<"EventLoop::handleRead() reads " << n << " bytes instead of 8" << std::endl;
    }
    weakupChannel_->setEvents(EPOLLIN | EPOLLET);
}

void EventLoop::handleConn(){
    updatePoller(weakupChannel_, 0);
}

void EventLoop::wakeup(){
    //std::cout<<"weakup "<<weakupFd_<< std::endl;
    __uint64_t one = 1;
    ssize_t sum = writen(weakupFd_, (char*)&one, sizeof(one));
    if(sum != sizeof(one)){
        std::cout<<"EventLoop::handleRead() write " << sum << " bytes instead of 8" << std::endl;
    }
}

void EventLoop::runInLoop(Functor&& cb){
    if(isInLoopThread()){
        cb();
    } else {
        queueInLoop(std::move(cb));
    }
}

void EventLoop::queueInLoop(Functor&& cb){
    {
        MutexLockGuard lockGuard(mutex);
        pendingFunctions.emplace_back(std::move(cb));
    }
    if(!isInLoopThread() || callingPendingFunctions_){
        wakeup();
    }
}

void EventLoop::loop(){
    assert(!isLooping_);
    assert(isInLoopThread());
    isLooping_ = true;
    isQuit_ = false;
    std::vector<SP_Channel> rets;
    while(!isQuit_){
        rets.clear();
        rets = poller_->poll();
        eventHandling_ = true;
        for(auto& it : rets){
            it->handEvent();
        }
        eventHandling_ = false;
        doPendinfFunctions();
        poller_->handleExpired();
    }
    isLooping_ = false;
}

void EventLoop::quit(){
    isQuit_ = true;
    if(!isInLoopThread()){
        wakeup();
    }
}

void EventLoop::doPendinfFunctions(){
    std::vector<Functor> funcs;
    callingPendingFunctions_ = true;
    {
        MutexLockGuard lockGuard(mutex);
        funcs.swap(pendingFunctions);
    }
    for(int i=0; i < funcs.size(); i++){
        funcs[i]();
    }
    callingPendingFunctions_ = false;
}
