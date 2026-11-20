#include<stdio.h>
#include<stdlib.h>
#include <sys/stat.h>


#define check_error(cond, msg)\
    do                                                                                              \
    {   \
        perror(msg);\
        if(!(cond))\
        {\
            fprintf(stderr, "File: %s\nFunc: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
            exit(EXIT_FAILURE);\
        }\
    } while (0)

#define UNUSED(x) ((void)x)

int main(int argc, char**argv)
{
    UNUSED(argc);
    mode_t mode = (S_IRUSR | S_IWUSR) | (S_IRGRP | S_IWGRP) | (S_IROTH);

    chmod(argv[1], mode);

    return 0;
}
    