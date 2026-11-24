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

static void obidji(const char *tekuci, time_t *max_vreme, char **resenje)
{   
    errno = 0;
    DIR *d = opendir(tekuci);
    check_error(d != NULL, "opendir");

    struct dirent *dir;

    while(1)
    {   
        errno = 0;
        dir = readdir(d);
        if(dir == NULL)
        {
            check_error(errno == 0, "readdir");
            break;
        }
        if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
        {
            continue;
        }
        char full_path[4096];
        int s = snprintf(full_path, sizeof(full_path), "%s/%s", tekuci, dir->d_name);
        check_error(s != sizeof(full_path), "snprintf");

        struct stat fInfo;
        check_error(lstat(full_path, &fInfo) != -1, "lstat");

        if(S_ISLNK(fInfo.st_mode))
        {
            if(*max_vreme < fInfo.st_mtime)
            {
                *max_vreme = fInfo.st_mtime;
                char *kopija = strdup(dir->d_name);
                free(*resenje);
                *resenje = kopija;
            }
        }
        else if(S_ISDIR(fInfo.st_mode))
        {
            obidji(full_path, max_vreme, resenje);
        }
    }
    check_error(closedir(d) != -1, "closedir");
}

int main(int argc, char** argv)
{
    check_error(argc == 2, "./a.out putanja_do_direktorijuma");

    time_t max_vreme = -1;
    
    char *resenje = NULL;

    obidji(argv[1], &max_vreme, &resenje);

    if(resenje != NULL)
    {
        printf("%s\n", resenje);
    }
    else
    {
        printf("Neuspeh");
    }
    free(resenje);
    return 0;

    

}