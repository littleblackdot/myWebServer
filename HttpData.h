#pragma once
#include<memory>
#include<unistd.h>
#include<unordered_map>
#include<map>
#include "EventLoop.h"
#include "Channel.h"


enum ProcessState{
    STATE_PARSE_URI = 1,
    STATE_PARSE_HEADERS,
    STATE_RECV_BODY,
    STATE_ANALYSIS,
    STATE_FINISH,
};


enum URIState {
  PARSE_URI_AGAIN = 1,
  PARSE_URI_ERROR,
  PARSE_URI_SUCCESS,
};


enum HeaderState {
  PARSE_HEADER_SUCCESS = 1,
  PARSE_HEADER_AGAIN,
  PARSE_HEADER_ERROR
};

enum AnalysisState { 
    ANALYSIS_SUCCESS = 1,
    ANALYSIS_ERROR
};

enum HeadParseState {
  H_START = 0,
  H_KEY,
  H_COLON,   //冒号
  H_SPACES_AFTER_COLON,
  H_VALUE,
  H_CR,
  H_LF,
  H_END_CR,
  H_END_LF
};

enum ConnectionState {
    H_CONNECTED = 0,
    H_DISCONNECTING,
    H_DISCONNECTED 
};


enum HttpMethod { 
    METHOD_POST = 1, 
    METHOD_GET, 
    METHOD_HEAD 
};


enum HttpVersion { 
    HTTP_10 = 1, 
    HTTP_11 
};

class MimeType{
private:
    static pthread_once_t onceControl;
    static void init();
    static std::unordered_map<std::string, std::string> mime;
    MimeType();
    MimeType(MimeType &m);
public:
    static std::string getMime(const std::string &suffix);
};


class HttpData : public std::enable_shared_from_this<HttpData>{
private:
    EventLoop *loop_;
    std::shared_ptr<Channel> channel_;
    
    int readPos_;
    int fd_;
    
    std::string inBuffer_;
    std::string outBuffer_;
    std::string fileName_;
    std::string path_;


    bool err_;
    bool keepAlive_;
    std::map<std::string, std::string> headers_;
    std::weak_ptr<TimeNode> timeNode_;

    ConnectionState connectionState_;
    HttpMethod httpMethod_;
    HttpVersion httpVersion_;
    ProcessState processState_;
    HeadParseState headerParseState_;

    void handleRead();
    void handleWrite();
    void handleConn();
    void handleErr(int fd, int errNum, std::string Msg);
    URIState parseUri();
    HeaderState parseHeaders();
    AnalysisState AnalysisRequest();
public:
    HttpData(EventLoop *loop, int connfd);
    ~HttpData() { close(fd_); }
    void reset();
    void separateTime();
    void linkTime(std::shared_ptr<TimeNode> timeNode){ timeNode_ = timeNode; }
    std:: shared_ptr<Channel> getChannel() { return channel_; }
    EventLoop* getLoop() { return loop_; }
    void closeConnection();
    void handleNewConnection();
};

typedef std::shared_ptr<HttpData> SP_HttpData;