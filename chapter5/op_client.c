#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4

void error_handling(const char *message);

int main(int argc, char *argv[])
{
    int sock, result, opnd_cnt;
    char opmsg[BUF_SIZE];
    struct sockaddr_in serv_addr;

    if (argc != 3)
    {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        error_handling("socket() error!");
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        error_handling("connect() error!");
    }
    else
    {
        puts("Connected...........");
    }
    
    printf("Operand count: ");
    scanf("%d", &opnd_cnt);
    opmsg[0] = (char)opnd_cnt;
    for (int i = 0; i < opnd_cnt; ++i)
    {
        printf("Operand %d: ", i + 1);
        scanf("%d", (int *)&opmsg[i * OPSZ + 1]);
    }
    getchar();
    printf("Operator: ");
    scanf("%c", &opmsg[opnd_cnt * OPSZ + 1]);
    
    write(sock, opmsg, opnd_cnt * OPSZ + 2);
    read(sock, &result, RLT_SIZE);
    printf("Operation result: %d\n", result);
    close(sock);

    return 0;
}

void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
