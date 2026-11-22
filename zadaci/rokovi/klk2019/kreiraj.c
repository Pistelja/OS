#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>




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
    UNUSED(argc);
    UNUSED(argv);
    mode_t stara = umask(0);
    check_error(mkdir("Ispit", 0775) != -1, "mkdir");

    check_error(mkdir("./Ispit/Predavanja", 0775) != -1, "mdkir");

    check_error(mkdir("./Ispit/Vezbe", 0775) != -1, "mkdir");

    int fd1 = open("./Ispit/Predavanja/Cas1.txt", O_RDWR | O_CREAT, 0777);
    check_error(fd1 != -1, "open");
    int fd2 = open("./Ispit/Predavanja/Cas2.txt", O_RDWR | O_CREAT, 0777);
    check_error(fd2 != -1, "open");
    int fd3 = open("./Ispit/Predavanja/Pomocno.txt", O_RDWR | O_CREAT, 0777);
    check_error(fd3 != -1, "open");

    
    char cwd[256];
    check_error(getcwd(cwd, sizeof(cwd)) != NULL, "getcwd");

    char putanja[256];
    int n = snprintf(putanja, sizeof(putanja), "%s/Ispit/Predavanja/Cas1.txt", cwd);
    UNUSED(n);

    check_error(symlink(putanja, "./Ispit/Vezbe/Predavanja1") != -1, "symlink");

    umask(stara);

    return 0;


}
    