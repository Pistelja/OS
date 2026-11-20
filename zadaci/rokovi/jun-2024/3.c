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
#include <time.h>

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
    check_error(argc == 1,"./a.out");
    char *path = getenv("PATH");
    check_error(path != NULL, "getenv");
    char *s = NULL;
    char *pathKopija = strdup(path);
    s = strtok(pathKopija,":");
    while(s != NULL)
    {
        if(s[0] != '\0')
        { 
        struct stat fileInfo;
        mode_t maska = S_IRUSR | S_IRGRP | S_IROTH;
        if(stat(s,&fileInfo) != -1 && S_ISDIR(fileInfo.st_mode))
        {
        if((maska & fileInfo.st_mode) == maska)
        {
            char *name = basename(s);
            printf("%s\n", name);
        }
        }
        }
        s = strtok(NULL, ":");
    }


    free(pathKopija);

    return 0;

}