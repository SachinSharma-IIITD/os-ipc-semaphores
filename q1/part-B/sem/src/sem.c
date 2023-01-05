#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <wait.h>

#define NUM_FORKS 5
#define NUM_BOWLS 2

sem_t sem_forks[NUM_FORKS];
sem_t sem_bowls[NUM_BOWLS];
int bowls_taken[NUM_FORKS];

void *dine_philo(void *idx);
void eating(int indices);
void *get_bowl(void *indices);

int main()
{
    pthread_t threads[NUM_FORKS];

    for (int i = 0; i < NUM_FORKS; i++)
    {
        sem_t sem_temp;
        sem_init(&sem_temp, 0, 1);
        sem_forks[i] = sem_temp;
    }

    for (int i = 0; i < NUM_BOWLS; i++)
    {
        sem_t sem_temp;
        sem_init(&sem_temp, 0, 1);
        sem_bowls[i] = sem_temp;
    }

    for (int i = 0; i < NUM_FORKS; i++)
    {
        int *idx = malloc(sizeof(int));
        *idx = i;
        pthread_create(&threads[i], NULL, &dine_philo, (void *)idx);
    }

    for (int i = 0; i < NUM_FORKS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    return 0;
}

void *dine_philo(void *idx)
{
    int num = *(int *)idx;
    printf("-- Thinking : Philosopher %d \n", num);

    if (num == 4)
    {
        sem_wait(&sem_forks[0]);
        sem_wait(&sem_forks[4]);
    }
    else
    {
        sem_wait(&sem_forks[num]);
        sem_wait(&sem_forks[(num + 1) % NUM_FORKS]);
    }

    pthread_t bowl_threads[2];
    int indices[2];
    indices[0] = num;

    indices[1] = 0;
    pthread_create(&bowl_threads[0], NULL, &get_bowl, (void *)indices);
    indices[1] = 1;
    pthread_create(&bowl_threads[0], NULL, &get_bowl, (void *)indices);

    wait(NULL);

    eating(num);

    sem_post(&sem_forks[num]);
    sem_post(&sem_forks[(num + 1) % NUM_FORKS]);
    sem_post(&sem_bowls[bowls_taken[num]]);

    pthread_exit(NULL);
}

void *get_bowl(void *indices)
{
    int p_num = (int)((int *)indices)[0];
    int b_num = (int)((int *)indices)[1];

    sem_wait(&sem_bowls[b_num]);
    bowls_taken[p_num] = b_num;

    pthread_exit(NULL);
}

void eating(int idx)
{
    printf("\n# Eating : Philosopher %d \n\n", idx);
    sleep(1);
    printf("\n# Finished eating : Philosopher %d \n\n", idx);
    return;
}