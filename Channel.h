#pragma once

#include "noncopyable.h"
#include<functional>
#include<sys/epoll.h>
#include<memory>

class EventLoop;
class HttpData;



class Channel : noncopyable {

typedef std::function<void()> CallBack;

private:
    EventLoop* ownerLoop_;
    int fd_;
    __uint32_t events_;
    __uint32_t revents_;
    __uint32_t lastEvents_;
    std::weak_ptr<HttpData> holder_;
    
    CallBack readHandler_;
    CallBack writeHandler_;
    CallBack errHandler_;
    CallBack connHandler_;

public:
    Channel(EventLoop*);
    Channel(EventLoop*, int fd);
    ~Channel() {};
    
    int getFd() { return fd_; }
    void setFd(int fd) { fd_ = fd; }
    
    void setHolder(std::shared_ptr<HttpData> holder) { holder_ = holder; }
    std::shared_ptr<HttpData> getHolder() { return holder_.lock(); }       //diff
    
    void setEvents(__uint32_t events) { events_ = events; }
    void setRevents(__uint32_t revents) { revents_ = revents; }
    
    void setReadHandler(CallBack &&readHandler) { readHandler_ = readHandler; }
    void setWriteHandler(CallBack &&writeHandler) { writeHandler_ = writeHandler; }
    void setErrHandler(CallBack &&errHandler) { errHandler_ = errHandler; }
    void setConnHandler(CallBack &&connHandler) { connHandler_ = connHandler; }
    void handEvent();
    
    bool EqualAndUpdateLastEvents();
    __uint32_t& getEvents() {return events_; } 
    __uint32_t getLastEvents() { return lastEvents_; }
};

typedef std::shared_ptr<Channel> SP_Channel;