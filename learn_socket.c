#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
int main()
{
    int fd= socket(PF_INET,SOCK_STREAM,0);
    int fd2= socekt(PF_INET,SOCK_DGRAM,0);
    struct sockaddr_in serv_sock;
    memset(&serv_sock,0,sizeof(serv_sock));
    char* serv_addr="1.1.1.1";
    char* port="919";
    serv_sock.sin_family=AF_INET;
    serv_sock.sin_port=htnos(atoi(0));
    serv_sock.sin_addr.s_addr=inet_addr(serv_addr);
    bind(fd,(struct sockaddr*)&serv_sock,sizeof(serv_sock));
}