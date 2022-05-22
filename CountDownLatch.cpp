#include "CountDownLatch.h"
#include<iostream>
CountDownLatch::CountDownLatch(int count)
    : mutex_(), condition_(mutex_), count_(count) {}

void CountDownLatch::wait() {
  MutexLockGuard lock(mutex_);
 // std::cout<< "in wait" << std::endl;
  //pthread_cond_wait调用上会释放锁当被唤醒时，pthread_cond_wait函数返回时，解除阻塞并且重新申请获取互斥锁
  while (count_ > 0) condition_.wait();
}

void CountDownLatch::countDown() {
  MutexLockGuard lock(mutex_);
 // std::cout<< "in countDown" << std::endl;
  --count_;
  if (count_ == 0) condition_.signalAll();
}

