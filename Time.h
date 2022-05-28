#pragma once

#include<queue>
#include<memory>
#include<deque>

class HttpData;
typedef std::shared_ptr<HttpData> SP_HttpData; 

class TimeNode{
private:
    bool deleted_;
    size_t expiredTime_;    //ms为单位
    SP_HttpData spHttpData_;
public:
    TimeNode(SP_HttpData requestData, int timeout);
    ~TimeNode();
    TimeNode(TimeNode &tn);
    void update(int timeout);
    void clearHttpData();
    bool isValid();
    bool isDeleted() {return deleted_; }
    void setDeleted() { deleted_ = true; }
    size_t getExpiredTime() { return expiredTime_;}
};

struct TimeCmp{
    bool operator()(std::shared_ptr<TimeNode> &a, std::shared_ptr<TimeNode> &b){
        return a->getExpiredTime() > b->getExpiredTime();
    }
};



class TimeManager{
private:
    std::priority_queue<std::shared_ptr<TimeNode>,
        std::deque<std::shared_ptr<TimeNode>>,
         TimeCmp> timerNodeQueue;
public:
    TimeManager();
    ~TimeManager();
    void addTimeNode(SP_HttpData SP_HttpData, int timeout);
    void handleExpiredTimeNode();
};


