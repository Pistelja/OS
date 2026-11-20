#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include <fcntl.h>
#include<string.h>




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

int main(int argc, char**argv)
{
    check_error(argc == 3, "./a.out filepath number_of_bytes");
    int n = atoi(argv[2]);
    char *buffer = malloc(n + 1);
    check_error(buffer != NULL, "Neuspela alokacija");
    errno = 0;
    int fd = open(argv[1], O_RDONLY);
    if(fd == -1)
    {
        switch (errno)
        {
        case EACCES:
            printf("The requested access to the file is not allowed\n");
            exit(EXIT_FAILURE);
            break;
        case EBADF:
            printf("not valid file descriptor");
            exit(EXIT_FAILURE);
            break;
        case EDQUOT:
            printf("nzm ni ja");
            exit(EXIT_FAILURE);
            break;
        case ENOTDIR:
            printf("Nije direktorijum");
            exit(EXIT_FAILURE);
            break;    
        default:
            break;
        }
    }
    int procitano = read(fd, buffer, n);
    check_error(procitano != -1, "read");
    buffer[procitano] = '\0';
    printf("%s\n", buffer);
    free(buffer);

    close(fd);

    return 0;


}
    