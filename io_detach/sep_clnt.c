#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

void error_handling(const char *message);

int main(int argc, char *argv[])
{
    int sock;
    char buf[BUF_SIZE];
    FILE *readfp, *writefp;
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
    readfp = fdopen(sock, "r");
    writefp = fdopen(sock, "w");
    while (1)
    {
        if (fgets(buf, sizeof(buf), readfp) == NULL)
        {
            break;
        }
        printf("%s", buf);
        fflush(stdout);
    }
    fprintf(writefp, "From client: Thank you!\n");
    fflush(writefp);
    fclose(writefp);
    fclose(readfp);

    return 0;
}

void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
