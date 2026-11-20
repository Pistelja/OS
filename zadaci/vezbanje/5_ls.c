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
    check_error(argc == 1, "./a.out");

    UNUSED(argv);

    struct dirent *d = NULL;

    DIR *dir_stream = opendir(".");
    check_error(dir_stream != NULL, "opendir");

    errno = 0;
    while((d = readdir(dir_stream)) != NULL)
    {
        if(strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0)
        {
            printf("%s\n", d->d_name);
        }
        errno = 0;
    }
    check_error(errno == 0, "readir");

    closedir(dir_stream);

    return 0;

}
