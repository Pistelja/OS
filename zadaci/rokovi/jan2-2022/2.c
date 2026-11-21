#define _XOPEN_SOURCE 700
#define _DEFAULT_SOURCE
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
#include <limits.h>
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
    check_error(argc == 2, "./a.out putanja_do_direktorijuma");
    errno = 0;
    DIR *d = opendir(argv[1]);
    if(d == NULL)
    {
        check_error(errno != EACCES, "Permission denied");
        check_error(errno == EACCES, "opendir");
    }
    struct dirent *dir = readdir(d);
    errno = 0;
    time_t max_time = 0;
    char *resenje = NULL;
    while(dir != NULL)
    {   
        struct stat fInfo;
        char full_path[4096];
        snprintf(full_path, sizeof(full_path), "%s/%s", argv[1], dir->d_name);
        check_error(lstat(full_path, &fInfo)!= -1, "stat");
        if(S_ISLNK(fInfo.st_mode))
        {   
            time_t modifikovano = fInfo.st_mtime;
            if(modifikovano > max_time)
            {
                max_time = modifikovano;
                free(resenje);
                resenje = strdup(dir->d_name);
            }
        }
        errno = 0;
        dir = readdir(d);
    }
    check_error(errno == 0, "readdir");

    if(resenje == NULL)
    {
        printf("Nema linkova u direktorijumu\n");
    }
    else
    {
        printf("%s\n", resenje);
    }
    free(resenje);
    closedir(d);
    
    return 0;
}