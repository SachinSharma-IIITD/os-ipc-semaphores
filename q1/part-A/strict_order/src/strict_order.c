#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILO 5

void *dine_philo(void *idx);
void eating(int indices);

int main()
{
    pthread_t pthread[NUM_PHILO];

    for (int i = 0; i < NUM_PHILO / 2; i++)
    {
        int *idx = malloc(sizeof(int));
        *idx = i;

        while (*idx >= 0 && *idx < NUM_PHILO)
        {
            printf("IDX : %d\n", *idx);
            pthread_create(&pthread[*idx], NULL, &dine_philo, (void *)idx);
            *idx += 2;
        }

        *idx = i;

        while (*idx >= 0 && *idx < NUM_PHILO)
        {
            pthread_join(pthread[*idx], NULL);
            *idx += 2;
        }
    }
}

void *dine_philo(void *idx)
{
    int num = *(int *)idx;
    printf("NUM : %d\n", num);
    printf("-- Thinking : Philosopher %d \n", num);

    eating(num);

    pthread_exit(NULL);
}

void eating(int idx)
{
    printf("\n# Eating : Philosopher %d \n", idx);
    sleep(1);
    printf("\n# Finished eating : Philosopher %d \n\n", idx);
    return;
}