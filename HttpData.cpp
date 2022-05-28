
#include<functional>
#include<iostream>
#include<cstring>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/mman.h>

#include"HttpData.h"
#include"EventLoop.h"
#include "Channel.h"
#include "Util.h"
#include "CodeConvert.h"
#include "log/Log.h"

pthread_once_t MimeType::onceControl = PTHREAD_ONCE_INIT;
std::unordered_map<std::string, std::string> MimeType::mime;

const __uint32_t DEFAULT_EVENT = EPOLLIN | EPOLLET | EPOLLONESHOT;
const int DEFAULT_EXPIRED_TIME = 2000;              // ms
const int DEFAULT_KEEP_ALIVE_TIME = 5 * 60 * 1000;  // ms

void MimeType::init() {
  mime[".html"] = "text/html";
  mime[".avi"] = "video/x-msvideo";
  mime[".bmp"] = "image/bmp";
  mime[".c"] = "text/plain";
  mime[".doc"] = "application/msword";
  mime[".gif"] = "image/gif";
  mime[".gz"] = "application/x-gzip";
  mime[".htm"] = "text/html";
  mime[".ico"] = "image/x-icon";
  mime[".jpg"] = "image/jpeg";
  mime[".png"] = "image/png";
  mime[".txt"] = "text/plain";
  mime[".mp3"] = "audio/mp3";
  mime["default"] = "text/html";
  mime[".json"] = "application//json";
  mime[".css"] = "text/css";
  mime[".js"] = "text/js";
}





std::string MimeType::getMime(const std::string &suffix){
    pthread_once(&onceControl, MimeType::init);
    if(mime.find(suffix) == mime.end()){
        return mime["default"];
    }else{
        return mime[suffix];
    }
}

HttpData:: HttpData(EventLoop* loop, int connfd)
    : loop_(loop),
    channel_(new Channel(loop_, connfd)),
    fd_(connfd),
    err_(false),
    connectionState_(H_CONNECTED),
    httpMethod_(METHOD_GET),
    httpVersion_(HTTP_11),
    processState_(STATE_PARSE_URI),
    headerParseState_(H_START),
    keepAlive_(false),
    readPos_(0)
{
    //channel_->setHolder(shared_from_this());
    channel_->setReadHandler(bind(&HttpData::handleRead, this));
    channel_->setWriteHandler(bind(&HttpData::handleWrite, this));
    channel_->setConnHandler(bind(&HttpData::handleConn, this));
}

void  HttpData::reset(){
    separateTime();
    fileName_.clear();
    readPos_ = 0;
    path_.clear();
    headers_.clear();
    processState_ = STATE_PARSE_URI;
    headerParseState_ = H_START;
}

void HttpData::separateTime(){
     if(timeNode_.lock()){
        std::shared_ptr<TimeNode> tmpNode = timeNode_.lock();
        tmpNode->clearHttpData();
        timeNode_.reset();
    }
}



HeaderState HttpData::parseHeaders(){
    std::string &str = inBuffer_;
    int keyStart = -1;
    int keyEnd = -1;
    int valueStart = -1;
    int valueEnd = -1;
    int parsePos = 0;
    bool notFinish = true;
    size_t i = 0;
    for(; i<str.size() && notFinish; ++i){
        switch(headerParseState_){
            case H_START : {
                if(str[i] == '\n' || str[i] == '\r'){
                    break;
                }
                headerParseState_ = H_KEY;
                keyStart = i;
                parsePos = i;
                break;
            }

            case H_KEY: {
                if (str[i] == ':') {
                    keyEnd = i;
                    if (keyEnd - keyStart <= 0) {
                        return PARSE_HEADER_ERROR;
                    }
                    headerParseState_ = H_COLON;
                } else if (str[i] == '\n' || str[i] == '\r'){
                    return PARSE_HEADER_ERROR;
                }
                break;
            }

            case H_COLON: {// 冒号
                if (str[i] == ' ') {
                    headerParseState_ = H_SPACES_AFTER_COLON;
                } else {
                    return PARSE_HEADER_ERROR;
                }
                break;
            }

            case H_SPACES_AFTER_COLON: {
                headerParseState_ = H_VALUE;
                valueStart = i;
                break;
            }

            case H_VALUE: {
                if (str[i] == '\r') {
                    headerParseState_ = H_CR;
                    valueEnd = i;
                    if (valueEnd - valueStart <= 0){
                        return PARSE_HEADER_ERROR;
                    } 
                } else if (i - valueStart > 255){
                    return PARSE_HEADER_ERROR;
                }
                break;
            }

            case H_CR: {
                if (str[i] == '\n') {
                    headerParseState_ = H_LF;
                    std::string key(str.begin() + keyStart, str.begin() + keyEnd);
                    std::string value(str.begin() + valueStart, str.begin() + valueEnd);
                    headers_[key] = value;
                    parsePos = i; //完整获取一个header信息才更新parsePos, 不然不更新，下次从头再解析一次
                } else{
                    return PARSE_HEADER_ERROR;
                }
                break;
            }
            
            case H_LF: {
                if (str[i] == '\r') {
                    headerParseState_ = H_END_CR;
                } else {
                    keyStart = i;
                    headerParseState_ = H_KEY;
                }
                break;
            }

            case H_END_CR: {
                if (str[i] == '\n') {
                    headerParseState_ = H_END_LF;
                } else{
                    return PARSE_HEADER_ERROR;
                }
                break;
            }

            case H_END_LF: {
                notFinish = false;
                keyStart = i;
                parsePos = i;
                break;
            }
        }
    }

    if(headerParseState_ == H_END_LF){
        str = str.substr(i);
        return PARSE_HEADER_SUCCESS;
    }
    str = str.substr(parsePos);
    return PARSE_HEADER_AGAIN;
}


URIState HttpData::parseUri(){
    std::string &str = inBuffer_;
    std::string cop = str;
    // 读到完整的请求行再开始解析请求
    size_t pos = str.find('\r', readPos_);
    if(pos < 0){
        //LOG<<"PARSE_URI_AGAIN"<<std::endl;
        return PARSE_URI_AGAIN;
    }

    //Http报文中截取第一行
    std::string request = str.substr(0, pos);
    if (str.size() > pos + 1)
        str = str.substr(pos + 1);
    else
        str.clear();

    // 获取Method
    int posGet = request.find("GET");
    int posPost = request.find("POST");
    int posHead = request.find("HEAD");

    if (posGet >= 0) {
        pos = posGet;
        httpMethod_ = METHOD_GET;
    } else if (posPost >= 0) {
        pos = posPost;
        httpMethod_ = METHOD_POST;
    } else if (posHead >= 0) {
        pos = posHead;
        httpMethod_ = METHOD_HEAD;
    } else {
        return PARSE_URI_ERROR;
    }

    // filename
    pos = request.find("/", pos);
    if (pos < 0) {
        fileName_ = "index.html";
        httpVersion_ = HTTP_11;
        return PARSE_URI_SUCCESS;
    } else {
        size_t _pos = request.find(' ', pos);
        if (_pos < 0){
            return PARSE_URI_ERROR;
        } else {
            if (_pos - pos > 1) {
                fileName_ = request.substr(pos + 1, _pos - pos - 1);
                size_t __pos = fileName_.find('?');
                if (__pos >= 0) {
                    fileName_ = fileName_.substr(0, __pos);
                }
                if(fileName_[(fileName_.size()-1)] == '/'){
                    fileName_ += "index.html";
                }
            } else {
                fileName_ = "index.html";
            } 
        }
        pos = _pos;
    }
    // HTTP 版本号
    pos = request.find("/", pos);
    if (pos < 0)
        return PARSE_URI_ERROR;
    else {
        if (request.size() - pos <= 3){
            return PARSE_URI_ERROR;
        } else {
            std::string ver = request.substr(pos + 1, 3);
            if (ver == "1.0"){
                httpVersion_ = HTTP_10;
            } else if (ver == "1.1"){
                httpVersion_ = HTTP_11;
            } else {
                return PARSE_URI_ERROR;
            }
        }
    }
    return PARSE_URI_SUCCESS;
}


AnalysisState HttpData::AnalysisRequest(){
    if(httpMethod_ == METHOD_POST){
        //POST不做处理
    }else if(httpMethod_ == METHOD_GET || httpMethod_ == METHOD_HEAD){
        std::string header;
        if(httpVersion_ == HTTP_10)
            header += "HTTP/1.0 200 OK\r\n";
        else{
            header += "HTTP/1.1 200 OK\r\n";
        }
        if(headers_.find("Connection") != headers_.end() && 
            (headers_["Connection"] == "Keep-Alive" || headers_["Connection"] == "Keep-alive")) {
                keepAlive_ = true;
                header += std::string("Connection: Keep-Alive\r\n") + "Keep-Alive: timeout=" +
                            std::to_string(DEFAULT_KEEP_ALIVE_TIME) + "\r\n";
        }
        int dotPos = fileName_.find_last_of('.');
        std::string fileType;
        if(dotPos < 0){
            fileType = MimeType::getMime("default");
        }else{
            fileType = MimeType::getMime(fileName_.substr(dotPos));
        }

        // echo test
        if (fileName_ == "hello") {
            //std::cout<<"echo hello"<<std::endl;
            outBuffer_ = header + "Content-type: text/plain\r\n"
            "Content-Length: " + "11" + "\r\n"+"\r\nHello World";//"HTTP/1.1 200 OK\r\nContent-type: text/plain\r\n\r\nHello World";
            return ANALYSIS_SUCCESS;
        }

        // if (fileName_ == "favicon.ico" || fileName_ == "favicon.png") {
        //     header += "Content-Type: image/png\r\n";
        //     header += "Content-Length: " + std::to_string(sizeof favicon) + "\r\n";
        //     header += "Server: LittleBlackDot's Web Server\r\n";

        //     header += "\r\n";
        //     outBuffer_ += header;
        //     outBuffer_ += std::string(favicon, favicon + sizeof favicon);
        //     ;
        //     return ANALYSIS_SUCCESS;
        // }


        //处理url中的'%'编码
        int u8Start = fileName_.find('%');
        if(u8Start > 0){
            int u8End = fileName_.find('/', u8Start);
            std::string u8Codes = fileName_.substr(u8Start, u8End-u8Start);
            std::string fileName_chPart =  gb2312UrlCodeToGb2312Code(u8Codes);
            fileName_ = fileName_.substr(0, u8Start) + fileName_chPart + fileName_.substr(u8End);
        }

        std::string root = "./hexo/";
        fileName_ = root + fileName_;
        struct stat goalFileState;
        if(stat(fileName_.c_str(), &goalFileState) < 0){
            std::cout<< "find file: " + fileName_ + " fail" << std::endl;
            header.clear();
            handleErr(fd_, 404, "Not Found!");
            return ANALYSIS_ERROR;
        }
        header += "Content-Type: " + fileType + "\r\n";
        header += "Content-Length: " + std::to_string(goalFileState.st_size) + "\r\n";
        header += "Server: LittleBlackDot's Web Server\r\n";
        // 头部结束
        header += "\r\n";
        outBuffer_ += header;
        if (httpMethod_ == METHOD_HEAD) return ANALYSIS_SUCCESS;
        int goalFileFd = open(fileName_.c_str(), O_RDONLY, 0);
        if(goalFileFd < 0){
            std::cout<< "open file: " + fileName_ + " fail" << std::endl;
            header.clear();
            handleErr(fd_, 404, "Not Found!");
            return ANALYSIS_ERROR;
        }
        void *mmapPtr = mmap(NULL, goalFileState.st_size, PROT_READ, MAP_PRIVATE, goalFileFd, 0);
        close(goalFileFd);
        if(mmapPtr == (void*) -1){
            munmap(mmapPtr, goalFileState.st_size);
            outBuffer_.clear();
            handleErr(fd_, 404, "Not Found");
            return ANALYSIS_ERROR;
        }

        char *srcAddr = (char*) mmapPtr;
        outBuffer_ += std::string(srcAddr, srcAddr + goalFileState.st_size);
        munmap(mmapPtr, goalFileState.st_size);
        return ANALYSIS_SUCCESS;
    }
    return ANALYSIS_ERROR;
}


void HttpData::handleRead(){
    __uint32_t &events = channel_->getEvents();
    do{
        bool isEnd = false;
        int readNum = readn(fd_, inBuffer_, isEnd);
        LOG << "Request: "<< inBuffer_;
        //std::cout<< "from--------"<< fd_ << std::endl;
        if(connectionState_ == H_DISCONNECTING){
            inBuffer_.clear();
            break;
        }

        if(readNum < 0) {
            perror("HttpData::handleRead readn:");
            err_ =  true;
            handleErr(fd_, 400, "Bad Request");
            break;
        } else if(isEnd) {
        /*
            读sockfd读到EOF，可能是RequestAborted，或者来自网络的数据没有达到等原因
            最可能是对端已经关闭了，统一按照对端已经关闭处理
        */
            connectionState_ = H_DISCONNECTING;
            if(readNum == 0) {
                break;
            }
        }

        if(processState_ == STATE_PARSE_URI){
            URIState flag = this->parseUri();
            if(flag == PARSE_URI_AGAIN){
                break;
            }else if(flag == PARSE_URI_ERROR) {
                perror("HttpData::handelRead: parseUri fail");
                std::cout <<"HttpData::handelRead: parseUri fail " << "FD = " << fd_ 
                        << "," << inBuffer_ << "******"<<std::endl;
                inBuffer_.clear();
                err_ = true;
                handleErr(fd_, 400, "Bad Request");
                break;
            }else{
                processState_ = STATE_PARSE_HEADERS;
            }
        }

        if(processState_ == STATE_PARSE_HEADERS){
            HeaderState flag = this->parseHeaders();
            if(flag == PARSE_HEADER_AGAIN) {
                break;
            }else if (flag == PARSE_HEADER_ERROR) {
                perror("HttpData::handelRead->parseHeaders fail");
                err_ = true;
                handleErr(fd_, 400, "Bad Request");
                break;
            }

            if (httpMethod_ == METHOD_POST) {
                // POST方法准备
                processState_ = STATE_RECV_BODY;
            } else {
                processState_ = STATE_ANALYSIS;
            }

            if(processState_ == STATE_RECV_BODY){
                int contentLength = -1;
                if(headers_.find("Content-length") != headers_.end()){
                    contentLength = stoi(headers_["Content-length"]);
                } else {
                    err_ = true;
                    handleErr(fd_, 400, "Bad Request: lack of argument (Content-length)");
                    break;
                }
                if(int(inBuffer_.size()) < contentLength){
                    break;
                }
                processState_ = STATE_ANALYSIS;
            }

            if(processState_ == STATE_ANALYSIS){
                AnalysisState flag = this->AnalysisRequest();
                if(flag == ANALYSIS_SUCCESS){
                    processState_ = STATE_FINISH;
                    break;
                } else {
                    err_= true;
                    break;  
                }
            }    
        }     
    }while(false);
    
    if(!err_ && outBuffer_.size() > 0){
        handleWrite();
    }

    if(!err_ && processState_ == STATE_FINISH){
        this->reset();
        if(inBuffer_.size() > 0){
            if(connectionState_ != H_DISCONNECTING){
                //std::cout<<"read again"<<std::endl;
                handleRead();
            }
        }

    }else if(!err_ && connectionState_ != H_DISCONNECTED){
        events |= EPOLLIN;      
    }

}


void  HttpData::handleWrite(){
    //std::cout<<"handleWrite for fd "<< fd_ <<std::endl;
    if(!err_ && connectionState_ != H_DISCONNECTED) {
        __uint32_t &events = channel_->getEvents();
        int sendNum = 0;
        //std::cout<< "out len" << outBuffer_.size()<< std::endl;
        if((sendNum = writen(fd_, outBuffer_)) < 0){
            perror("HttpData::handleWrite->writen");
            events = 0;
            err_ = true;
        }
        //std::cout<< "sendNum "<< sendNum << std::endl;
        if(outBuffer_.size() > 0){
            //std::cout<< "content left" << std::endl;
            events |= EPOLLOUT;
        }
    }
}

void HttpData::handleConn(){
    separateTime();
    __uint32_t &events = channel_->getEvents();
    if(!err_ && connectionState_ == H_CONNECTED){
        if(events != 0){
            int timeout = DEFAULT_EXPIRED_TIME;
            if(keepAlive_){
                timeout = DEFAULT_KEEP_ALIVE_TIME;
            }
            if((events & EPOLLIN) && (events & EPOLLOUT)){
                events = __uint32_t(0);
                events |= EPOLLOUT;
            }
            events |= EPOLLET;
            loop_->updatePoller(channel_, timeout);
        } else {
            int timeout = keepAlive_ ? DEFAULT_KEEP_ALIVE_TIME : (DEFAULT_KEEP_ALIVE_TIME >> 1);
            events |= ( EPOLLIN | EPOLLET );
            loop_->updatePoller(channel_, timeout);
        }
    } else if(!err_ && connectionState_ == H_DISCONNECTING && (events & EPOLLOUT)){
        events = ( EPOLLOUT | EPOLLET );
    } else {
        loop_->runInLoop(bind(&HttpData::closeConnection, shared_from_this()));
    }
}

void HttpData::handleErr(int fd, int errCode, std::string msg){
    msg = " " +  msg;
    char sendBuffer[4096];
    std::string  bodyBuffer, headerBuffer;
    bodyBuffer += "<html><title>哎~出错了</title>";
    bodyBuffer += "<body bgcolor=\"ffffff\">";
    bodyBuffer += std::to_string(errCode) + msg;
    bodyBuffer += "<hr><em> LittleBlackDot's Web Server</em>\n</body></html>";

    headerBuffer += "HTTP/1.1 " + std::to_string(errCode) + msg + "\r\n";
    headerBuffer += "Content-Type: text/html\r\n";
    headerBuffer += "Connection: Close\r\n";
    headerBuffer += "Content-Length: " + std::to_string(bodyBuffer.size()) + "\r\n";
    headerBuffer += "Server: LittleBlackDot's Web Server\r\n";
    headerBuffer += "\r\n";

    // 错误处理不考虑writen不完的情况
    sprintf(sendBuffer, "%s", headerBuffer.c_str());
    writen(fd, sendBuffer, strlen(sendBuffer));
    sprintf(sendBuffer, "%s", bodyBuffer.c_str());
    writen(fd, sendBuffer, strlen(sendBuffer));
}

void HttpData::closeConnection(){
    connectionState_ = H_DISCONNECTED;
    std::shared_ptr<HttpData> guard(shared_from_this()); //保证 loop_->removeFromPoller(channel_);执行完本HttpData对象不会因为引用计数为零被释放
    //shutdown(fd_, SHUT_RDWR);
    loop_->removeFromPoller(channel_);
}

void HttpData::handleNewConnection(){
    channel_->setEvents(DEFAULT_EVENT);
    loop_->addToPoller(channel_, DEFAULT_EXPIRED_TIME);
}



               