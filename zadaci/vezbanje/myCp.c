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

#define BUFFER_SIZE 4096


int main(int argc, char** argv)
{
    check_error(3 == argc, "./a.out destPath srcPath");

    int fdSrc = open(argv[2], O_RDONLY);
    
    check_error(fdSrc != -1, "open source");

    char *buffer = malloc(BUFFER_SIZE);
    check_error(buffer != NULL, "malloc");

    struct stat fInfo;
    check_error(stat(argv[2], &fInfo) != -1, "stat");

    mode_t oldUmask = umask(0);

    int fdDst = open(argv[1],(O_CREAT | O_TRUNC | O_WRONLY), fInfo.st_mode);
    check_error(fdDst != -1, "open destination");


    check_error(lseek(fdSrc, 0, SEEK_SET) != -1, "lseek");
    size_t procitano;


    while((procitano = read(fdSrc, buffer, BUFFER_SIZE)) > 0)
    {
        check_error(write(fdDst, buffer, procitano) == ((ssize_t) procitano), "write");
    }
    check_error(procitano == 0, "read");

    close(fdSrc);
    close(fdDst);
    free(buffer);

    umask(oldUmask);

    return 0;

}