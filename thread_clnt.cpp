#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <iostream>
#include <sys/epoll.h>
#include <vector>
#include<algorithm>
#include<semaphore.h>
#include<thread>
#include<string>
using namespace std;
#define BUFS 512
#define EPOLL_SIZE 50
char msg[BUFS];
void send_th(int,string);
void rcv(int);
int main(int argc,char* argv[])
{
    if(argc!=4)
    {
        cout<<"ERROR INPUT"<<endl;
    }
    int clnt=socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in serv_addr;
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(atoi(argv[2]));
    if(connect(clnt,(struct sockaddr *)&serv_addr,sizeof(serv_addr))==-1)
    {
        cout<<"ERROR CONNECT"<<endl;
    }
    std::thread snd_thread(send_th,clnt,string(argv[3]));
    std::thread rcv_thread(rcv,clnt);
    snd_thread.join();
    rcv_thread.join();
    close(clnt);
    return 0;
}
void send_th(int sock,string name)
{
    while(1)
    {
      string msg;
      getline(cin,msg);
      string sendm=name+": "+msg;
      if(msg=="q"||msg=="Q")
      {
        close(sock);
        exit(0);
      }
      else
      {
        write(sock,sendm.c_str(),sendm.size());
      }
    }
}
void rcv(int sock)
{
    int str_len;
    while(1)
    {
        str_len=read(sock,msg,BUFS-1);
        msg[str_len]=0;
        cout<<msg<<endl;
    }
}