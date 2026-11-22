#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <ftw.h>
#include <string.h>
#include <libgen.h>



#define check_error(cond, msg)                                                                      \
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

static int ima_vise_vlasnika(gid_t gid)
{
    struct group *g = getgrgid(gid);
    check_error(g != NULL, "getgrgid");
    int i = 0;

    while(g->gr_mem[i] != NULL)
    {
        i++;
    }
    return i > 1;
}

int ispisi_nazive(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    if(ftwbuf->level == 3 && typeflag != FTW_F &&  ima_vise_vlasnika(sb->st_gid))
    {   
        char *kopija = strdup(fpath);
        char *ime = basename(kopija);
        printf("%s\n", ime);
        free(kopija);
    }
    return 0;
}

int main(int argc, char** argv)
{   
    UNUSED(argc);
    UNUSED(argv);
    char *putanja = getenv("PUTANJA");
    check_error(putanja != NULL, "getenv");

    struct stat fInfo;

    check_error(stat(putanja, &fInfo)!=-1, "stat");

    check_error(S_ISDIR(fInfo.st_mode), "Nije direktorijum");

    check_error(nftw(putanja, ispisi_nazive, 50, FTW_PHYS) != -1, "nftw");



    return 0;

}