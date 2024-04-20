#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

void error_handling(const char *message);

int main(void)
{
    const char *addr = "1.2.3.4";
    struct sockaddr_in addr_inet;

    if (inet_aton(addr, &addr_inet.sin_addr))
    {
        printf("Network ordered integer addr: %#x\n", addr_inet.sin_addr.s_addr);
    }
    else
    {
        error_handling("Conversion error!");
    }

    return 0;
}

void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

