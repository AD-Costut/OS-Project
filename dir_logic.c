#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>

#define PATH_MAX 250

void dir_logic(char dir_path[])
{
    struct stat dir_stat;
    char options[3];

    printf("Select an option: ");
    scanf("%s", options);

    if (strstr(options, "-n"))
    {
        printf("Directory name: %s\n", dir_path);
    }

    if (strstr(options, "-d"))
    {
        if (stat(dir_path, &dir_stat) == -1)
        {
            printf("Error: Unable to get directory status\n");
            return;
        }

        printf("Directory size: %ld bytes\n", dir_stat.st_size);
    }

    if (strstr(options, "-a"))
    {
        print_access_rights(dir_stat);
    }

    if (strstr(options, "-c"))
    {
        DIR *dir = opendir(dir_path);
        if (!dir)
        {
            printf("Error: Unable to open directory\n");
            return;
        }

        int count = 0;
        struct dirent *entry;
        while ((entry = readdir(dir)))
        {
            char file_path[PATH_MAX];
            snprintf(file_path, PATH_MAX, "%s/%s", dir_path, entry->d_name);

            if (stat(file_path, &dir_stat) == 0 && S_ISREG(dir_stat.st_mode))
            {
                char *ext = strrchr(entry->d_name, '.');
                if (ext && strcmp(ext, ".c") == 0)
                {
                    count++;
                }
            }
        }

        closedir(dir);
        printf("Total number of files with .c extension: %d\n", count);
    }
}
