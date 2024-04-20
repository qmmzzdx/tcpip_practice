#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void read_childproc(int sig)
{
    int status = 0;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status))
    {
        printf("Removed process id: %d\n", id);
        printf("Child send: %d\n", WEXITSTATUS(status));
    }
}

int main(void)
{
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (sigaction(SIGCHLD, &act, NULL) == -1)
    {
        printf("sigaction() error!\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == 0)
    {
        puts("Hi! I'm child process.");
        sleep(10);
        return 12;
    }
    else
    {
        printf("Child process id: %d\n", pid);
        pid = fork();
        if (pid == 0)
        {
            puts("Hi! I'm child process.");
            sleep(10);
            exit(24);
        }
        else
        {
            printf("Child process id: %d\n", pid);
            for (int i = 0; i < 5; ++i)
            {
                puts("wait...");
                sleep(5);
            }
        }
    }

    return 0;
}
