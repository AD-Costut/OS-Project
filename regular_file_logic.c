#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>

#define BUFFER_SIZE 256

void regular_file_logic(char file_path[], struct stat file_stat)
{

    char options[3];

    printf("Select an option: ");
    scanf("%s", options);

    if (strstr(options, "-n"))
    {
        printf("File name: %s\n", file_path);
    }

    if (strstr(options, "-d"))
    {
        printf("File size: %ld bytes\n", file_stat.st_size);
    }

    if (strstr(options, "-h"))
    {
        printf("Hard link count: %ld\n", (long)file_stat.st_nlink);
    }

    if (strstr(options, "-m"))
    {
        printf("Time of last modification: %s", ctime(&file_stat.st_mtime));
    }

    if (strstr(options, "-a"))
    {
        print_access_rights(file_stat);
    }

    if (strstr(options, "-l"))
    {
        printf("\nEnter symlink name: ");
        char symlink_name[BUFFER_SIZE];
        scanf("%s", symlink_name);

        if(symlink(file_path, symlink_name)==-1){
            perror("symlink");
        }
        printf("Symlink points to: %s\n", file_path);
    }

}
