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

int ispisi(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{   
    UNUSED(ftwbuf);
    
    if(typeflag == FTW_F && S_ISREG(sb->st_mode))
    {   
        char *kopija = strdup(fpath);
        check_error(kopija != NULL, "strdup");
        char *ime = basename(kopija);
        char buf[11];
        buf[0] = '-';
        mode_t mode = sb->st_mode;
        buf[1] = (mode & S_IRUSR) ? 'r' : '-';
        buf[2] = (mode & S_IWUSR) ? 'w' : '-';
        buf[3] = (mode & S_IXUSR) ? 'x' : '-';

        buf[4] = (mode & S_IRGRP) ? 'r' : '-';
        buf[5] = (mode & S_IWGRP) ? 'w' : '-';
        buf[6] = (mode & S_IXGRP) ? 'x' : '-';

        buf[7] = (mode & S_IROTH) ? 'r' : '-';
        buf[8] = (mode & S_IWOTH) ? 'w' : '-';
        buf[9] = (mode & S_IXOTH) ? 'x' : '-';

        buf[10] = '\0';
        printf("%s %s\n", buf, ime);
        free(kopija);
    }
    return 0;
}

int main(int argc, char** argv)
{
    check_error(argc == 2, "./a.out putanja_do_direktorijuma");
    check_error(nftw(argv[1], ispisi, 50, FTW_PHYS)!= -1, "nftw");
    return 0;


}