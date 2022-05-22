#include"EventLoopThreadPool.h"
#include "EventLoop.h"

EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop, int threadNum)
  :threadNum_(threadNum), baseLoop_(baseLoop) {
      if(threadNum_ <= 0){
          std::cout << " threadNum < 0" << std::endl;
          abort();
      }
}

void EventLoopThreadPool::start(){
    baseLoop_->assertInLoopThread();
    started_ = true;
    for(int i=0; i<threadNum_; ++i){
        std::shared_ptr<EventLoopThread> tmpThread(new EventLoopThread());
        loopThreads_.push_back(tmpThread);
        loops_.push_back(tmpThread->startLoop());
    }
}


EventLoop* EventLoopThreadPool::getNextLoop(){
    baseLoop_->assertInLoopThread();
    assert(started_);
    EventLoop* loop = baseLoop_;
    
    if(!loops_.empty()){
        loop = loops_[next_];
        next_ = (next_ + 1) % threadNum_;
    }

    return loop;
}
