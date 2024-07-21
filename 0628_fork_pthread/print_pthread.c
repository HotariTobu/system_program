#include <stdio.h>
#include <pthread.h>

void *callback(void *arg)
{
    printf(arg);
}

int main()
{
    pthread_t t;

    printf("Hello_World!\n");
    int tr = pthread_create(&t, NULL, callback, "Hi!\n");
    if (tr == 0)
    {
        printf("Succeed in creating a thread!\n");
    }
    else
    {
        printf("Failed to creating a thread...\n");
    }
}
