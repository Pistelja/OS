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

int size = 0;


int dir_size(const char* fpath, const struct stat *sb, int flag, struct FTW *fwbuff)
{
    UNUSED(fpath);
    UNUSED(flag);
    UNUSED(fwbuff);

    size += sb->st_size;

    return 0;
}

int main(int argc, char** argv)
{

    check_error(argc == 2, "./a.out putanja do direktorijuma");

    struct stat fInfo;
    check_error(stat(argv[1],&fInfo) != -1, "stat");
    check_error((fInfo.st_mode & S_IFMT) == S_IFDIR, "nije direktorijum");

    check_error(nftw(argv[1], dir_size, 50, 0) != -1, "nftw");

    printf("Velicina direktorijuma je %d\n", size);
    return 0;
}