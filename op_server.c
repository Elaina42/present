#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

int main(int argc,char* argv[])
{
    int serve_sock=socket(PF_INET,SOCK_STREAM,0);
    int client_sock;
    struct sockaddr_in serve_addr;
    struct sockaddr_in client_addr;
    memset(&serve_addr,0,sizeof(serve_addr));
    serve_addr.sin_family=AF_INET;
    serve_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serve_addr.sin_port=htons(atoi(argv[1]));
    if(bind(serve_sock,(struct sockaddr *)&serve_addr,sizeof(serve_addr))==-1)fputs("bind_errr",stdout);
    if(listen(serve_sock,6)==-1)fputs("listen error",stdout);
    fputs("MAY acc",stdout);
    socklen_t clnt_addr_size=sizeof(client_addr);
    client_sock=accept(serve_sock,(struct sockaddr*)&client_addr,&clnt_addr_size);
    char opinfo[BUF_SIZE];
    int recv_ln=0;
    int op_num=0;
    read(client_sock,&op_num,1);
    printf("op_num :%d",op_num);
    while(recv_ln<op_num*4)
    {
        int recv_c=read(client_sock,&opinfo[recv_ln],BUF_SIZE-1);
        recv_ln+=recv_c;
    }
    int* p=(int *)opinfo; int sum=opinfo[0];

    for(int i=1;i<op_num;i++)
    {
        sum+=p[i];
    }
    write(client_sock,(char *)&sum,sizeof(sum));
    close(client_sock);
    close(serve_sock);  
    return 0;
}