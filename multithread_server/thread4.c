#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREAD 100

void *thread_inc(void *arg);
void *thread_dec(void *arg);

long long sum = 0;

int main(void)
{
    pthread_t t_id[NUM_THREAD];

    printf("sizeof(long long): %ld\n", sizeof(long long));
    for (int i = 0; i < NUM_THREAD; ++i)
    {
        if (i & 1)
        {
            pthread_create(&t_id[i], NULL, thread_inc, NULL);
        }
        else
        {
            pthread_create(&t_id[i], NULL, thread_dec, NULL);
        }
    }
    for (int i = 0; i < NUM_THREAD; ++i)
    {
        pthread_join(t_id[i], NULL);
    }
    printf("Result: %lld\n", sum);

    return 0;
}

void *thread_inc(void *arg)
{
    for (int i = 0; i < 50000; ++i)
    {
        ++sum;
    }
    return NULL;
}

void *thread_dec(void *arg)
{
    for (int i = 0; i < 50000; ++i)
    {
        --sum;
    }
    return NULL;
}
