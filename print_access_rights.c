#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>

void print_access_rights(struct stat fileStat)
{

    printf("\nUser:\n");
    printf("Read - %s\n", (fileStat.st_mode & S_IRUSR) ? "yes" : "no");
    printf("Write - %s\n", (fileStat.st_mode & S_IWUSR) ? "yes" : "no");
    printf("Exec - %s\n", (fileStat.st_mode & S_IXUSR) ? "yes" : "no");

    printf("\nGroup:\n");
    printf("Read - %s\n", (fileStat.st_mode & S_IRGRP) ? "yes" : "no");
    printf("Write - %s\n", (fileStat.st_mode & S_IWGRP) ? "yes" : "no");
    printf("Exec - %s\n", (fileStat.st_mode & S_IXGRP) ? "yes" : "no");

    printf("\nOthers:\n");
    printf("Read - %s\n", (fileStat.st_mode & S_IROTH) ? "yes" : "no");
    printf("Write - %s\n", (fileStat.st_mode & S_IWOTH) ? "yes" : "no");
    printf("Exec - %s\n", (fileStat.st_mode & S_IXOTH) ? "yes" : "no");
}