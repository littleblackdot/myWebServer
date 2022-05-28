#pragma once

#include<string>
#include "../noncopyable.h"

class BaseFile : noncopyable{
private:
    size_t write(const char *logMess, size_t len);
    FILE *fp_;
    char buffer_[64*1024];
public: 
    explicit BaseFile(std::string fileName);
    ~BaseFile();
    void append(const char *logMess, size_t len);
    void flush();
};
