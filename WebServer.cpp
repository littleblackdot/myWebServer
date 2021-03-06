#include "WebServer.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <functional>
#include <cstring>
#include "Util.h"
#include "HttpData.h"
#include "log/Log.h"

WebServer::WebServer(EventLoop* loop, int threadNum, int port)
    : mainLoop_(loop),
    threadNum_(threadNum),
    evLoopThreadPool_(new EventLoopThreadPool(loop, threadNum_)),
    started_(false),
    acceptChannel_(new Channel(loop)),
    port_(port),
    listenFd_(createListenSocket(port)){
    //std::cout<< "listenFd: "<<listenFd_ << std::endl;
    acceptChannel_->setFd(listenFd_);
    handle_for_sigpipe();
    if(setSocketNonBlocking(listenFd_) < 0){
        perror("set socket non block failed");
        abort();
    }
}

void WebServer::start(){
    evLoopThreadPool_->start();
    acceptChannel_->setEvents(EPOLLIN | EPOLLET);
    acceptChannel_->setReadHandler(std::bind(&WebServer::handNewConn, this));
    acceptChannel_->setConnHandler(std::bind(&WebServer::handThisConn, this));
    mainLoop_->addToPoller(acceptChannel_, 0);
    started_ = true;
    mainLoop_->loop();
}

void WebServer::handNewConn(){
    struct sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof clientAddr);
    socklen_t clientAddrLen = sizeof(clientAddr);
    int newConnfd;
    while( (newConnfd = accept(listenFd_, (struct sockaddr*)&clientAddr, &clientAddrLen)) > 0){
        
        EventLoop* tmpLoop = evLoopThreadPool_->getNextLoop();
        LOG << "New connection from " << inet_ntoa(clientAddr.sin_addr) << ":"
                << ntohs(clientAddr.sin_port);
        if(newConnfd >= MAXFDS ){
            close(newConnfd);
            std::cout<< "no more client" << std::endl;
            continue;
        }

        if(setSocketNonBlocking(newConnfd) < 0){
            std::cout<< "Set non-block failed -- server" << std::endl;
            return;
        }
        setSocketNodelay(newConnfd);
        std::shared_ptr<HttpData> httpData(new HttpData(tmpLoop, newConnfd));
        httpData->getChannel()->setHolder(httpData);
        tmpLoop->runInLoop(std::bind(&HttpData::handleNewConnection, httpData));
    }
    acceptChannel_->setEvents(EPOLLIN | EPOLLET);
}

