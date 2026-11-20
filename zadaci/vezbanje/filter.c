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

char *ekstenzija = NULL;


int filter(const char* fpath, const struct stat *sb, int flag, struct FTW *fwbuff)
{
    UNUSED(flag);
    UNUSED(fwbuff);

    if(flag == FTW_F)
    {   
        char *fName = basename(fpath);
        char *ext = strchr(fName,'.');

        if(ext != NULL && (strcmp(ext, ekstenzija) == 0))
        {
            printf("%-80s\n", fName);
        }
    }

    return 0;
}

int main(int argc, char** argv)
{

    check_error(argc == 3, "./a.out putanja_do_direktorijuma ekstenzija");

    struct stat fInfo;
    ekstenzija = argv[2];
    check_error(stat(argv[1],&fInfo) != -1, "stat");
    check_error((fInfo.st_mode & S_IFMT) == S_IFDIR, "nije direktorijum");

    check_error(nftw(argv[1], filter, 50, 0) != -1, "nftw");

    
    return 0;
}