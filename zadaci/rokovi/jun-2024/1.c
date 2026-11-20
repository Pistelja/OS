#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <pwd.h>
#include <ftw.h>
#include <string.h>

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

#define UNUSED(x) ((void)x)


int main(int argc, char** argv)
{
    check_error(argc == 4, "./a.out putanja_do_fajla a b");

    int fd = open(argv[1], O_RDWR);
    check_error(fd != -1, "open");

    int a = atoi(argv[2]);
    int b = atoi(argv[3]);

    struct stat fInfo;
    check_error(stat(argv[1], &fInfo) != -1, "stat");

    size_t count = b - a + 1;

    char *buffer1 = malloc(count);
    check_error(buffer1 != NULL, "malloc");

    check_error(lseek(fd, a, SEEK_SET)!=-1, "lseek");
    int procitano = read(fd, buffer1, count);
    check_error(procitano != -1, "read");

    check_error(write(STDOUT_FILENO,buffer1, procitano) == procitano, "write");

    printf("\n%d\n", procitano);

    close(fd);
    free(buffer1);
    return 0;

}