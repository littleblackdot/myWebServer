#include "WebServer.h"
#include "EventLoop.h"
#include<getopt.h>
#include<string>


int main(int argc, char* argv[]){
    int threadNum = 4;
    int port = 80;
    std::string logPath = "./WebServer.log";
    int opt;
   const char *str = "t:l:p:";
   while ((opt = getopt(argc, argv, str)) != -1) {
        switch (opt) {

            case 't': {
                threadNum = atoi(optarg);
                break;
            }

            case 'l': {
                logPath = optarg;
                if (logPath.size() < 2 || optarg[0] != '/') {
                    printf("logPath should start with \"/\"\n");
                    abort();
                }
                break;
            }

            case 'p': {
                port = atoi(optarg);
                break;
            }

            default: break;
        }
   }
   EventLoop mainLoop;
   WebServer server(&mainLoop, threadNum, port);
   server.start();
   //mainLoop.loop();
   return 0;
}