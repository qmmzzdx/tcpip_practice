#include <stdio.h>
#include <arpa/inet.h>

void dot_convert_network_orders(const char *addr);

int main(void)
{
    const char *addr1 = "1.2.3.4";
    const char *addr2 = "1.2.3.256";

    dot_convert_network_orders(addr1);
    dot_convert_network_orders(addr2);

    return 0;
}

void dot_convert_network_orders(const char *addr)
{
    unsigned long conv_addr = inet_addr(addr);
    if (conv_addr == INADDR_NONE)
    {
        printf("Error occured!\n");
    }
    else
    {
        printf("Network ordered integer addr: %#lx\n", conv_addr);
    }
}

