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
#define NEDELJA 7*24*60*60

int total_size = 0;
char *ekstenzija = NULL;

int size_of_regular_files(const char *fpath, const struct stat *sb,int typeflag, struct FTW *ftwbuf)
{   
    time_t now = time(NULL);
    UNUSED(ftwbuf);

    char *kopija = strdup(fpath);
    check_error(kopija != NULL, "stdup");
    char *ime = basename(kopija);
    char *ext = strrchr(ime, '.');

    if((typeflag == FTW_F && S_ISREG(sb->st_mode)) && (now - sb->st_mtime) < NEDELJA && ext != NULL && strcmp(ext,ekstenzija) == 0)
    {
        total_size += sb->st_size;
    }
    free(kopija);
    return 0;
}

int main(int argc, char **argv)
{
    check_error(argc == 3, "./a.out putanja_do_direktorijuma ekstenzija");

    ekstenzija = argv[2];

    check_error(nftw(argv[1], size_of_regular_files,50,FTW_PHYS) != -1, "nftw");

    printf("%d\n", total_size);

    return 0;

}   