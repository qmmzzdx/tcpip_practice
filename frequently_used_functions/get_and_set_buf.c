#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

void error_handling(const char *message);

int main(int argc, char *argv[])
{
    socklen_t len;
    int sock, snd_buf, rcv_buf, state;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    len = sizeof(snd_buf);
    if ((state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void *)&snd_buf, &len)) == -1)
    {
        error_handling("getsockopt() error!");
    }
    len = sizeof(rcv_buf);
    if ((state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void *)&rcv_buf, &len)) == -1)
    {
        error_handling("getsockopt() error!");
    }
    printf("Initial buffer size:\n");
    printf("Input buffer size: %d\n", rcv_buf);
    printf("Output buffer size: %d\n", snd_buf);

    snd_buf = rcv_buf = 1024 * 3;
    len = sizeof(snd_buf);
    if ((state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void *)&snd_buf, len)) == -1)
    {
        error_handling("setsockopt() error!");
    }
    len = sizeof(rcv_buf);
    if ((state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void *)&rcv_buf, len)) == -1)
    {
        error_handling("setsockopt() error!");
    }
    len = sizeof(snd_buf);
    if ((state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void *)&snd_buf, &len)) == -1)
    {
        error_handling("getsockopt() error!");
    }
    len = sizeof(rcv_buf);
    if ((state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void *)&rcv_buf, &len)) == -1)
    {
        error_handling("getsockopt() error!");
    }
    printf("Call setsockopt function:\n");
    printf("Input buffer size: %d\n", rcv_buf);
    printf("Output buffer size: %d\n", snd_buf);

    return 0;
}

void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

