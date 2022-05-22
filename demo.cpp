#include<iostream>
#include<memory>
#include"Thread.h"
using namespace std;


void func(){
    for(int  i=0; i<10; i++){
        cout<<"working"<<endl;
        sleep(1);
    }    
}

int main(){
    ::Thread thread(function<void()>(func), "test");
    thread.start();
    while(1);
}