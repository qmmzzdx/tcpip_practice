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
    int recv_sock, str_len;
    char buf[BUF_SIZE];
    struct sockaddr_in addr;

    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    if ((recv_sock = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
    {
        error_handling("UDP socket creation error!");
    }
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));

    if (bind(recv_sock, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        error_handling("bind() error!");
    }
    while (1)
    {
        if ((str_len = recvfrom(recv_sock, buf, BUF_SIZE - 1, 0, NULL, 0)) == -1)
        {
            break;
        }
        buf[str_len] = '\0';
        printf("%s\n", buf);
    }
    close(recv_sock);

    return 0;
}

void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
