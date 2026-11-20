#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include <unistd.h>
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
    check_error(argc == 3, "./write_bytes ime_fajla string");
    int fd = open(argv[1], (O_WRONLY | O_CREAT | O_TRUNC),(S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH));
    check_error(fd != -1, "open");

    int zapisano = write(fd,argv[2], strlen(argv[2]));
    check_error(zapisano == (int)(strlen(argv[2])), "write");

    close(fd);
    return 0;

    

}