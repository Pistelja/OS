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

int main(int argc, char** argv)
{
    check_error(argc ==2, "./a.out putanja_do_direktorijuma");

    struct dirent *d = NULL;

    DIR *dir_stream = opendir(argv[1]);
    check_error(dir_stream != NULL, "opendir");

    errno = 0;
    while((d = readdir(dir_stream))!= NULL)
    {
        if(strcmp(d->d_name, ".")!= 0 && strcmp(d->d_name, "..")!= 0)
        {   
            char *file_path = malloc(strlen(argv[1]) + 1 + strlen(d->d_name) + 1);
            check_error(file_path != NULL, "malloc");

            strcpy(file_path, argv[1]);
            strcat(file_path, "/");
            strcat(file_path, d->d_name);

            struct stat fInfo;
            check_error(stat(file_path, &fInfo) != -1, "stat");
            free(file_path);

            struct passwd *user_info = getpwuid(fInfo.st_uid);
            printf("%s %s\n", user_info->pw_name, d->d_name);
        }
        errno = 0;
    }
    check_error(errno == 0, "readdir");

    closedir(dir_stream);

    return 0;
 

}