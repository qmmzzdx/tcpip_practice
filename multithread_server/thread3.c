#include <stdio.h>
#include <pthread.h>

void *thread_summation(void *arg);

int sum = 0;

int main(void)
{
    pthread_t id1, id2;
    int range1[2] = {1, 5}, range2[2] = {6, 10};

    if (pthread_create(&id1, NULL, thread_summation, (void *)&range1) != 0)
    {
        puts("pthread_create() error!");
        return -1;
    }
    if (pthread_create(&id2, NULL, thread_summation, (void *)&range2) != 0)
    {
        puts("pthread_create() error!");
        return -1;
    }
    if (pthread_join(id1, NULL) != 0)
    {
        puts("pthread_join() error!");
        return -1;
    }
    if (pthread_join(id2, NULL) != 0)
    {
        puts("pthread_join() error!");
        return -1;
    }
    printf("Result: %d\n", sum);

    return 0;
}

void *thread_summation(void *arg)
{
    for (int st = ((int *)arg)[0], ed = ((int *)arg)[1]; st <= ed; ++st)
    {
        sum += st;
    }
    return NULL;
}
