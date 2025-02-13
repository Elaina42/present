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
using namespace std;
#define BUFS 512
#define EPOLL_SIZE 50
vector<int> clnt_fd;
int main(int argc,char* argv[])
{
    if(argc!=2)
    {
        cout<<"Usage: "<<argv[0]<<" port";
    }
    int str_len;char msg[BUFS];
    int serve_sock=socket(PF_INET,SOCK_STREAM,0),clnt_sock;
    struct sockaddr_in serv_addr,clnt_addr;
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(atoi(argv[1]));
    if(bind(serve_sock,(sockaddr *)&serv_addr,sizeof(serv_addr))==-1)
    {
        cout<<"BIND ERROR";
    }
    if(listen(serve_sock,5)==-1)
    {
        cout<<"LISTEN ERROR";
    }
    int epfd=epoll_create(EPOLL_SIZE);
    epoll_event ev,*events;events=new epoll_event[EPOLL_SIZE];
    ev.data.fd=serve_sock;
    ev.events=EPOLLIN;
    epoll_ctl(epfd,EPOLL_CTL_ADD,serve_sock,&ev);
    int event_cnt;socklen_t adr_sz=sizeof(clnt_addr);
    while(1)
    {
        event_cnt=epoll_wait(epfd,events,EPOLL_SIZE,-1);
        if(event_cnt==-1)
        {
            cout<<"EPOLL ERROR"<<endl;break;
        }
        for(int i=0;i<event_cnt;i++)
        {
            if(events[i].data.fd==serve_sock)
            {
                clnt_sock=accept(serve_sock,(sockaddr*)&clnt_addr,&adr_sz);
                ev.data.fd=clnt_sock;
                ev.events=EPOLLIN;
                epoll_ctl(epfd,EPOLL_CTL_ADD,clnt_sock,&ev);
                clnt_fd.push_back(clnt_sock);
            }
            else
            {
                str_len=read(events[i].data.fd,msg,BUFS);
                if(str_len==0)
                {
                    epoll_ctl(epfd,EPOLL_CTL_DEL,events[i].data.fd,NULL);
                    close(events[i].data.fd);
                    cout<<"CLOSE "<<events[i].data.fd<<endl;
                    clnt_fd.erase(std::remove(clnt_fd.begin(),clnt_fd.end(),events[i].data.fd),clnt_fd.end());
                }
                else
                {
                    for(auto it:clnt_fd)
                    {
                        write(it,msg,str_len);
                    }
                }
            }
        }
    }
    close(epfd);
    close(serve_sock);
    return 0;
}