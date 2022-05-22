#pragma once

#include<pthread.h>
#include "noncopyable.h"

class MutexLock : noncopyable{
private:
    pthread_mutex_t mutex_;
    friend class Condition;
    pthread_mutex_t* get() { return &mutex_; }

public:
    MutexLock() {pthread_mutex_init(&mutex_, NULL); }
    ~MutexLock(){
        pthread_mutex_lock(&mutex_);
        pthread_mutex_destroy(&mutex_);
    }
    void lock() { pthread_mutex_lock(&mutex_); }
    void unlock() { pthread_mutex_unlock(&mutex_); }
};


class MutexLockGuard : noncopyable{
private:
    MutexLock &mutexLock_;
public:
    explicit MutexLockGuard(MutexLock &mutexLock) :mutexLock_(mutexLock) {mutexLock_.lock(); }
    ~MutexLockGuard(){ mutexLock_.unlock(); } 
};