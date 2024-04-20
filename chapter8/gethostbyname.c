#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

void error_handling(const char *message);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <addr>\n", argv[0]);
        exit(1);
    }
    struct hostent *host = gethostbyname(argv[1]);
    if (host == NULL)
    {
        error_handling("gethostbyname() error!");
    }
    printf("Official name: %s\n", host->h_name);
    for (int i = 0; host->h_aliases[i]; ++i)
    {
        printf("Aliases %d: %s\n", i + 1, host->h_aliases[i]);
    }
    printf("Address type: %s\n", host->h_addrtype == AF_INET ? "IPv4" : "IPv6");
    printf("IP address length: %d\n", host->h_length);
    for (int i = 0; host->h_addr_list[i]; ++i)
    {
        printf("IP address %d: %s\n", i + 1, inet_ntoa(*(struct in_addr *)host->h_addr_list[i]));
    }

    return 0;
}

void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
