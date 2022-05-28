#pragma once

#include<sys/epoll.h>
#include<memory>
#include<unordered_map>
#include<vector>
#include "Time.h"
#include "noncopyable.h"

class HttpData;
class Channel;


typedef std::shared_ptr<HttpData> SP_HttpData;
typedef std::shared_ptr<Channel> SP_Channel;

class Epoll : noncopyable {
public:
    Epoll();
    ~Epoll();
    void EpollAdd(SP_Channel chan, int timeout);
    void EpollMod(SP_Channel chan, int timeout);
    void EpollDel(SP_Channel chan);
    std::vector<SP_Channel> poll();
    std::vector<SP_Channel> getActiveEvents(int eventNum);
    void addTimeNode(SP_Channel, int);
    int getEpollFd() { return epollFd_; }
    void handleExpired();
private:
    static const int MAXFDS = 100000;
    int epollFd_;
    std::vector<epoll_event> events_;
    SP_Channel channels_[MAXFDS];   //fd做key值
    SP_HttpData httpDatas_[MAXFDS];  //fd做key值
    TimeManager timeNodeManager_;
};
