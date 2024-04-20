#include <stdio.h>
#include <unistd.h>

int gval = 10;

int main(void)
{
    pid_t pid;
    int lval = 20;

    ++gval, lval += 5;
    pid = fork();
    if (pid == 0)
    {
        gval += 2, lval += 2;
        printf("Child process:\n");
    }
    else
    {
        gval -= 2, lval -= 2;
        printf("Parent process:\n");
    }
    printf("gval = %d, lval = %d\n", gval, lval);
    
    return 0;
}
