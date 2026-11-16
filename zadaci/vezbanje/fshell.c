#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>


#define check_error(cond,msg)\
    do                                                                                          \
    {\
        if(!(cond))\
        {\
            perror(msg);\
            fprintf(stderr, "File %s\nFunc: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
            exit(EXIT_FAILURE);\
        }\
    } while (0)


#define UNUSED(x) ((void)x)

int main(int argc, char** argv)
{   
    UNUSED(argc);
    UNUSED(argv);

    FILE *stream = stdin;
    char *line = NULL;
    size_t size = 0;
    ssize_t nread;
    printf("%% ");
    errno = 0;
    while((nread = getline(&line, &size, stream))!= -1)
    {   
        line[nread - 1] = '\0'; 
        if(strncmp(line, "exit ", 5*sizeof(char)) == 0)
        {   
            exit(EXIT_SUCCESS);
        }
        else if(strncmp(line, "mkdir ", 6*sizeof(char))== 0)
        {   
            check_error(mkdir(&line[6], 0775) != -1, "mkdir");
        }
        else if(strncmp(line, "mkfile ", 7*sizeof(char))==0)
        {   
            char *path = &line[7];
            int fd = creat(path, 0775);
            check_error(fd != -1, "mkfile");
        }
        else if(strncmp(line, "rm ", 3*sizeof(char))==0)
        {
            char *path = &line[3];
            if(unlink(path) == -1)
            {
                check_error(errno != ENOENT, "Nepostoji fajl koji zelis da obrises");
                check_error(errno == ENOENT, "rm");
            }
        }
        else if(strncmp(line, "rmdir ", 6*sizeof(char))==0)
        {
            char *path = &line[6];
            if(rmdir(path) == -1)
            {
                check_error(errno != ENOENT, "Nepostoji direktorijum koji zelis da obrises");
                check_error(errno == ENOENT, "rmdir");
            }
        }
        else if(strncmp(line, "cat ", 4*sizeof(char))==0)
        {
            char *path = &line[4];
            int fd = open(path, O_RDONLY);
            if(fd == -1)
            {
                check_error(errno != ENOENT, "Ne postoji fajl koji smo pokusali da otvorimo");
                check_error(errno == ENOENT, "cat open");
            }
            off_t ofset = lseek(fd, 0, SEEK_END);
            check_error(ofset != -1, "cat lseek");

            char *buffer = malloc(ofset + 1);

            lseek(fd,0,SEEK_SET);

            int procitano = read(fd, buffer, ofset);
            check_error(procitano == ofset, "cat read");
            buffer[procitano] = '\0';

            printf("%s\n", buffer);
            free(buffer);
            close(fd);
        }
        else if(strncmp(line, "insert ", 7*sizeof(char))==0)
        {
            char **niz_komandi = malloc(4*sizeof(char *));
            check_error(niz_komandi != NULL, "malloc");

            int i = 0;
            niz_komandi[0] = strtok(&line[7]," ");
            while(niz_komandi[i] != NULL && i < 3)
            {
                i++;
                niz_komandi[i] = strtok(NULL, " ");
            }
            char *rec = niz_komandi[0];
            char *poz = niz_komandi[1];
            char *path = niz_komandi[2];

            char *endptr;
            errno = 0;
            long ofsetarg = strtol(poz, &endptr, 10);

            if(errno != 0 || *endptr != '\0' || ofsetarg < 0)
            {
                fprintf(stderr, "Greska: Pozicija mora biti validan broj!\n");
                free(niz_komandi);
                errno = 0;
                printf("%% ");
                continue;
            }
            
            int fd = open(path, O_RDWR);
            if(fd == -1)
            {
                check_error(errno != ENOENT, "Ne postoji fajl koji smo pokusali da otvorimo");
                check_error(errno == ENOENT, "cat open");
            }


            // off_t ofsetarg = atoi(poz);

            off_t kraj_fajla= lseek(fd, 0, SEEK_END);
            check_error(kraj_fajla !=-1, "insert: lseek end");

            if(ofsetarg >kraj_fajla)
            {
                ofsetarg = kraj_fajla;
            }

            off_t preostalo = kraj_fajla - ofsetarg;
            char *tmp_buffer = NULL;

            if(preostalo > 0)
            {   
                tmp_buffer = malloc(preostalo);
                check_error(tmp_buffer != NULL, "insert malloc tmpbufer");

                check_error(lseek(fd, ofsetarg, SEEK_SET) != -1, "insert: lseek read");
                ssize_t procitano = read(fd, tmp_buffer, preostalo);
                check_error(procitano == preostalo, "insert: read preostalo"); 
            }


            off_t ofset = lseek(fd, ofsetarg, SEEK_SET);
            check_error(ofset != -1, "insert lseek write");

            ssize_t duzina_reci = strlen(rec);

            ssize_t zapisano = write(fd, rec, duzina_reci);
            check_error(zapisano == duzina_reci, "insert write");

            if(preostalo > 0)
            {
                ssize_t zapisano_ostatak = write(fd, tmp_buffer, preostalo);
                check_error(zapisano_ostatak == preostalo, "write preostalo");
                free(tmp_buffer);
            }

            close(fd);
            free(niz_komandi);
            
        }
        errno = 0;
        printf("%% ");
    }

    free(line);
    
}
