#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>

#define SHM_SIZE 1024
#define ARR_LEN 50
#define CHUNK_LEN 5
#define STR_LEN 5

int max_index(int *indices, int len);

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

    int read_size = sizeof(char) * ((STR_LEN + 1) + 3) * CHUNK_LEN;
    char *str_to_read = malloc(read_size);

    int *indices = malloc(sizeof(int) * CHUNK_LEN);
    char *temp = malloc(sizeof(char) * 2);

    for (int i = 0; i < ARR_LEN; i += CHUNK_LEN)
    {
        sem_wait(sem_p2);

        puts("**********");
        puts("Recieved Chunk : \n");

        strcpy(str_to_read, data);
        printf("%s", str_to_read);
        puts("**********");

        int x = 0;
        for (int j = 0; j < CHUNK_LEN; j++)
        {
            x += STR_LEN + 1;
            temp[0] = str_to_read[x];
            temp[1] = str_to_read[x + 1];
            x += 3;

            indices[j] = atoi(temp);
        }

        int max_idx = max_index(indices, CHUNK_LEN);
        sprintf(data, "%d", max_idx);

        sem_post(sem_p1);
    }

    // shmdt((const void *)data);
    return 0;
}

int max_index(int *indices, int len)
{
    int max = indices[0];

    for (int i = 1; i < len; i++)
    {
        if (max < indices[i])
            max = indices[i];
    }
    return max;
}