#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>

#define MAX_OPTIONS 100
#define BUFFER_SIZE 256

void regular_file_menu(char *file_path)
{
    int file_descriptor = open(file_path, O_RDONLY);

    if (file_descriptor == -1)
    {
        printf("Error opening file: %s\n", strerror(errno));
        return;
    }

    struct stat file_stat;
    if (fstat(file_descriptor, &file_stat) == -1)
    {
        printf("Error getting file stats: %s\n", strerror(errno));
        close(file_descriptor);
        return;
    }

    printf("File name: %s\n", file_path);
    printf("File size: %lld bytes\n", (long long int)file_stat.st_size);
    printf("Hard link count: %ld\n", (long)file_stat.st_nlink);
    printf("Time of last modification: %s", ctime(&file_stat.st_mtime));
    print_access_rights(file_stat);

    char options[MAX_OPTIONS];
    printf("Enter options string: ");
    fgets(options, MAX_OPTIONS, stdin);

    int length = strlen(options);
    if (options[length - 1] == '\n')
    {
        options[length - 1] = '\0';
    }

    if (strstr(options, "-n"))
    {
        printf("File name: %s\n", file_path);
    }

    if (strstr(options, "-d"))
    {
        printf("File size: %lld bytes\n", (long long)file_stat.st_size);
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
        printf("Enter symlink name: ");
        char symlink_name[BUFFER_SIZE];
        fgets(symlink_name, BUFFER_SIZE, stdin);

        char resolved_path[BUFFER_SIZE];
        if (readlink(symlink_name, resolved_path, BUFFER_SIZE) == -1)
        {
            printf("Error reading symlink: %s\n", strerror(errno));
            return;
        }

        printf("Symlink points to: %s\n", resolved_path);
    }

    close(file_descriptor);
}