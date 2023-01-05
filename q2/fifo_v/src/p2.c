#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFO_NAME "bin/fifo"
#define ARR_LEN 50
#define CHUNK_LEN 5
#define STR_LEN 10

int main(int argc, char const *argv[])
{
    mknod(FIFO_NAME, S_IFIFO | 0666, 0);
    int fd;
    
    char *str_to_read = NULL;
    char *str_to_write = NULL;

    int read_size = sizeof(char) * ((STR_LEN + 1) + 3) * CHUNK_LEN;
    int write_size = sizeof(char) * 2;

    for (int i = 0; i < ARR_LEN; i += CHUNK_LEN)
    {
        fd = open(FIFO_NAME, O_RDONLY);

        str_to_read = malloc(read_size);
        int status;

        if ((status = read(fd, str_to_read, read_size)) == -1)
            perror("read");

        else {
            puts("****************");
            printf("Recieved Chunk:\n\n%s", str_to_read);
            puts("****************\n");
        }

        close(fd);

        fd = open(FIFO_NAME, O_WRONLY);

        str_to_write = malloc(write_size);
        str_to_write[0] = str_to_read[read_size-3];
        str_to_write[1] = str_to_read[read_size-2];
        
        write(fd, str_to_write, write_size);
        close(fd);
    }

    return 0;
}