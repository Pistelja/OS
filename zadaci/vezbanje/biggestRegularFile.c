#include<stdio.h>
#include<stdlib.h>
#include <sys/stat.h>
#include<errno.h>
#include <string.h>


#define _XOPEN_SOURCE 700

#define check_error(cond,msg)                                                                    \
    do\
    {\
         if(!(cond))\
    {   \
        perror(msg);\
        fprintf(stderr, "File: %s\nLine: %d\nFunc: %s\n", __FILE__, __LINE__, __func__);\
        exit(EXIT_FAILURE);\
    }\
    }while(0)


   

int main(int argc, char** argv)
{
    check_error(argc > 2, "./a.out file1 file2 file3 ...");

    struct stat sb;

    struct stat sb_max;
    char *ime;
    int max_velicina = -1;

    for(int i = 1; i < argc; i++)
    {   
        errno = 0;
        if(lstat(argv[i], &sb) == -1)
        {
            check_error((errno & ENOENT) == 0, "Greska");
            printf("Fajl ne postoji ili je prosledjen nepostojeci simbolicki link");
            exit(EXIT_SUCCESS);
        }
        if((sb.st_mode & S_IFMT) == S_IFREG) 
        {
            if(sb.st_size > max_velicina)
            {
                max_velicina = sb.st_size;
                sb_max = sb;
                ime = malloc(sizeof(char)*(strlen(argv[i])+1));
                strcpy(ime, argv[i]);
            }
        }
    }
    printf("%d\n", max_velicina);
    printf("%s\n", ime);
    printf("%d\n", sb_max.st_gid);

    return 0;


}