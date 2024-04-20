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
    int send_sock, so_brd = 1;
    char buf[BUF_SIZE];
    struct sockaddr_in broad_addr;

    if (argc != 3)
    {
        printf("Usage: %s <Broadcast IP> <Port>\n", argv[0]);
        exit(1);
    }
    if ((send_sock = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
    {
        error_handling("socket() error!");
    }
    memset(&broad_addr, 0, sizeof(broad_addr));
    broad_addr.sin_family = AF_INET;
    broad_addr.sin_addr.s_addr = inet_addr(argv[1]);
    broad_addr.sin_port = htons(atoi(argv[2]));

    setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST, (void *)&so_brd, sizeof(so_brd));
    if ((fp = fopen("news.txt", "r")) == NULL)
    {
        error_handling("fopen() error!");
    }
    while (!feof(fp))
    {
        fgets(buf, BUF_SIZE, fp);
        sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr *)&broad_addr, sizeof(broad_addr));
        sleep(2);
    }
    fclose(fp);
    close(send_sock);

    return 0;
}

void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
