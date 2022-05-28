#pragma once

#include<memory>
#include "Channel.h"
#include "EventLoop.h"
#include "EventLoopThreadPool.h"


class WebServer{
private:
    EventLoop *mainLoop_;
    int threadNum_;
    std::unique_ptr<EventLoopThreadPool> evLoopThreadPool_;
    bool started_;
    std::shared_ptr<Channel> acceptChannel_;
    int port_;
    int listenFd_;
    static const int MAXFDS = 100000;
public:
    WebServer(EventLoop *loop, int threadNum, int port);
    ~WebServer(){}
    EventLoop* getLoop() const {return mainLoop_; }
    void start();
    void handNewConn();
    void handThisConn() { mainLoop_-> updatePoller(acceptChannel_, 0); }
};