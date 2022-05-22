#pragma once

#include<pthread.h>
#include<time.h>
#include<cstdint>
#include<errno.h>
#include "MutexLock.h"
#include "noncopyable.h"

class Condition : noncopyable{
private:
    MutexLock &mutexLock_;
    pthread_cond_t cond_;

public:
    explicit Condition(MutexLock &mutexLock) : mutexLock_(mutexLock){
        pthread_cond_init(&cond_, NULL);
    }
    ~Condition(){ pthread_cond_destroy(&cond_);}
    void wait() { pthread_cond_wait(&cond_, mutexLock_.get()); }
    void signal(){ pthread_cond_signal(&cond_); }
    void signalAll(){ pthread_cond_broadcast(&cond_); }
    bool waitForSeconds(int seconds){
        struct timespec abstime;
        clock_gettime(CLOCK_REALTIME, &abstime);
        abstime.tv_sec += time_t(seconds);
        return ETIMEDOUT == pthread_cond_timedwait(&cond_, mutexLock_.get(), &abstime);
    }
};