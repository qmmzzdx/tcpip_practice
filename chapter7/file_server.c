#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 30

void error_handling(const char *message);

int main(int argc, char *argv[])
{
    FILE *fp;
    char buf[BUF_SIZE];
    int serv_sock, clnt_sock, read_cnt;
    socklen_t clnt_addr_size;
    struct sockaddr_in serv_addr, clnt_addr;

    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    if ((fp = fopen("file_server.c", "rb")) == NULL)
    {
        error_handling("fopen() error!");
    }
    if ((serv_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        error_handling("socket() error!");
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        error_handling("bind() error!");
    }
    if (listen(serv_sock, 5) == -1)
    {
        error_handling("listen() error!");
    }
    clnt_addr_size = sizeof(clnt_addr);
    if ((clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size)) == -1)
    {
        error_handling("accept() error!");
    }
    while (1)
    {
        if ((read_cnt = fread((void *)buf, 1, BUF_SIZE, fp)) < BUF_SIZE)
        {
            write(clnt_sock, buf, read_cnt);
            break;
        }
        write(clnt_sock, buf, BUF_SIZE);
    }
    shutdown(clnt_sock, SHUT_WR);
    read(clnt_sock, buf, BUF_SIZE);
    printf("Message from client: %s\n", buf);

    fclose(fp);
    close(clnt_sock);
    close(serv_sock);

    return 0;
}

void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
