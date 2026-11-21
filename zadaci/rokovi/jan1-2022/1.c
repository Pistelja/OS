#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>



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

int main(int argc, char **argv)
{
    int i = 1;
    int br = 0;
    int max_velicina = -1;
    char *najveci_fajl = NULL;
    while(i < argc)
    {
        char *fpath = argv[i];
        struct stat fInfo;
        char *kopija = strdup(argv[i]);
        stat(fpath, &fInfo);
        if(!(S_ISREG(fInfo.st_mode)))
        {   
            i++;
            continue;
        }
        if(max_velicina < fInfo.st_size)
        {
            max_velicina = fInfo.st_size;
            free(najveci_fajl);
            najveci_fajl = strdup(basename(kopija));
            br++;
        }
        free(kopija);
        i++;
    }
    if(br == 0)
        printf("\nneuspeh\n");
    else
    {
        printf("%s %d\n", najveci_fajl, max_velicina);
    }
    
    return 0;
}
    