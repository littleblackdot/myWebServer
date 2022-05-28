#include"Channel.h"
#include<iostream>


Channel::Channel(EventLoop* loop)
    : ownerLoop_(loop), events_(0), lastEvents_(0), fd_(0) {}

Channel::Channel(EventLoop* loop, int fd)
    : ownerLoop_(loop), fd_(fd), events_(0), lastEvents_(0) {} 

void Channel::handEvent(){
    //std::cout<< "handle event for "<< fd_<< std::endl;
    events_ = 0;
    if((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN)) {//EPOLLHUP:tcp连接读写都被关闭
      events_ = 0;
      //revents_ = 0; //diff
      return;
    }
    if(revents_ & EPOLLERR){
        if(errHandler_){
            errHandler_();
        }
        events_ = 0;
        return ;
    }
    if(revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)){
        if(readHandler_){     //EPOLLRDHUP: 关闭读(对端调用close 或者 shutdown(SHUT_WR))，在读一次清空缓冲区
            //std::cout<<"handle read for " <<fd_ << std::endl; 
            readHandler_();   //EPOLLPRI： 表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）
        } 
    }
    if(revents_ & EPOLLOUT){
        if(writeHandler_){
            //std::cout<<"handle write for " << fd_ << std::endl;
            writeHandler_();
        }
    }
    if(connHandler_){
        //std::cout<<"handle conn for " <<fd_ << std::endl;
        connHandler_();
    }
}

bool Channel::EqualAndUpdateLastEvents(){
    bool ret = (lastEvents_ == events_);
    lastEvents_ = events_;
    return ret;
}