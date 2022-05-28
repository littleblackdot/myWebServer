#include"Util.h"
#include<errno.h>
#include<fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iconv.h>
#include <iostream>

const int MAX_BUFF = 4096;

ssize_t readn(int fd, void *buff, size_t n){
    size_t nleft = n;
    ssize_t nread = 0;
    ssize_t readSum = 0;
    char *ptr = (char*)buff; 
    while(nleft > 0){
        if((nread = read(fd, ptr, nleft)) < 0){
            if(errno == EINTR){  //系统调用被中断打断
                continue;
                //nread = 0;
            }else if(errno == EAGAIN){  //无数据可读
                return readSum;
            }else{
                return -1;
            }
        }else if(nread == 0){
            break;
        }
        readSum += nread;
        nleft -= nread;
        ptr += nread;
    } 
    return readSum;
}

ssize_t readn(int fd, std::string &inBuffer, bool &isEnd){
    ssize_t nread = 0;
    ssize_t readSum = 0;
    char buff[MAX_BUFF];
    while(true){
        nread = read(fd, buff, MAX_BUFF);
        if(nread < 0 ){
            if(errno == EAGAIN){
                return readSum;
            }else if(errno == EINTR){
                continue;
            }else{
                perror("read error");
                return -1;
            }
        }else if(nread == 0){// read EOF   
            isEnd = true;        
            break;              
        }
        inBuffer += std::string(buff, buff + nread);
        readSum += nread;
    }
    return readSum;
}

ssize_t readn(int fd, std::string &inBuffer){
    bool tmp;
    return readn(fd, inBuffer, tmp);
}

ssize_t writen(int fd, void *buff, size_t n){
    size_t leftNum = n;
    ssize_t writenNum = 0;
    ssize_t writenSum = 0;
    char *ptr = (char*) buff;
    while(leftNum > 0){
        writenNum = write(fd, ptr, leftNum);
        if(writenNum < 0){
            if(errno == EINTR){
                continue;
            } else if(errno == EAGAIN) {
                return writenSum;
            } else{
                return -1;
            }
        }
        writenSum += writenNum;
        leftNum -= writenNum;
        ptr += writenNum;
    }
    return writenSum;
}

ssize_t writen(int fd, std::string &outBuffer){
    size_t writenSum = writen(fd, (void*)outBuffer.c_str(), outBuffer.size());
    if(writenSum  < 0) {
        return -1;
    } else if(writenSum == outBuffer.size()){
        outBuffer.clear();
    }else{
        outBuffer = outBuffer.substr(writenSum);
    }
    return writenSum;
}

void handle_for_sigpipe() {
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    if(sigaction(SIGPIPE, &sa, NULL) < 0){
        std::cout << "set sigpipe ignore fail" << std::endl;
    }
}

int setSocketNonBlocking(int fd){
    int flag = fcntl(fd, F_GETFL, 0);
    if(flag == -1){
        return -1;
    }
    flag |= O_NONBLOCK;
    if(fcntl(fd, F_SETFL, flag) == -1){
        return -1;
    }
    return 0;
}

void setSocketNodelay(int fd) {
    int enable = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (void *)&enable, sizeof(enable));  //禁止发送合并的Nagle算法。
}



//设置优雅关闭， tcp延迟断开，给发送缓冲区残留数据延迟一些时间用来发送
void setSocketNoLinger(int fd) {
  struct linger linger_;
  linger_.l_onoff = 1;
  linger_.l_linger = 30;
  setsockopt(fd, SOL_SOCKET, SO_LINGER, (const char *)&linger_,
             sizeof(linger_));
}


int createListenSocket(int port){
    if(port <  0 || port > 65535 ){
        return -1;
    }
    int listenFd_ = 0;
    if((listenFd_ = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        std::cout << "get  listenFd fail" <<std::endl;
        return -1;
    }
    
    //设置端口复用， 避免bind出现"Address already in use"
    int opt = 1;
    if(setsockopt(listenFd_, SOL_SOCKET, SO_REUSEADDR, &opt,  sizeof(int)) == -1){
        close(listenFd_);
        std::cout << "setsockopt fail" <<std::endl;
        return -1;
    }

    struct sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons((unsigned short)port);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listenFd_, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1){
        close(listenFd_);
        std::cout << "bind fail" <<std::endl;
        return -1;
    }

    if(listen(listenFd_, 2048) == -1){
        std::cout << "listen fail" <<std::endl;
        close(listenFd_);
    }
    
    if (listenFd_ == -1) {
    close(listenFd_);
    return -1;
    }
    return listenFd_;
}


