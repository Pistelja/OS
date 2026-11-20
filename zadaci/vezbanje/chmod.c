#include<stdio.h>
#include<stdlib.h>
#include <sys/stat.h>


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


int main(int argc, char** argv)
{
    check_error(argc == 2, "./a.out putanja_do_fajla");
    
    struct stat finfo;

    check_error(stat(argv[1], &finfo)!= -1, "stat");

    mode_t prava_fajla = finfo.st_mode;
    prava_fajla &= ~(S_IROTH);
    prava_fajla |= S_IWGRP;


    check_error(chmod(argv[1],prava_fajla)!= -1, "chmod");

    return 0;

}