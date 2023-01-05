#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <wait.h>
#include <time.h>

#define FIFO_NAME "bin/fifo"
#define ARR_LEN 50
#define CHUNK_LEN 5
#define STR_LEN 10

char **random_str_array(size_t arr_len, size_t str_len);

int main(int argc, char const *argv[])
{
    char **arr;
    char *str_to_write = NULL;
    char *str_to_read = NULL;
    int write_size = sizeof(char) * ((STR_LEN + 1) + 3) * CHUNK_LEN;
    int read_size = sizeof(char) * 2;

    int fd;
    int status;

    arr = random_str_array(ARR_LEN, STR_LEN);
    mknod(FIFO_NAME, S_IFIFO | 0666, 0);

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_REALTIME, &start_time);

    if (arr)
    {
        for (int i = 0; i < ARR_LEN; i += CHUNK_LEN)
        {
            fd = open(FIFO_NAME, O_WRONLY);

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

            if (str_to_write)
                if ((status = write(fd, str_to_write, write_size)) == -1)
                    perror("write");

            close(fd);

            fd = open(FIFO_NAME, O_RDONLY);

            str_to_read = malloc(read_size);
            if ((read(fd, str_to_read, read_size)) == -1)
                perror("read");
            else
            {
                printf("Acknowledged ID: %s\n\n", str_to_read);
            }
            close(fd);
        }

        clock_gettime(CLOCK_REALTIME, &end_time);
        long seconds = end_time.tv_sec - start_time.tv_sec;
        long nanoseconds = end_time.tv_nsec - start_time.tv_nsec;
        double elapsed = seconds + nanoseconds * 1e-9;

        puts("###########################");
        printf("Time taken: %.6f seconds\n", elapsed);
        puts("###########################");
    }
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
