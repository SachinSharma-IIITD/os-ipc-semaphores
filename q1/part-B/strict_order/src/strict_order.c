#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILO 5
#define NUM_BOWLS 2

void *dine_philo(void *idx);
void eating(int indices);

int main()
{
    pthread_t pthread[NUM_PHILO];

    for (int i = 0; i < NUM_PHILO / 2; i++)
    {
        //int *idx = malloc(sizeof(int));
	int a, b, c, d;
	a = i;
	b = i+2;
        //*idx = i;
        //printf("IDX : %d\n", a);
        pthread_create(&pthread[a], NULL, &dine_philo, (void *)&a);

        //*idx = i + 2;
        //printf("IDX : %d\n", b);
        pthread_create(&pthread[b], NULL, &dine_philo, (void *)&b);

        //*idx = i;
        pthread_join(pthread[a], NULL);

        //*idx = i + 2;
        pthread_join(pthread[b], NULL);
    }

    int i = 4;
    //printf("IDX : %d\n", i);
    pthread_create(&pthread[4], NULL, &dine_philo, (void *)&i);
    pthread_join(pthread[4], NULL);
}

void *dine_philo(void *idx)
{
    int num = *(int *)idx;
    //printf("NUM : %d\n", num);
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
