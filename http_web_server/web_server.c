#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
#define SMALL_BUF 100

void *request_handler(void *arg);
void send_data(FILE *fp, const char *ct, const char *file_name);
const char *content_type(const char *file);
void send_error(FILE *fp);
void error_handling(const char *message);

int main(int argc, char *argv[])
{
    pthread_t t_id;
    char buf[BUF_SIZE];
    int clnt_addr_size, serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;

    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
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
    if (listen(serv_sock, 20) == -1)
    {
        error_handling("listen() error!");
    }

    while (1)
    {
        clnt_addr_size = sizeof(clnt_addr);
        if ((clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size)) == -1)
        {
            error_handling("accept() error!");
        }
        printf("Connection request: %s:%d\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
        pthread_create(&t_id, NULL, request_handler, &clnt_sock);
        pthread_detach(t_id);
    }
    close(serv_sock);

    return 0;
}

void *request_handler(void *arg)
{
    int clnt_sock = *((int *)arg);
    FILE *clnt_read, *clnt_write;
    char ct[15], method[10], file_name[30], req_line[SMALL_BUF];

    clnt_read = fdopen(clnt_sock, "r");
    clnt_write = fdopen(dup(clnt_sock), "w");
    fgets(req_line, SMALL_BUF, clnt_read);
    if (strstr(req_line, "HTTP/") == NULL)
    {
        send_error(clnt_write);
        fclose(clnt_read), fclose(clnt_write);
        return NULL;
    }
    strcpy(method, strtok(req_line, " /"));
    strcpy(file_name, strtok(NULL, " /"));
    strcpy(ct, content_type(file_name));
    if (strcmp(method, "GET") != 0)
    {
        send_error(clnt_write);
        fclose(clnt_read), fclose(clnt_write);
        return NULL;
    }
    fclose(clnt_read);
    send_data(clnt_write, ct, file_name);
    return NULL;
}

void send_data(FILE *fp, const char *ct, const char *file_name)
{
    FILE *send_file;
    const char *protocol = "HTTP/1.0 200 OK\r\n";
    const char *server = "Server:Linux Web Server \r\n";
    const char *cnt_len = "Content-length:2048\r\n";
    char buf[BUF_SIZE], cnt_type[SMALL_BUF];

    sprintf(cnt_type, "Content-type:%s\r\n\r\n", ct);
    if ((send_file = fopen(file_name, "r")) == NULL)
    {
        send_error(fp);
        return;
    }
    fputs(protocol, fp), fputs(server, fp);
    fputs(cnt_len, fp), fputs(cnt_type, fp);
    while (fgets(buf, BUF_SIZE, send_file) != NULL)
    {
        fputs(buf, fp);
        fflush(fp);
    }
    fflush(fp);
    fclose(fp);
}

const char *content_type(const char *file)
{
    char extension[SMALL_BUF], file_name[SMALL_BUF];

    strcpy(file_name, file);
    strtok(file_name, ".");
    strcpy(extension, strtok(NULL, "."));
    return strcmp(extension, "html") || strcmp(extension, "htm") ? "text/plain" : "text/html";
}

void send_error(FILE *fp)
{
    const char *protocol = "HTTP/1.0 400 Bad Request\r\n";
    const char *server = "Server:Linux Web Server \r\n";
    const char *cnt_len = "Content-type:2048\r\n";
    const char *cnt_type = "Content-type:text/html\r\n\r\n";
    const char *content = "<html><head><title>NETWORK</title></head>"
        "<body><font size=+5><br>发生错误! 查看请求文件名和请求方式!"
        "</font></body></html>";

    fputs(protocol, fp), fputs(server, fp);
    fputs(cnt_len, fp), fputs(cnt_type, fp);
    fflush(fp);
}

void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
