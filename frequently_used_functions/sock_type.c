#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

void error_handling(const char *message);

int main(int argc, char *argv[])
{
    socklen_t optlen;
    int state, sock_type, tcp_sock, udp_sock;

    optlen = sizeof(sock_type);
    tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
    udp_sock = socket(PF_INET, SOCK_DGRAM, 0);
    printf("SOCK_STREAM: %d\n", SOCK_STREAM);
    printf("SOCK_DRAM: %d\n", SOCK_DGRAM);

    if ((state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (void *)&sock_type, &optlen)) == -1)
    {
        error_handling("getsockopt() error!");
    }
    else
    {
        printf("Socket type one: %d\n", sock_type);
    }
    if ((state = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (void *)&sock_type, &optlen)) == -1)
    {
        error_handling("getsockopt() error!");
    }
    else
    {
        printf("Socket type two: %d\n", sock_type);
    }

    return 0;
}

void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

