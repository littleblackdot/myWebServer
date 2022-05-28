#include"Time.h"
#include"HttpData.h"
#include<sys/time.h>
#include<unistd.h>


TimeNode:: TimeNode(SP_HttpData httpData, int timeout){
    spHttpData_ = httpData;
    deleted_ = false;
    struct timeval now;
    gettimeofday(&now, NULL);
    expiredTime_ = (((now.tv_sec % 10000) * 1000) + (now.tv_usec/1000)) + timeout;
}

TimeNode::TimeNode(TimeNode &tn)
    : spHttpData_(tn.spHttpData_), expiredTime_(0) {}

TimeNode:: ~TimeNode(){
    if(spHttpData_){
        spHttpData_->closeConnection();
    }
}

void TimeNode:: update(int timeout){
    struct timeval now;
    gettimeofday(&now, NULL);
    expiredTime_ = (((now.tv_sec % 10000) * 1000) + (now.tv_usec/1000)) + timeout;
}

//判断时间是否到期
bool TimeNode:: isValid(){
    struct timeval now;
    gettimeofday(&now, NULL);
    size_t tmp = (((now.tv_sec % 10000) * 1000) + (now.tv_usec/1000));
    if(tmp < expiredTime_){
        return true;
    }else{
        setDeleted();
        return false;
    }
}

void TimeNode::clearHttpData(){
    spHttpData_.reset();
    setDeleted();
}



TimeManager::TimeManager() {}

TimeManager::~TimeManager() {}

void TimeManager::addTimeNode(SP_HttpData spHttpData, int timeout){
    std::shared_ptr<TimeNode> node(new TimeNode(spHttpData, timeout));
    timerNodeQueue.push(node);
    spHttpData->linkTime(node);
}

void TimeManager::handleExpiredTimeNode(){
    while(!timerNodeQueue.empty()){
        std::shared_ptr<TimeNode> tmpNode = timerNodeQueue.top();
        if(!tmpNode->isValid() || tmpNode->isDeleted()){
            timerNodeQueue.pop();
        }else{
            break;
        }
    }
}



