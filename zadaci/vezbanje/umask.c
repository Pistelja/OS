#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>



#define check_error(cond, msg)\
    do\
    {\
        if(!(cond))\
        {\
            perror(msg);\
            fprintf(stderr, "File: %s\nLine: %d\nFunc: %s\n", __FILE__, __LINE__, __func__);\
            exit(EXIT_FAILURE);\
        }\
    } while (0)


int main(int argc, char** argv)
{
    check_error(argc == 3, "./a.out ime_fajla prava_pristupa");

    mode_t oldUmask = umask(0);

    long mode = strtol(argv[2], NULL, 8);

    int fd = open(argv[1], (O_CREAT| O_EXCL | O_WRONLY), mode);

    if(fd == -1)
    {
        if(errno == EEXIST)
        {
            printf("Fajl %s vec postoji\n", basename(argv[1]));
            check_error(chmod(argv[1], mode) != -1, "chmod");
        }
        check_error(errno == EEXIST, "open");
    }
    else
    {
        close(fd);
    }

    umask(oldUmask);

    return 0;
    
}
