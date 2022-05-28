#pragma once

#include<functional>
#include<vector>
#include<assert.h>
#include<sys/socket.h>
#include<memory>
#include "noncopyable.h"
#include "Channel.h"
#include "MutexLock.h"
#include "CurrentThread.h"
#include "Epoll.h"





class EventLoop : noncopyable{
private:
    typedef std::function<void()> Functor;
    bool isLooping_;
    bool isQuit_;
    bool eventHandling_;
    std::shared_ptr<Epoll> poller_;
    int weakupFd_;
    mutable MutexLock mutex;
    std::vector<Functor> pendingFunctions;
    bool callingPendingFunctions_;
    const pid_t threadId_;
    SP_Channel weakupChannel_;
    void handleRead();
    void wakeup();
    void handleConn();
    void doPendinfFunctions();
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void quit();
    void shutDown(SP_Channel channel){ shutdown(channel->getFd(), SHUT_WR); }

    void addToPoller(SP_Channel channel, int timeout){ poller_->EpollAdd(channel, timeout); }
    void removeFromPoller(SP_Channel channel){ poller_->EpollDel(channel); }
    void updatePoller(SP_Channel channel, int timeout=0){ poller_->EpollMod(channel, timeout); }
    
    void runInLoop(Functor&& cb);
    void queueInLoop(Functor &&cb);
       
    bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }
    void assertInLoopThread() { assert(isInLoopThread()); }
    
};