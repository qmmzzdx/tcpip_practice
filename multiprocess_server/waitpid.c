#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int status = 0;
    pid_t pid = fork();

    if (pid == 0)
    {
        sleep(15);
        return 24;
    }
    else
    {
        while (waitpid(-1, &status, WNOHANG) != -1)
        {
            sleep(3);
            puts("sleep 3seconds.");
        }
        if (WIFEXITED(status))
        {
            printf("Child send %d.\n", WEXITSTATUS(status));
        }
    }

    return 0;
}
