#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include <fcntl.h>
#include <unistd.h>

#define check_error(cond,msg)\
    do                                                                                          \
    {\
        if(!(cond))\
        {\
            perror(msg);\
            fprintf(stderr, "File %s\nFunc: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
            exit(EXIT_FAILURE);\
        }\
    } while (0)

#define UNUSED(x) ((void)x)

int main(int argc, char** argv)
{
    check_error(argc == 2, "./myCat ime_fajla");
    
    int fd = open(argv[1], (O_RDWR));

    check_error(fd != -1, "open");

    char *buffer = malloc(4096);
    check_error(buffer != NULL, "malloc");

    int procitano;

    while((procitano = read(fd, buffer, 4096)) > 0)
    {
        check_error(write(STDOUT_FILENO, buffer, procitano) == procitano, "write");
    }

    free(buffer);

    close(fd);

    return 0;

}
    