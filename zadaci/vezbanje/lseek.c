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


int main(int argc, char** argv)
{
    check_error(argc == 3, "./a.out ime_fajla broj_bajtova");

    int fd = open(argv[1], O_RDONLY);
    check_error(fd != -1, "open");
    
    off_t n = atoi(argv[2]);
    off_t nasao = lseek(fd, -n, SEEK_END);
    check_error(nasao != -1, "lseek");

    char *buffer = malloc(n + 1);
    check_error(buffer != NULL, "malloc");

    int procitano = read(fd, buffer, n);
    check_error(procitano == n, "read");
    buffer[procitano] = '\0';

    int ispisano = write(STDOUT_FILENO, buffer, procitano);
    check_error(ispisano == n, "write");

    free(buffer);
    close(fd);
    
    return 0;

}