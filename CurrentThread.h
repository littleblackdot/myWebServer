#pragma once
#include <stdint.h>
#include<iostream>
#include<unistd.h>
#include<sys/syscall.h>


namespace CurrentThread {
// internal
extern __thread int t_cachedTid;
extern __thread char t_tidString[32];
extern __thread int t_tidStringLength;
extern __thread const char* t_threadName;

pid_t getTid() { return static_cast<pid_t>(syscall(SYS_gettid)); }

void cacheTid(){
    if(t_cachedTid == 0){
        t_cachedTid = getTid();
        t_tidStringLength = snprintf(t_tidString, sizeof(t_tidString), "%5d ", t_cachedTid);
    }
}
inline int tid() {
  if (__builtin_expect(t_cachedTid == 0, 0)) {
    cacheTid();
  }
  return t_cachedTid;
}

inline const char* tidString()  // for logging
{
  return t_tidString;
}

inline int tidStringLength()  // for logging
{
  return t_tidStringLength;
}

inline const char* name() { return t_threadName; }
}