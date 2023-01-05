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
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_PATH "echo_socket"

#define SHM_SIZE 1024
#define ARR_LEN 20
#define CHUNK_LEN 5
#define STR_LEN 5

char **random_str_array(size_t arr_len, size_t str_len);

int main(int argc, char const *argv[])
{
    const char *sem_p1_name = "/sem_p1";
    const char *sem_p2_name = "/sem_p2";
    sem_t *sem_p1 = sem_open(sem_p1_name, O_CREAT, 0666, 1);
    sem_t *sem_p2 = sem_open(sem_p2_name, O_CREAT, 0666, 0);

    if (!fork())
    {
        execl("bin/p2", "p2", NULL);
    }

    else
    {

        int binding_socket, con_socket, t, len;
        struct sockaddr_un local, remote;
        char *data;

        local.sun_family = AF_UNIX;
        strcpy(local.sun_path, SOCK_PATH);
        unlink(local.sun_path);
        len = strlen(local.sun_path) + sizeof(local.sun_family);

        if (bind(binding_socket, (struct sockaddr *)&local, len) == -1)
        {
            perror("bind");
            exit(1);
        }

        if (listen(binding_socket, 5) == -1)
        {
            perror("listen");
            exit(1);
        }

        for (;;)
        {
            int done, n;
            printf("Waiting for a connection...\n");

            t = sizeof(remote);
            if ((con_socket = accept(binding_socket, (struct sockaddr *)&remote, &t)) == -1)
            {
                perror("accept");
                exit(1);
            }

            printf("Connected.\n");

            done = 0;
            
            do
            {
                n = recv(con_socket, str, 100, 0);
                if (n <= 0)
                {
                    if (n < 0)
                        perror("recv");
                    done = 1;
                }

                if (!done)
                    if (send(con_socket, str, n, 0) < 0)
                    {
                        perror("send");
                        done = 1;
                    }
            } while (!done);

            close(con_socket);
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
                sprintf(str_to_write, "%binding_socket%02d\n", str_to_write, j);
            }

            sem_wait(sem_p1);
            sprintf(data, "%binding_socket", str_to_write);
            sem_post(sem_p2);

            sem_wait(sem_p1);
            printf("\nAcknowledged Index : %binding_socket\n\n", data);
            sem_post(sem_p1);
        }

        clock_gettime(CLOCK_REALTIME, &end_time);
        long seconds = end_time.tv_sec - start_time.tv_sec;
        long nanoseconds = end_time.tv_nsec - start_time.tv_nsec;
        double elapsed = seconds + nanoseconds * 1e-9;

        puts("###########################");
        printf("Time taken: %.6f seconds\n", elapsed);
        puts("###########################");
    }
    sem_destroy(sem_p1);
    sem_destroy(sem_p2);

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
