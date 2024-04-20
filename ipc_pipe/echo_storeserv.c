#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/socket.h>
#define BUF_SIZE 100

void read_childproc(int sig);
void error_handling(const char *message);

int main(int argc, char *argv[])
{
    pid_t pid;
    int fds[2];
    char buf[BUF_SIZE];
    socklen_t clnt_addr_size;
    int str_len, serv_sock, clnt_sock;
    struct sigaction act;
    struct sockaddr_in serv_addr, clnt_addr;

    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (sigaction(SIGCHLD, &act, NULL) == -1)
    {
        error_handling("sigaction() error!");
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

    pipe(fds);
    pid = fork();
    if (pid == 0)
    {
        int len = 0;
        FILE *fp = fopen("echomsg.txt", "wt");
        char msgbuf[BUF_SIZE];
        for (int i = 0; i < 10; ++i)
        {
            len = read(fds[0], msgbuf, BUF_SIZE);
            fwrite((void *)msgbuf, 1, len, fp);
        }
        fclose(fp);
        return 0;
    }
    while (1)
    {
        clnt_addr_size = sizeof(clnt_addr);
        if ((clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size)) == -1)
        {
            continue;
        }
        else
        {
            puts("New client connected...");
        }
        pid = fork();
        if (pid == 0)
        {
            close(serv_sock);
            while (str_len = read(clnt_sock, buf, BUF_SIZE))
            {
                write(clnt_sock, buf, str_len);
                write(fds[1], buf, str_len);
            }
            close(clnt_sock);
            puts("Client disconnected...");
            return 0;
        }
        else
        {
            close(clnt_sock);
        }
    }
    close(serv_sock);

    return 0;
}

void read_childproc(int sig)
{
    int status = 0;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    printf("Removed process id: %d\n", pid);
}

void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
