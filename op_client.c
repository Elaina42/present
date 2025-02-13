#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
int main(int argc,char* argv[])
{
    int client_sock=socket(PF_INET,SOCK_STREAM,0);
    if(client_sock==-1)fputs("ERROR",stdout);
    struct sockaddr_in serv_adr;
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));
    if(connect(client_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)fputs("connect error",stdout);
    int opnd_cnt;
    fputs("Operand num: ",stdout);
    scanf("%d",&opnd_cnt);
    char opmsg[BUF_SIZE];
    opmsg[0]=(char)opnd_cnt;
    for (int i = 0; i < opnd_cnt; i++) {
        printf("Operand %d: ", i + 1);

        // 检查输入是否成功
        if (scanf("%d", (int*)&opmsg[i * 4 + 1]) != 1) {
            fprintf(stderr, "Invalid input. Please enter an integer.\n");
            return -1; // 错误处理
        }
    }
    int result;
    write(client_sock,opmsg,opnd_cnt*4+1);
    read(client_sock,&result,4);
    printf("result :%d\n",result);
    close(client_sock);
    return 0;
}