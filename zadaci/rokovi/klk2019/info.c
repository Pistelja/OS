#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>






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

int main(int argc, char** argv)
{
    check_error(argc == 2, "./a.out putanja_do_fajla");
    struct stat fInfo;
    check_error(stat(argv[1], &fInfo)!= -1, "stat");
    check_error(S_ISREG(fInfo.st_mode), "fajl nije regularan");


    struct passwd *p = getpwuid(fInfo.st_uid);
    struct group *g = getgrgid(fInfo.st_gid);

    printf("Tip fajla: ");

           switch (fInfo.st_mode & S_IFMT) {
           case S_IFBLK:  printf("b\n");            break;
           case S_IFCHR:  printf("c\n");            break;
           case S_IFDIR:  printf("d\n");            break;
           case S_IFIFO:  printf("F\n");            break;
           case S_IFLNK:  printf("s\n");            break;
           case S_IFREG:  printf("r\n");            break;
           case S_IFSOCK: printf("s\n");            break;
           default:       printf("unknown?\n");     break;
           }



    printf("Velicina fajla: %d\n", (int)(fInfo.st_size));
    printf("Ime vlasnika:%s\nIme grupe: %s\n", p->pw_name, g->gr_name);
    char* vreme_modifikacije = ctime(&fInfo.st_mtime);
    printf("Vreme modifikacije: %s", vreme_modifikacije);

    return 0;

}