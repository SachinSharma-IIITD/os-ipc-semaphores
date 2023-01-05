#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define SHM_SIZE 1024
#define ARR_LEN 50
#define CHUNK_LEN 5
#define STR_LEN 5

char **random_str_array(size_t arr_len, size_t str_len);

int main(int argc, char const *argv[])
{
    const char *sem_p1_name = "/sem_p1";
    const char *sem_p2_name = "/sem_p2";
    sem_t *sem_p1 = sem_open(sem_p1_name, O_CREAT, 0666, 1);
    sem_t *sem_p2 = sem_open(sem_p2_name, O_CREAT, 0666, 0);

    key_t key;
    int shmid;
    char *data;

    if ((key = ftok("Makefile", 'S')) == -1)
    {
        perror("ftok");
        return 1;
    }

    if ((shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT)) == -1)
    {
        perror("shmget");
        return 1;
    }

    if ((data = shmat(shmid, NULL, 0)) == (char *)-1)
    {
        perror("shmat");
        return 1;
    }

    char **arr = random_str_array(ARR_LEN, STR_LEN);
    char *str_to_write = NULL;
    int write_size = sizeof(char) * ((STR_LEN + 1) + 3) * CHUNK_LEN;

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_REALTIME, &start_time);

    for (int i = 0; i < ARR_LEN; i += CHUNK_LEN)
    {
        str_to_write = NULL;

        int j;
        for (j = i; j < i + CHUNK_LEN; j++)
        {
            if (!str_to_write)
            {
                str_to_write = malloc(write_size);
                strcpy(str_to_write, arr[j]);
                strcat(str_to_write, "\n");
            }
            else
            {
                strcat(str_to_write, arr[j]);
                strcat(str_to_write, "\n");
            }
            sprintf(str_to_write, "%s%02d\n", str_to_write, j);
        }

        sem_wait(sem_p1);
        sprintf(data, "%s", str_to_write);
        sem_post(sem_p2);

        sem_wait(sem_p1);
        printf("\nAcknowledged Index : %s\n\n", data);
        sem_post(sem_p1);
    }

    clock_gettime(CLOCK_REALTIME, &end_time);
    long seconds = end_time.tv_sec - start_time.tv_sec;
    long nanoseconds = end_time.tv_nsec - start_time.tv_nsec;
    double elapsed = seconds + nanoseconds * 1e-9;

    puts("###########################");
    printf("Time taken: %.6f seconds\n", elapsed);
    puts("###########################");

    // shmdt((const void *)data);
    // shmctl(shmid, IPC_RMID, NULL);

    // sem_destroy(sem_p1);
    // sem_destroy(sem_p2);

    return 0;
}

char **random_str_array(size_t arr_len, size_t str_len)
{
    char **arr = NULL;
    char *str = NULL;

    if (arr_len && str_len)
    {
        arr = malloc(sizeof(char *) * (arr_len + 1));

        for (int i = 0; i < arr_len; i++)
        {
            str = malloc(sizeof(char) * (str_len + 1));

            for (int j = 0; j < str_len; j++)
            {
                // char : 33-126
                str[j] = 33 + (rand() % 94);
            }
            str[str_len] = '\0';

            arr[i] = str;
        }
    }
    return arr;
}
