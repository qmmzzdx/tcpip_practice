#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
    FILE *readfp, *writefp;
    int serv_sock, clnt_sock;
    socklen_t clnt_addr_size;
    struct sockaddr_in serv_addr, clnt_addr;
    char buf[BUF_SIZE];

    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if ((serv_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "socket() error!\n");
        exit(EXIT_FAILURE);
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    memset(buf, 0, sizeof(buf));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        fprintf(stderr, "bind() error!\n");
        exit(EXIT_FAILURE);
    }
    if (listen(serv_sock, 5) == -1)
    {
        fprintf(stderr, "listen() error!\n");
        exit(EXIT_FAILURE);
    }
    if ((clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size)) == -1)
    {
        fprintf(stderr, "accept() error!\n");
        exit(EXIT_FAILURE);
    }

    readfp = fdopen(clnt_sock, "r");
    writefp = fdopen(dup(clnt_sock), "w");
    fprintf(writefp, "From server: Hi, client?\n");
    fprintf(writefp, "I love all of the world\n");
    fprintf(writefp, "You are awesome!\n");
    fflush(writefp);

    shutdown(fileno(writefp), SHUT_WR);
    fclose(writefp);
    fgets(buf, sizeof(buf), readfp);
    printf("%s", buf);
    fclose(readfp);

    return 0;
}
