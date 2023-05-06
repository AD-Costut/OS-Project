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
#define PATH_MAX 250

void print_access_rights(struct stat file_stat)
{
    printf("Access rights: ");
    printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
    printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
    printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
    printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
    printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
    printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
    printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
    printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
    printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
    printf((file_stat.st_mode & S_IXOTH) ? "x\n" : "-\n");
}
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

void regular_file_logic(char file_path[])
{

    struct stat file_stat;
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
}

void symbolic_link_logic(char symlink_path[]) {
    struct stat symlink_stat;
    char options[3];

    printf("Select an option:\n");
    scanf("%s", options);

    if (strstr(options, "-n")) {
        printf("Symbolic link name: %s\n", symlink_path);
    }

    if (strstr(options, "-l")) {
        if (unlink(symlink_path) == -1) {
            printf("Error deleting symbolic link: %s\n", strerror(errno));
            return;
        }

        printf("Symbolic link deleted successfully\n");
        return;
    }

    if (stat(symlink_path, &symlink_stat) == -1) {
        printf("Error: Unable to get symbolic link status\n");
        return;
    }

    if (strstr(options, "-d")) {
        printf("Symbolic link size: %ld bytes\n", symlink_stat.st_size);
    }

    if (strstr(options, "-t")) {
        char resolved_path[PATH_MAX];
        if (realpath(symlink_path, resolved_path) == NULL) {
            printf("Error: Unable to get resolved path\n");
            return;
        }

        struct stat target_stat;
        if (stat(resolved_path, &target_stat) == -1) {
            printf("Error: Unable to get target file status\n");
            return;
        }

        printf("Target file size: %ld bytes\n", target_stat.st_size);
    }

    if (strstr(options, "-a")) {
        print_access_rights(symlink_stat);
    }
}


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

void files_menu(int argc, char *dir_path[])
{
    struct stat file_stat;

    for (int i = 1; i < argc; i++)
    {
        if (lstat(dir_path[i], &file_stat) == -1)
        {
            printf("Error: Unable to get file status for %s\n", dir_path[i]);
            continue;
        }

        if (S_ISREG(file_stat.st_mode))
        {
            printf("%s is a regular file\n", dir_path[i]);
            printf("\n------MENU------\n\n");
            printf("name (-n)\nsize (-d)\nhard link count (-h)\ntime of last modification (-m)\naccess rights (-a)\ncreate symlink (-l)\n");
            regular_file_logic(dir_path[i]);
        }
        else if (S_ISDIR(file_stat.st_mode))
        {
            printf("%s is a directory\n", dir_path[i]);
            printf("\n------MENU------\n\n");
            printf("name (-n)\nsize (-d)\nfiles with .c extension (-c)\naccess rights (-a)\n");
            dir_logic(dir_path[i]);
        }
        else if (S_ISLNK(file_stat.st_mode))
        {
            printf("%s is a symbolic link\n", dir_path[i]);
            printf("\n------MENU------\n\n");
            printf("name (-n)\ndelete symlink (-l)\nsize of symlink (-d)\nsize of target file (-t)\naccess rights (-a)\n");
            symbolic_link_logic(dir_path[i]);
        }
    }
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        printf("Usage: %s [directory path]\n", argv[0]);
        return 1;
    }

    files_menu(argc, argv);

    return 0;
}
