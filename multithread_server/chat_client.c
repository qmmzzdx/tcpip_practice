#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 120

void *send_msg(void *arg);
void *recv_msg(void *arg);
void error_handling(const char *msg);

char msg[BUF_SIZE], name[BUF_SIZE] = "[DEFAULT]";

int main(int argc, char *argv[])
{
    int sock;
    void *thread_return;
    pthread_t send_thread, recv_thread;
    struct sockaddr_in serv_addr;

    if (argc != 4)
    {
        printf("Usage: %s <ip> <port> <name>\n", argv[0]);
	    exit(1);
    }
    sprintf(name, "[%s]", argv[3]);
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

    pthread_create(&send_thread, NULL, send_msg, (void *)&sock);
    pthread_create(&recv_thread, NULL, recv_msg, (void *)&sock);
    pthread_join(send_thread, &thread_return);
    pthread_join(recv_thread, &thread_return);
    close(sock);

    return 0;
}

void *send_msg(void *arg)
{
    int sock = *((int *)arg);
    char name_msg[BUF_SIZE + BUF_SIZE];

    while (1)
    {
        fgets(msg, BUF_SIZE, stdin);
        if (strcmp(msg, "q\n") == 0 || strcmp(msg, "Q\n") == 0)
        {
            close(sock);
            exit(0);
        }
        sprintf(name_msg, "%s %s", name, msg);
        write(sock, name_msg, strlen(name_msg));
    }
    return NULL;
}

void *recv_msg(void *arg)
{
    char name_msg[BUF_SIZE];
    int str_len, sock = *((int *)arg);

    while (1)
    {
        if ((str_len = read(sock, name_msg, BUF_SIZE - 1)) == -1)
        {
            return (void *)-1;
        }
        name_msg[str_len] = '\0';
        printf("%s", name_msg);
    }
    return NULL;
}

void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

