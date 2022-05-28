#include "FileUtil.h"
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

BaseFile::BaseFile(std::string fileName): fp_(fopen(fileName.c_str(), "ae")){
    //人为的设置缓冲区
    setbuffer(fp_, buffer_, sizeof(buffer_));
}

void BaseFile::append(const char* logMess, size_t len){
    size_t n = this->write(logMess, len);
    size_t remain = len - n;
    while (remain > 0) {
        size_t x = this->write(logMess + n, remain);
        if (x == 0) {
            int err = ferror(fp_);
            if (err) fprintf(stderr, "AppendFile::append() failed !\n");
            break;
        }
        n += x;
        remain = len - n;
    }
}

BaseFile::~BaseFile(){fclose(fp_);}
void BaseFile::flush(){ fflush(fp_); }

size_t BaseFile::write(const char* logMess, size_t len){
    //fwrite的线程不安全版本，但是写入效率高
    return fwrite_unlocked(logMess, 1, len, fp_);
}