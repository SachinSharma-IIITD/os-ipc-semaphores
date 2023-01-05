#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILO 5

sem_t sems[NUM_PHILO];

void *dine_philo(void *idx);
void eating(int indices);

int main()
{
    pthread_t pthread[NUM_PHILO];

    for (int i = 0; i < NUM_PHILO; i++)
    {
        sem_t sem_temp;
        sem_init(&sem_temp, 0, 1);
        sems[i] = sem_temp;
    }

    for (int i = 0; i < NUM_PHILO; i++)
    {
        int *idx = malloc(sizeof(int));
        *idx = i;
        pthread_create(&pthread[i], NULL, &dine_philo, (void *)idx);
    }

    for (int i = 0; i < NUM_PHILO; i++)
    {
        pthread_join(pthread[i], NULL);
    }
    return 0;
}

void *dine_philo(void *idx)
{
    int num = *(int *)idx;
    printf("-- Thinking : Philosopher %d \n", num);

    if (num == 4)
    {
        sem_wait(&sems[0]);
        sem_wait(&sems[4]);
    }
    else
    {
        sem_wait(&sems[num]);
        sem_wait(&sems[(num + 1) % NUM_PHILO]);
    }

    eating(num);

    sem_post(&sems[num]);
    sem_post(&sems[(num + 1) % NUM_PHILO]);

    pthread_exit(NULL);
}

void eating(int idx)
{
    printf("\n# Eating : Philosopher %d \n\n", idx);
    sleep(1);
    printf("\n# Finished eating : Philosopher %d \n\n", idx);
    return;
}