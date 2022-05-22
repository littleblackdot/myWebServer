#include "Thread.h"
#include "CurrentThread.h"
#include <sys/prctl.h>
#include<assert.h>

namespace CurrentThread{
    __thread int t_cachedTid = 0;
    __thread char t_tidString[32];
    __thread int t_tidStringLength = 6;
    __thread const char* t_threadName = "default";
}



// 为了在线程中保留name,tid这些数据
struct ThreadData{
    typedef std::function<void()> ThreadFunc;
    ThreadFunc func_;
    std::string name_;
    pid_t* tId_;
    CountDownLatch* latch_;
    ThreadData(const ThreadFunc& func, const std::string &name,
                pid_t* tId, CountDownLatch *latch)
        : func_(func), name_(name), tId_(tId), latch_(latch) {}
    
    void runInThread(){
        *tId_ = CurrentThread::getTid();
        tId_ = nullptr;
        latch_->countDown();
        latch_ = nullptr;
        CurrentThread::t_threadName = name_.empty() ? "runningThread" : name_.c_str();
        prctl(PR_SET_NAME, CurrentThread::t_threadName);
        func_();
        CurrentThread::t_threadName = "finished";
    }
};

void* startThread(void* data){
    ThreadData* threadData = (ThreadData*)(data);
    threadData->runInThread();
    delete threadData;
    return NULL;
}

Thread::Thread(const ThreadFunc &func, const std::string& name)
  : started_(false),
    joined_(false),
    threadId_(0),
    tid_(0),
    func_(func),
    name_(name),
    latch_(1){
        setDefaultName();
}

Thread::~Thread(){ 
    if(started_ && !joined_){
        pthread_detach(threadId_);
    }
}

void Thread::setDefaultName(){
    if(name_.empty()){
        name_ = "Thread";
    }
}

void Thread::start(){
    assert(!started_);
    started_ = true;
    ThreadData* data = new ThreadData(func_, name_, &tid_, &latch_);
    if(pthread_create(&threadId_, NULL, startThread, (void*)data) == 0){ //create thread succcess
        latch_.wait();
        assert(tid_ > 0);
        std::cout<<"in start" << std::endl;
    } else {
        started_ = false;
        delete data;
    }
}

int Thread::join(){
    assert(started_);
    assert(!joined_);
    joined_ = true;
    return pthread_join(threadId_, NULL);
}