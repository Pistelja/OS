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

char *resenje = NULL;
int max_modifikovano = 0;

int izdvoji_simbolicke(const char *fpath, const struct stat *sb,int typeflag, struct FTW *ftwbuf)
{   
    UNUSED(typeflag);
    if(ftwbuf->level > 1)
    {
        return 0;
    }
    char *kopija = strdup(fpath);
    check_error(kopija != NULL, "strdup");
    if(S_ISLNK(sb->st_mode))
    {
        if(max_modifikovano < sb->st_mtime)
        {
            max_modifikovano = sb->st_mtime;
            free(resenje);
            resenje = strdup(basename(kopija));
            check_error(resenje != NULL, "strdup");
        }
    }
    free(kopija);
    return 0;
}

int main(int argc, char** argv)
{
    check_error(argc == 2, "./a.out putanja_do_direktorijuma");
    
    check_error(nftw(argv[1],izdvoji_simbolicke,50, FTW_PHYS) != -1, "nftw");

    
    if(resenje != NULL)
    {
        printf("%s\n", resenje);
        free(resenje);
    }
    else
    {
        printf("Ne postoji simbolicki link");
    }
   
    return 0;
}