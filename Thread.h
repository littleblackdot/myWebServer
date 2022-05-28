#pragma once

#include<pthread.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <functional>
#include <memory>
#include <string>
#include "CountDownLatch.h"
#include "noncopyable.h"
#include "CurrentThread.h"

class Thread : noncopyable{
typedef std::function<void()> ThreadFunc;
private:
    void setDefaultName();
    bool started_;
    bool joined_;
    pthread_t threadId_;
    pid_t tid_;
    ThreadFunc func_;
    std::string name_;
    CountDownLatch latch_;

public:   
    explicit Thread(const ThreadFunc&, const std::string& name = std::string());
    ~Thread();
    void start();
    int join();
    bool started() { return started_; }
    pid_t tid() { return tid_; }
    const std::string& name() const{ return name_; }
};