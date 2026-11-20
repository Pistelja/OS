#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<errno.h>
#include<string.h>


#define check_error(cond, msg)\
    do                                                                                          \
    {   \
        if(!(cond))\
        {\
            perror(msg);\
            fprintf(stderr, "File: %s\nFunc: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
            exit(EXIT_FAILURE);\
        }\
    } while (0)

#define UNUSED(x) ((void)x)

int main(int argc, char **argv)
{
    check_error(argc == 3, "./a.out -[fd] putanja");
    check_error(argv[1][0] == '-', "Los prvi argument komandne linije");
    if(argv[1][1] == 'f')
    {
        check_error(unlink(argv[2]) == 0, "Neuspelo brisanje fajla");
    }
    if(argv[1][1] == 'd')
    {   
        errno = 0;
        if(rmdir(argv[2]) != 0)
        {
            check_error(errno != ENOTDIR, "nije direktorijum");
        }
    }


}