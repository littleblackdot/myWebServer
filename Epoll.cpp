#include "Epoll.h"
#include "Channel.h"
#include "HttpData.h"
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <deque>
#include <queue>
#include <arpa/inet.h>
#include <iostream>


const int EVENTSSUM = 4096;
const int EPOLLWAIT_TIME = 10000;

Epoll::Epoll(): epollFd_(epoll_create1(EPOLL_CLOEXEC)), events_(EVENTSSUM){
    assert(epollFd_ > 0);
}

Epoll::~Epoll(){}

void Epoll::EpollAdd(SP_Channel channel, int timeout){
    int fd = channel->getFd();
    if(timeout > 0){
        addTimeNode(channel, timeout);
        httpDatas_[fd] = channel->getHolder();
    }
    struct epoll_event event;
    event.data.fd = fd;
    event.events = channel->getEvents();
    channels_[fd] = channel;
    channel->EqualAndUpdateLastEvents();
    if(epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &event) < 0){
        perror("epoll_add fail");
        channels_[fd].reset();
    }
}

void Epoll::EpollMod(SP_Channel channel, int timeout){
    if(timeout > 0){
        addTimeNode(channel, timeout);
    }
    int fd = channel->getFd();
    if(!channel->EqualAndUpdateLastEvents()){
        struct epoll_event event;
        event.data.fd = fd;
        event.events = channel->getEvents();
        if(epoll_ctl(epollFd_, EPOLL_CTL_MOD, fd, &event) < 0){
            perror("epoll_add fail");
            channels_[fd].reset();
        }
    }
}

void Epoll::EpollDel(SP_Channel channel) {
  int fd = channel->getFd();
  struct epoll_event event;
  event.data.fd = fd;
  event.events = channel->getLastEvents();
  if (epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd, &event) < 0) {
    perror("epoll_del error");
  }
  channels_[fd].reset();
  httpDatas_[fd].reset();
}


std::vector<SP_Channel> Epoll::poll(){
    while(true){
        int eventCount = epoll_wait(epollFd_, &*events_.begin(), events_.size(), EPOLLWAIT_TIME);
        if(eventCount < 0){
            perror("epoll wait err");
        }
        std::vector<SP_Channel> ret = getActiveEvents(eventCount);
        if(ret.size() > 0){
            return ret;
        }
    }
}

std::vector<SP_Channel> Epoll::getActiveEvents(int eventNum){
    std::vector<SP_Channel> activeEvents;
    for(int i=0; i<eventNum; ++i){
        int fd = events_[i].data.fd;
        SP_Channel channel = channels_[fd];
        if(channel){
            channel->setRevents(events_[i].events);
            channel->setEvents(0);
            activeEvents.push_back(channel);
        }else{
            std::cout<< "channel is vaild" << std::endl;
        }
    }
    return activeEvents;
}

void Epoll::addTimeNode(SP_Channel channel, int timeout){
    std::shared_ptr<HttpData> t = channel->getHolder();
    if(t){
        timeNodeManager_.addTimeNode(t, timeout);
    }else{
        std::cout<<"timer add fail"<<std::endl;
    }
}

void Epoll::handleExpired() { timeNodeManager_.handleExpiredTimeNode(); }